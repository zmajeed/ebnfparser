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
#include <flat_set>
#include <map>
#include <variant>
#include <compare>
#include <utility>
#include <algorithm>
#include <memory>
#include <functional>
#include <print>

namespace ebnfparser {
using namespace std;

struct Grammar;
struct Header;
struct Rule;
struct Item;
struct Repetition;
struct AltBase;
struct Alternative;
struct Concatenation;
struct Optional;
struct Group;
struct CompareAlt;

using Symbol = string;
using Nonterminal = Symbol;
using Token = Symbol;
using Literal = Symbol;

struct Header {
  vector<string> lines{};
};

struct Grammar {
  Header header{};
  vector<Rule> rules{};
};

struct Rule {
  string nonterminal{};
// hopefully flat_set is fast enough for iterative applications like convert to bnf that doesn't need lookup
// could go back to vector<Alternative>
  flat_set<Alternative> alts{};
};

struct Concatenation {
  vector<Repetition> reps{};

  strong_ordering operator<=>(const Concatenation& other) const = default;
};

struct AltBase {
  flat_set<Concatenation> concats{};

  strong_ordering operator<=>(const AltBase& other) const = default;
};

struct Alternative: public AltBase {
};

struct Optional: public AltBase {
};

struct Group: public AltBase {
};

struct Item: variant<Symbol, Optional, Group> {
  using variant::variant;

  strong_ordering operator<=>(const Item& other) const;
  bool operator==(const Item& other) const = default;

private:
  template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
};

struct Repetition { 
  bool isRepeated = false;
  Item item{};

  strong_ordering operator<=>(const Repetition& other) const = default;
};

inline
strong_ordering Item::operator<=>(const Item& other) const {

  if(index() != other.index()) {
    return index() <=> other.index();
  }

// capture other because visit only takes overload as parameter
// cannot pass second parameter to lambda overloads
  return visit(overload{

    [&other](const Symbol& s1) -> strong_ordering {
      const auto& s2 = get<Symbol>(other);
      return s1 <=> s2;
    },

    [&other](const Optional& o1) -> strong_ordering {
      const auto& o2 = get<Optional>(other);
      return o1 <=> o2;
    },

    [&other](const Group& g1) -> strong_ordering {
      const auto& g2 = get<Group>(other);
      return g1 <=> g2;
    },

    [](const auto&) -> strong_ordering {
      println("Item.spaceship: unexpected Item type");
      return {{}};
    },
  }
#if __cpp_lib_variant < 202306L
  , *this
#endif
  );
}


struct AstNode: variant<Grammar, Header, Rule, Alternative, Concatenation, Repetition, Optional, Item, Symbol, Group> {
  using variant::variant;

// delicate balance here due to funny business between gcc and vc++ that might go away once vc++ supports c++26 varaint visit member function
// the problem is vc++ doesn't like this auto&& here, has to be this const auto& or this const auto&&
// except for default match that must be this auto&&
// on the other hand, gcc is fine with this auto&& but doesn't like this const auto&&
// so this const auto& self it is, except in default match
  void printAst() const {

    visit(overload{

      [](this const auto& self, const Grammar& g) -> void {
        self(g.header);
        for(const auto& rule: g.rules) {
          self(rule);
          println("\n");
        }
      },

      [](this const auto& self, const Rule& r) -> void {
        const auto& alts = r.alts;

        print("{} ::=", r.nonterminal);
        if(alts.empty()) {
          return;
        }
        print(" ");
        self(alts.begin()[0]);
        for(int i = 1; i < ssize(alts); ++i) {
          const auto& alt = alts.begin()[i];
          print(" |");
          self(alt);
        }
      },

      [](this const auto& self, const Alternative& a) -> void {
        const auto& concats = a.concats;

        if(concats.empty()) {
          return;
        }
        self(concats.begin()[0]);
        for(int i = 1; i < ssize(concats); ++i) {
          const auto& concat = concats.begin()[i];
          print(" |");
          self(concat);
        }
      },

      [](this const auto& self, const Group& g) -> void {
        const auto& concats = g.concats;

        print("{{");

        if(concats.empty()) {
          return;
        }
        self(concats.begin()[0]);
        for(int i = 1; i < ssize(concats); ++i) {
          const auto& concat = concats.begin()[i];
          print(" |");
          self(concat);
        }

        print(" }}");
      },

      [](this const auto& self, const Optional& o) -> void {
        const auto& concats = o.concats;

        print("[");

        if(concats.empty()) {
          return;
        }
        self(concats.begin()[0]);
        for(int i = 1; i < ssize(concats); ++i) {
          const auto& concat = concats.begin()[i];
          print(" |");
          self(concat);
        }

        print(" ]");
      },

      [](this const auto& self, const Concatenation& c) -> void {
        for(const auto& rep: c.reps) {
          print(" ");
          self(rep);
        }
      },

      [](this const auto& self, const Repetition& r) -> void {
        self(r.item);
        if(r.isRepeated) {
          print(" ...");
        }
      },

      [](this const auto& self, const Item& i) -> void {
#if __cpp_lib_variant >= 202306L
        i.visit(self);
#else
        visit(self, i);
#endif
      },

      [](this const auto&, const Symbol& s) -> void {
        print("{}", s);
      },

      [](this const auto&, const Header& h) -> void {
        for(const auto& line: h.lines) {
          print("{}", line);
        }
      },

      [](this auto&&, const auto&) -> void {
        println("printAst: unexpected ast node");
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

}

#endif

