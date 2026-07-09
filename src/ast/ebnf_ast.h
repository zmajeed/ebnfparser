#ifndef EBNF_AST_H
#define EBNF_AST_H
// ast/ebnf_ast.h

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

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <functional>
#include <variant>
#include <print>

namespace ebnfparser {
using namespace std;

struct Grammar;
struct Header;
struct Rule;
struct Item;
struct Repetition;
struct Alternative;
struct Concatenation;
struct Optional;
struct Group;

using Symbol = string;
using Nonterminal = Symbol;
using Token = Symbol;
using Literal = Symbol;

struct Header {
  vector<string> lines{};
};

struct Rule {
  string nonterminal{};
  vector<Alternative> alternatives;
};

struct Grammar {
  Header header{};
  vector<Rule> rules{};
};

struct Concatenation {
  vector<Repetition> reps{};
};

struct AltBase {
  vector<Concatenation> concats{};
};

struct Alternative: public AltBase {
};

struct Optional: public AltBase {
};

struct Group: public AltBase {
};

struct Item: variant<Symbol, Optional, Group> {
  using variant::variant;
};

struct Repetition { 
  bool isRepeated = false;
  Item item{};
};


struct AstNode: variant<Grammar, Header, Rule, Alternative, Concatenation, Repetition, Optional, Item, Symbol, Group> {
  using variant::variant;

  template<class... Ts> struct overload: Ts... { using Ts::operator()...; };

  void printAst() const {

    visit(overload{

      [](this auto&& self, const Grammar& g) -> void {
        println("grammar:");
        self(g.header);
        println("{} rules:", g.rules.size());
        for(auto& rule: g.rules) {
          self(rule);
        }
      },

      [](this auto&& self, const Rule& r) -> void {
        println("rule:");
        println("{}:", r.nonterminal);
        for(auto& alt: r.alternatives) {
          self(alt);
        }
      },

      [](this auto&& self, const Alternative& a) -> void {
        println("alternative:");
        for(auto& concat: a.concats) {
          self(concat);
        }
      },

      [](this auto&& self, const Group& g) -> void {
        println("group:");
        for(auto& concat: g.concats) {
          self(concat);
        }
      },

      [](this auto&& self, const Optional& o) -> void {
        println("optional:");
        for(auto& concat: o.concats) {
          self(concat);
        }
      },

      [](this auto&& self, const Concatenation& c) -> void {
        println("concatenation:");
        for(auto& rep: c.reps) {
          self(rep);
        }
      },

      [](this auto&& self, const Repetition& r) -> void {
        println("repetition:");
        r.item.visit(self);
      },

      [](this auto&& self, const Item& i) -> void {
        println("item: should never match");
        i.visit(self);
      },

      [](this auto&&, const Symbol& s) -> void {
        println("symbol: {}", s);
      },

      [](this auto&&, const Header& h) -> void {
        println("header:");
        for(auto& line: h.lines) {
          print("{}", line);
        }
      },

      [](this auto&&, const auto&) -> void {
        println("default visit");
      },

    });
  }

};

}

#endif

