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
        self(g.header);
        for(const auto& rule: g.rules) {
          self(rule);
          println("\n");
        }
      },

      [](this auto&& self, const Rule& r) -> void {
        const auto& alts = r.alternatives;

        print("{} ::=", r.nonterminal);
        if(alts.empty()) {
          return;
        }
        print(" ");
        self(alts[0]);
        for(int i = 1; i < ssize(alts); ++i) {
          const auto& alt = alts[i];
          print(" |");
          self(alt);
        }
      },

      [](this auto&& self, const Alternative& a) -> void {
        const auto& concats = a.concats;

        if(concats.empty()) {
          return;
        }
        self(concats[0]);
        for(int i = 1; i < ssize(concats); ++i) {
          const auto& concat = concats[i];
          print(" |");
          self(concat);
        }
      },

      [](this auto&& self, const Group& g) -> void {
        const auto& concats = g.concats;

        print("{{");

        if(concats.empty()) {
          return;
        }
        self(concats[0]);
        for(int i = 1; i < ssize(concats); ++i) {
          const auto& concat = concats[i];
          print(" |");
          self(concat);
        }

        print(" }}");
      },

      [](this auto&& self, const Optional& o) -> void {
        const auto& concats = o.concats;

        print("[");

        if(concats.empty()) {
          return;
        }
        self(concats[0]);
        for(int i = 1; i < ssize(concats); ++i) {
          const auto& concat = concats[i];
          print(" |");
          self(concat);
        }

        print(" ]");
      },

      [](this auto&& self, const Concatenation& c) -> void {
        for(const auto& rep: c.reps) {
          print(" ");
          self(rep);
        }
      },

      [](this auto&& self, const Repetition& r) -> void {
        self(r.item);
        if(r.isRepeated) {
          print(" ...");
        }
      },

      [](this auto&& self, const Item& i) -> void {
        i.visit(self);
      },

      [](this auto&&, const Symbol& s) -> void {
        print("{}", s);
      },

      [](this auto&&, const Header& h) -> void {
        for(const auto& line: h.lines) {
          print("{}", line);
        }
      },

      [](this auto&&, const auto&) -> void {
        println("unexpected ast node");
      },

    });
  }

};

}

#endif

