#ifndef EBNF_PARSE_H
#define EBNF_PARSE_H
// parser_api/ebnf_parse.h

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

#include "lexer/ebnf_lexer.h"
#include "ebnf_parser.bison.h"

#include <stdlib.h>

#include <string>
#include <istream>
#include <fstream>
#include <iostream>
#include <expected>

#include "ast/ebnf_ast.h"
#include "ebnf_convert/ebnf_convert.h"

namespace ebnfparse {
using namespace std;

using namespace ebnfparser;
using namespace ebnfconvert;

struct Bnf {
  BnfNode root;

  void print() const {
    root.printBnf();
  }

};

struct Ebnf {
  AstNode root;

  void print() const {
    root.printAst();
  }

  expected<Bnf, int> convert() {
    EbnfConvert ebnf;
    return Bnf{move(ebnf.convert(root))};
  }

};

struct EbnfParse {

  static
  expected<Ebnf, int> parse(istream& in, bool debug = false) {

    auto inputFilename = make_unique<string>("stream");

    Lexer lexer(in);

    BisonParam bisonParam;
    LexParam lexParam{.loc = location(inputFilename.get())};

    duration<double> yylexSec{};

    EbnfParser parser(
      [&lexer, &yylexSec](LexParam& lexParam) -> EbnfParser::symbol_type {
        time_point<steady_clock> start = steady_clock::now();
        auto token = lexer.yylex(lexParam);
        time_point<steady_clock> end = steady_clock::now();
        yylexSec += end - start;
        return token;
      },
      bisonParam,
      lexParam
    );

    lexer.set_debug(debug);
    parser.set_debug_level(debug);

    if(auto ev = parser(); ev != 0) {
      println(stderr, "parse failed");
      return unexpected{ev};
    }

    return Ebnf{bisonParam.ast};
  }

  static
  expected<Ebnf, int> parse(const string&& file = "", bool debug = false) {

    Lexer lexer;

    // set filename for bison error reporting
    // switch input stream for lexer to read from file instead of default stdin
    auto inputFilename = make_unique<string>("stdin");

    ifstream fileStream;
    if(!file.empty() && file != "-"sv) {
      if(fileStream.open(file); !fileStream) {
        println(stderr, "error opening file \"{}\"", file);
        return unexpected{-1};
      }
      *inputFilename = file;
      lexer.switch_streams(&fileStream);
    }

    BisonParam bisonParam;
    LexParam lexParam{.loc = location(inputFilename.get())};

    duration<double> yylexSec{};

    EbnfParser parser(
      [&lexer, &yylexSec](LexParam& lexParam) -> EbnfParser::symbol_type {
        time_point<steady_clock> start = steady_clock::now();
        auto token = lexer.yylex(lexParam);
        time_point<steady_clock> end = steady_clock::now();
        yylexSec += end - start;
        return token;
      },
      bisonParam,
      lexParam
    );

    lexer.set_debug(debug);
    parser.set_debug_level(debug);

    if(auto ev = parser(); ev != 0) {
      println(stderr, "parse failed");
      return unexpected{ev};
    }

    return Ebnf{bisonParam.ast};
  }

#if 0
  expected<AstNode, int> convert(AstNode &&) {

  }
#endif

};

}
#endif

