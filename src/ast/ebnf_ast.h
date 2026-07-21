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
#include <initializer_list>
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

struct AltBase;
struct Alternative;
struct Concatenation;
struct Grammar;
struct Group;
struct Header;
struct Item;
struct Optional;
struct Repetition;
struct Rule;

using Symbol = string;
using Nonterminal = Symbol;
using Token = Symbol;
using Literal = Symbol;


// AltBase, base class for Alternative, Optional, Group

struct AltBase {
  flat_set<Concatenation> concats;

  strong_ordering operator<=>(const AltBase& other) const;
  bool operator==(const AltBase& other) const = default;

};

// Alternative

struct Alternative: public AltBase {
};

// Optional

struct Optional: public AltBase {
};

// Group

struct Group: public AltBase {
};

// Rule
// may need to define constructors and destructors outside class if cyclic dependency errors can't be fixed by reordering class definitions
// Rule(), Rule(string nt, initializer_list<Alternative> init), ~Rule()

struct Rule {
  string nonterminal{};

// hopefully flat_set is fast enough for iterative applications like convert to bnf that doesn't need lookup
// could go back to vector<Alternative>
  flat_set<Alternative> alts;

};

// Header

struct Header {
  vector<string> lines{};
};

// Grammar

struct Grammar {
  Header header{};
  vector<Rule> rules;
};

// Item

struct Item: variant<Symbol, Optional, Group> {
  using variant::variant;

  strong_ordering operator<=>(const Item& other) const;
  bool operator==(const Item& other) const = default;

};

// Repetition

struct Repetition { 
  bool isRepeated = false;
  Item item;

  strong_ordering operator<=>(const Repetition& other) const = default;
};

// Concatenation

struct Concatenation {
  vector<Repetition> reps;

  strong_ordering operator<=>(const Concatenation& other) const;
  bool operator==(const Concatenation& other) const = default;

};


// AstNode

struct AstNode: variant<Grammar, Header, Rule, Alternative, Concatenation, Repetition, Optional, Item, Symbol, Group> {
  using variant::variant;

  void printAst() const;

private:

#if WIN32
  template<class... Ts> struct __declspec(empty_bases) overload: Ts... { using Ts::operator()...; };
#else
  template<class... Ts> struct overload: Ts... { using Ts::operator()...; };
#endif
  template<class... Ts> overload(Ts...) -> overload<Ts...>;

};

// annoying ifdefs due to differences between gcc 16 and vc++ 18
// hope it goes away once vc++ supports c++26 variant visit member function

inline
void AstNode::printAst() const {

  visit(overload{

    [](this auto&& self, const Grammar& g) -> void {
      self(g.header);
      for(const auto& rule: g.rules) {
        self(rule);
        println("\n");
      }
    },

    [](this auto&& self, const Rule& r) -> void {
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

    [](this auto&& self, const Alternative& a) -> void {
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

    [](this auto&& self, const Group& g) -> void {
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

    [](this auto&& self, const Optional& o) -> void {
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

// new c++26 variant visit member function, takes 1 parameter
// old variant visit global function, takes 2 parameters
#if __cpp_lib_variant >= 202306L
      i.visit(self);
#else
      visit(self, i);
#endif
    },

    [](this auto&&, const Symbol& s) -> void {
      print("{}", s);
    },

    [](this auto&&, const Header& h) -> void {
      for(const auto& line: h.lines) {
        print("{}", line);
      }
    },

// default match
    [](this const auto&&, const auto&) -> void {
      println("printAst.new: unexpected default match");
    },

// new c++26 variant visit member function, takes 1 parameter
// old variant visit global function, takes 2 parameters
#if __cpp_lib_variant >= 202306L
  });
#else
  }, *this);
#endif

}

// these must be defined outside class

// Concatenation

// for flat_set<Concatenation>
inline
strong_ordering Concatenation::operator<=>(const Concatenation& other) const {
  return reps <=> other.reps;
}

// AltBase

// for flat_set<Alternative>
inline
strong_ordering AltBase::operator<=>(const AltBase& other) const {
  return concats <=> other.concats;
}

// Item

// for flat_set<Alternative> and flat_set<Concatenation>
inline
strong_ordering Item::operator<=>(const Item& other) const {
    return static_cast<const variant<Symbol, Optional, Group>&>(*this)
      <=> static_cast<const variant<Symbol, Optional, Group>&>(other);
}


}

#endif

