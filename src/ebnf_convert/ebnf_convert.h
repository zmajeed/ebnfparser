#ifndef EBNF_CONVERT_H
#define EBNF_CONVERT_H
// ebnf_convert.h

/*
MIT License

Copyright (c) 2024-2026 Zartaj Majeed

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "ast/ebnf_ast.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <ranges>

namespace ebnfconvert {
using namespace std;
using namespace ebnfparser;

struct BnfGrammar;
struct BnfHeader;
struct BnfRule;
struct BnfChoice;
struct BnfSequence;

using BnfSymbol = string;
using BnfNonterminal = BnfSymbol;
using BnfToken = BnfSymbol;
using BnfLiteral = BnfSymbol;
using BnfEmpty = BnfSymbol;

// Header is sequence of lines
struct BnfHeader {
  bool isWhitespace{};
  vector<string> lines{};
};

// Grammar is sequence of BnfRule
struct BnfGrammar {
  BnfHeader header{};
  vector<BnfRule> rules{};
};

struct BnfRule {
  string nterm{};
  vector<BnfChoice> choices;
};

// BnfSequence is sequence of BnfSymbol
struct BnfSequence {
  vector<BnfSymbol> syms{};
};

// BnfChoice is sequence of BnfSequence
struct BnfChoice {
  vector<BnfSequence> seqs{};
};

struct BnfNode: variant<BnfGrammar, BnfHeader, BnfRule, BnfChoice, BnfSequence, BnfSymbol> {
  using variant::variant;

  void printBnf() const {

    visit(overload{

      [](this const auto& self, const BnfGrammar& g) -> void {
        self(g.header);
        for(auto& rule: g.rules) {
          self(rule);
          println("\n");
        }
      },

      [](this const auto& self, const BnfRule& r) -> void {
        print("{}:", r.nterm);
        for(int i = 0; i < ssize(r.choices); ++i) {
          print("{}", i > 0? "| ": " ");
          self(r.choices[i]);
        }
      },

      [](this const auto& self, const BnfChoice& c) -> void {
        for(int i = 0; i < ssize(c.seqs); ++i) {
          print("{}", i > 0? "| ": " ");
          self(c.seqs[i]);
        }
      },

      [](this const auto& self, const BnfSequence& s) -> void {
        for(auto& sym: s.syms) {
          self(sym);
          print(" ");
        }
      },

      [](this const auto&, const BnfSymbol& s) -> void {
        print("{}", s);
      },

      [](this const auto&, const BnfHeader& h) -> void {
        if(h.isWhitespace) {
          return;
        }
        println("/*");
        for(auto& line: h.lines) {
          print("{}", line);
        }
        println("*/");
        println("");
      },

      [](this auto&&, const auto&) -> void {
        println("unexpected bnf node");
      },
#if __cpp_lib_variant >= 202306L
    });
#else
    }, *this);
#endif
  }

private:
  template<class... Ts> struct overload: Ts... { using Ts::operator()...; };

};

struct EbnfConvert {

  uint64_t numRulesParsed = 0;
  uint64_t numRulesGenerated = 0;

