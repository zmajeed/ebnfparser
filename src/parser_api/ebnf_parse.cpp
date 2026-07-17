// parser_api/ebnf_parse.cpp

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

#include "ebnf_parse.h"

#include <stdlib.h>
#include <getopt.h>

#include <string>
#include <iostream>
#include <memory>
#include <istream>
#include <fstream>
#include <print>

using namespace std;
using namespace ebnfparse;

void usage() {
  println("Usage: ebnfparse [-h | --help] [--debug] [--stats] [file]");
  println("Parses valid EBNF as defined in Section 5.2 of the GQL ISO-39075:2024 standard");
  println("Prints nothing if parse succeeds, otherwise prints an error message with line number");
  println("");
  println("Options:");
  println("--debug: turns on Bison parser and Flex lexer debug traces, off by default");
  println("--stats: print timing stats on successful parse, off by default");
  println("--help | -h: prints usage help");
  println("file: EBNF grammar file, use stdin if no file or file is \"-\"");
}

int main(int argc, char* argv[])
{
  int debug = 0;
  int printStats = 0;
  int printEbnf = 0;
  int convert = 0;

// need filename pointer to stick around for bison error messages that print filename and position
  auto inputFilename = make_unique<string>("stdin");

  option opts[] = {
    {"debug", no_argument, &debug, 1},
    {"print", no_argument, &printEbnf, 1},
    {"stats", no_argument, &printStats, 1},
    {"convert", no_argument, &convert, 1},
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

  if(optind >= argc) {
    usage();
    exit(1);
  }

  ifstream fileStream;

  auto file = argv[optind];
  if(file != "-"sv) {
    if(fileStream.open(file); !fileStream) {
      println(stderr, "error opening file \"{}\"", file);
      exit(1);
    }
  }

  auto ev = EbnfParse::parse(fileStream);

  if(!ev) {
    println(stderr, "parse failed");
    return ev.error();
  }

  Ebnf ebnf{move(ev.value())};

  if(printEbnf == 1) {
    ebnf.print();
  }

  if(convert == 1) {
    auto _ = ebnf.convert();
    Bnf bnf = move(_.value());
    bnf.print();
  }

#if 0
  if(printStats == 1) {
    const auto& [_, __, parseSec] = bisonParam.stats;
    println("parse time: {:.9f} sec", parseSec.count());
    println("lex_time {:.9f} sec", yylexSec.count());
  }
#endif

  return 0;
}

