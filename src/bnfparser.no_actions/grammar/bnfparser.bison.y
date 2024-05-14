// bnfparser.bison.y

/*
MIT License

Copyright (c) 2024 Zartaj Majeed

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

// bison -Wall -Wdangling-alias -Werror --color=always -Wcounterexamples --report=counterexamples,lookaheads --report-file=bisonreport.txt bnfparser.bison.y

// c++ parser classes skeleton file
%skeleton "lalr1.cc"

%require "3.8"

// c++ parser
%language "c++"

// generate parser description report .output file
%verbose

%defines "bnfparser.bison.h"
%output "bnfparser.bison.cpp"

// enable debug trace
%define parse.trace

// adds location parameter to symbol constructors
%locations

// generate header for location to be used outside of bison parser
%define api.location.file "locations.bison.h"

%define parse.error detailed

// want to return character token from flex but don't think it can work with api.token.constructor
// better lookup performance at cost of disallowing literal characters in grammar
%define api.token.raw

%code requires {
// %code requires codeblock goes at top of .h outside of namespace and parser class
// standard c++ #includes and defines

#include <string>
#include <functional>
#include <chrono>

#include "locations.bison.h"

namespace bnfparser {

using namespace std;
using namespace chrono;

struct BisonParam {
  duration<double> parseTimeTakenSec;
  time_point<steady_clock> parseStartTime;
  time_point<steady_clock> parseEndTime;
};

}

}

// custom namespace generated by bison
%define api.namespace {bnfparser}

// custom classname generated by bison
%define api.parser.class {BnfParser}

// parser constructor parameter 1
// this is the yylex bison will call, it can have any signature we want since it's a lambda passed in by the client that wraps and hides the actual yylex call
%parse-param {function<BnfParser::symbol_type(location&)> yylex}

%parse-param {BisonParam& bisonParam}

// parser constructor parameter 2
// only because each lex-param also must be parse-param
%parse-param {location& loc}

// yylex parameter 1
// pass location to lexer to update
%lex-param {location& loc}

// use actual types for tokens
%define api.value.type variant

// use c++ objects, changes signature of yylex, yylex now returns PGNParser::symbol_type, yylex takes no parameters, change everywhere yylex is referenced, eg parse-param, any lambdas
%define api.token.constructor

%define parse.assert

%code provides {
// %code provides codeblock goes in .h after namespace and parser class
// everything here needs BnfParser defined earlier

// parser objects
namespace bnfparser {

using namespace std;

}

}

%{
// %{ unnamed codeblock goes at very top of .cpp file before namespace and parser class

#include <sstream>

%}

%code {
// %code codeblock goes at top of .cpp outside namespace and parser class

#include <chrono>

using namespace std;

namespace {
  constexpr auto defaultInputName = "inputstream"s;
}

void bnfparser::BnfParser::error(const location& loc, const string& msg) {
  cerr << "error at " << loc << ": " << msg << "\n";
}

}

%initial-action {
// %initial-action codeblock goes inside parse() function in .cpp, it's a separate brace-scoped block, anything declared here is local to this block and cannot be used anywhere else in parse()

  bisonParam.parseStartTime = steady_clock::now();

  if(loc.begin.filename == nullptr) {
    loc.initialize(&defaultInputName);
  }
}

%token COLON_EQUAL          "::="
%token LEFT_BRACKET         "["
%token RIGHT_BRACKET        "]"
%token LEFT_BRACE           "{"
%token RIGHT_BRACE          "}"
%token ELLIPSIS             "..."
%token BAR                  "|"

%token WHITESPACE
%token RULE_SEPARATOR

%token <string> NONTERMINAL
%token <string> TOKEN
%token <string> LITERAL
%token <string> COMMENT
%token <string> HEADER_LINE

%start ebnf

%%

// no code allowed in rules section, just bison comments that are dropped from .cpp

ebnf: header rule postprocess | header rule rules postprocess

rules: RULE_SEPARATOR rule | rules RULE_SEPARATOR rule

rule: NONTERMINAL "::=" production_combos

production_combos: concatenation | alternative | COMMENT

production: element | optional | repetition | group

element: NONTERMINAL | TOKEN | LITERAL | NONTERMINAL COMMENT | TOKEN COMMENT

concatenation: production | concatenation production

alternative: production_combos "|" concatenation

optional: "[" production_combos "]"

repetition: element "..." | group "..." | optional "..."

group: "{" production_combos "}"

header: %empty | header_lines

header_lines: HEADER_LINE | header_lines HEADER_LINE

// midrule action for postprocessing
postprocess: %empty {
  auto& b = bisonParam;
  b.parseEndTime = steady_clock::now();
  b.parseTimeTakenSec = b.parseEndTime - b.parseStartTime;
}

%%

#ifdef BUILD_MAIN

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include <string>
#include <iostream>
#include <memory>
#include <istream>
#include <fstream>

#include "lexer/bnfparser_lexer.h"
#include "bnfparser.bison.h"

using namespace std;
using namespace bnfparser;

void usage() {
  puts("Usage: bnfparse [-h | --help] [--debug] [file]");
  puts("bnfparse checks if file is valid extended BNF as defined in Section 5.2 of the GQL ISO-39075:2024 standard");
  puts("It prints nothing if the parse succeeds and the file is valid, otherwise it prints an error message with line numbers");
  puts("");
  puts("Options:");
  puts("--debug: turns on Bison parser and Flex lexer debug traces, off by default");
  puts("--stats: print timing stats on successful parse, off by default");
  puts("--help | -h: prints usage help");
  puts("file: extended BNF grammar file");
}

int main(int argc, char* argv[])
{
  ios_base::sync_with_stdio(false);

  bool debug{};
  bool printStats{};

// need filename pointer to stick around for bison error messages that print filename and position
  auto inputFilename = make_unique<string>("stdin");
  string changefile;

  option opts[] = {
    {"debug", no_argument, (int*)&debug, 1},
    {"stats", no_argument, (int*)&printStats, 1},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  for(int i, optLetter; (optLetter = getopt_long(argc, argv, "h", opts, &i)) != -1;) {
    switch(optLetter) {
    case 0:
      break;
    case 'h':
      usage();
      return 0;
    case '?':
      usage();
      return 1;
    default:
      break;
    }
  }

  Lexer lexer;

// set filename for bison error reporting
// switch input stream for lexer to read from file instead of default stdin
  ifstream fileStream;
  if(optind < argc) {
    *inputFilename = argv[optind];
    if(fileStream.open(*inputFilename); !fileStream) {
      fprintf(stderr, "error opening file \"%s\"\n", inputFilename->c_str());
      exit(1);
    }
    lexer.switch_streams(&fileStream);
  }

  location loc(inputFilename.get());
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  lexer.set_debug(debug);
  parser.set_debug_level(debug);

  if(auto ev = parser(); ev != 0) {
    fputs("parse failed\n", stderr);
    return ev;
  }

  if(printStats) {
    printf("parse time: %.9f secs\n", bisonParam.parseTimeTakenSec.count());
  }

  return 0;
}

#endif