  BnfNode convert(const AstNode& ebnf) const {

    int groupNum = 0;
    int listNum = 0;
    vector<BnfRule> newRules;

// visit ebnf nodes inside alternative
    auto altOverload = overload{

// group
      [&newRules, &groupNum](this auto&& self, const Group& g) -> BnfChoice {
        vector<BnfSequence> seqs;
        for(auto& concat: g.concats) {
          auto choice = self(concat);
          seqs.append_range(choice.seqs);
        }
        return { seqs };
      },

// optional
      [&newRules](this auto&& self, const Optional& o) -> BnfChoice {
        vector<BnfSequence> seqs{ BnfSequence{{ BnfEmpty{"%empty"} }} };
        for(auto& concat: o.concats) {
          auto result = self(concat);
          seqs.append_range(result.seqs);
        }
        return {seqs};
      },

// concatenation
// this is the hard part
// each rep is one or more sequences
// each sequence is one or more symbols, possibly %empty symbol
// extend symbol sequences with symbols from each rep sequence
      [](this auto&& self, const Concatenation& c) -> BnfChoice {

        vector<BnfSequence> seqs;

        for(int i = 0; i < ssize(c.reps); ++i) {
          const auto& rep = c.reps[i];
          const auto& result = self(rep);
          const auto& repSeqs = result.seqs;

          vector<BnfSequence> acc;
          for(int j = 0; j < ssize(repSeqs); ++j) {
            vector<BnfSymbol> joinedSyms;
            for(int k = 0; k < ssize(seqs); ++k) {
              joinedSyms.clear();
// drop %empty for any join
              if(seqs[k].syms[0] == "%empty" && seqs[k].syms.size() > 1) {
                joinedSyms.append_range(ranges::subrange(seqs[k].syms.begin() + 1, seqs[k].syms.end()));
              } else {
                joinedSyms.append_range(seqs[k].syms);
              }
              if(repSeqs[j].syms[0] == "%empty" && repSeqs[j].syms.size() > 1) {
                joinedSyms.append_range(ranges::subrange(repSeqs[j].syms.begin() + 1, repSeqs[j].syms.end()));
              } else {
                joinedSyms.append_range(repSeqs[j].syms);
              }
              acc.push_back(BnfSequence{joinedSyms});
            }
            if(joinedSyms.empty()) {
              acc.push_back(repSeqs[j]);
            }

          }
// 
          seqs = move(acc);
        }
        return BnfChoice{seqs};
      },

// repetition
      [&newRules, &groupNum, &listNum](this auto&& self, const Repetition& r) -> BnfChoice {
        auto result = self(r.item);
        if(!r.isRepeated) {
          return result;
        }
// replace list by generated list name in original ebnf rule
// move list to rhs of new rule for generated list name
        string groupName = format("group_{}", groupNum++);
        string listName = format("{}_list_{}", groupName, listNum++);
        BnfRule groupRule{
          groupName,
          {
            result
          }
        };
        BnfRule leftRecursive{
          listName,
          {
            BnfChoice{{
              BnfSequence{{
                groupName
              }}
            }},
            BnfChoice{{
              BnfSequence{{
                listName,
                groupName
              }}
            }}
          }
        };

        newRules.push_back(groupRule);
        newRules.push_back(leftRecursive);

        return {{
          BnfSequence{{
              listName
          }}
        }};
      },

// item
      [](this auto&& self, const Item& i) -> BnfChoice {
#if __cpp_lib_variant >= 202306L
        return i.visit(self);
#else
        return visit(self, i);
#endif
      },

// symbol
      [](this auto&&, const Symbol& s) -> BnfChoice {
        if(s[0] == '<' && s.back() == '>') {
          auto nterm = regex_replace(s.substr(1, s.length() - 2), regex{"[^a-zA-Z0-9_]"}, "_");
          return { { BnfSequence{ {nterm} } } };
        }
        return { { BnfSequence{ {s} } } };
      },


 // default match
      [](this auto&&, const auto&) -> BnfChoice {
        println("unexpected ast node inside alternative");
        return {};
      },
    };


    auto ebnfOverload = overload{
// grammar
      [&newRules](this const auto& self, const Grammar& g) -> BnfNode {
        BnfHeader header = get<BnfHeader>(self(g.header));
        vector<BnfRule> rules;
        for(auto& rule: g.rules) {
          rules.push_back(get<BnfRule>(self(rule)));
        }
        return BnfGrammar{header, rules};
      },

// rule
      [](this const auto& self, const Rule& r) -> BnfNode {
        auto nterm = regex_replace(r.nonterminal.substr(1, r.nonterminal.length() - 2), regex{"[^a-zA-Z0-9_]"}, "_");
        vector<BnfChoice> choices;
        for(auto& alt: r.alts) {
          choices.push_back(get<BnfChoice>(self(alt)));
        }
        return BnfRule{nterm, choices};
      },

// alternative
      [altOverload](this const auto& self, const Alternative& a) -> BnfNode {
        vector<BnfSequence> seqs;
        for(auto& concat: a.concats) {
          auto newChoice = altOverload(concat);
          seqs.append_range(newChoice.seqs);
        }
        return BnfChoice{seqs};
      },

// header
      [](this const auto&, const Header& h) -> BnfNode {
        bool isWhitespace = true;
        for(const auto& line: h.lines) {
          if(regex_search(line, regex{"\\S"})) {
            isWhitespace = false;
            break;
          }
        }
        return BnfHeader{
          isWhitespace,
          h.lines
        };
      },

      [](this auto&&, const auto&) -> BnfNode {
        println("unexpected convert ebnf node");
        return {};
      },
    };

#if __cpp_lib_variant >= 202306L
    auto result = ebnf.visit(ebnfOverload);
#else
    auto result = visit(ebnfOverload, ebnf);
#endif
    BnfGrammar bnf = get<BnfGrammar>(result);
    bnf.rules.append_range(newRules);

    return bnf;

  }

private:
  template<class... Ts> struct overload: Ts... { using Ts::operator()...; };

};

}
#endif
