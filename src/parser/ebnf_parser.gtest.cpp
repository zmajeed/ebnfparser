// ebnf_parser.gtest.cpp

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

#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "lexer/ebnf_lexer.h"
#include "ebnf_parser.bison.h"

using namespace std;

using namespace ::testing;

namespace ebnfparser::testing {

TEST(BisonParser, test_0000) {

  stringstream s("<true literal> ::= TRUE");
  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0001) {

  stringstream s(R"%(<true literal> ::=
  TRUE
| FALSE
| UNKNOWN
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0002) {

  stringstream s("<nested query specification> ::= <left brace> <query specification> <right brace>");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0003) {

  stringstream s(R"%(
<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0004) {

  stringstream s(R"%(
<session activity> ::=
    <session reset command>...
  | <session set command>... [ <session reset command>... ]
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0005) {

  stringstream s(R"%(
<transaction characteristics> ::=
    <transaction mode 1> [ { <comma> <transaction mode 2> }... ]
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0006) {

  stringstream s(R"%(
<create graph statement> ::=
    CREATE { [ PROPERTY ] GRAPH [ IF NOT EXISTS ]
  | OR REPLACE [ PROPERTY ] GRAPH }
         <catalog graph parent and name> { <open graph type>
  | <of graph type> }
         [ <graph source> ]
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0007) {

  stringstream s(R"%(
<delete statement> ::=
    [ DETACH
  | NODETACH ] DELETE <delete item list>
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0008) {

  stringstream s(R"%(
<exists predicate> ::=
    EXISTS { <left brace> <graph pattern> <right brace>
  | <left paren> <graph pattern> <right paren>
  | <left brace> <match statement block> <right brace>
  | <left paren> <match statement block> <right paren>
  | <nested query specification> }
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0009) {

  stringstream s(R"%(
<transaction activity> ::=
    <start transaction command>
         [ <procedure specification> [ <end transaction command> ] ]
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0010) {
  stringstream s(R"%(
<session set command> ::=
    SESSION SET { <session set schema clause>
  | <session set graph clause>
  | <session set time zone clause>
  | <session set parameter clause> }
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0011) {
  stringstream s(R"%(
<single quoted character representation> ::=
    <character representation>
  | <double single quote>!! See the Syntax Rules.
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0012) {
  stringstream s(R"%(
<double single quote> ::=
    <quote> <quote>
    !! See the Syntax Rules.
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0013) {

  stringstream s(R"%(
<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>

<program activity> ::=
    <session activity>
  | <transaction activity>
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0014) {
  stringstream s(R"%(

<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>

<program activity> ::=
    <session activity>
  | <transaction activity>


)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0015) {
  stringstream s(R"%(
************************************************************************************************
This file is a "digital artifact" that contains the grammar specified by ISO/IEC 39075.

This grammar may be used by implementers of GQL-implementations when generating parsers for the 
GQL language.
************************************************************************************************

<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>

<program activity> ::=
    <session activity>
  | <transaction activity>


)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0016) {
  stringstream s(R"%(
           <GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>

      <program activity> ::=
    <session activity>
  | <transaction activity>


)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0017) {
  stringstream s(R"%(
           <GQL-program>
  ::= <program activity> [ <session close command> ]
  | <session close command>

      <program activity>
::=
    <session activity>
|
<transaction activity>


)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0018) {
  stringstream s(R"%(
<implementation-defined access mode> ::=
    !! See the Syntax Rules.

)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0019) {
  stringstream s(R"%(
<implementation-defined access mode> ::=
    !! See the Syntax Rules.

<rollback command> ::=
    ROLLBACK

)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0020) {
  stringstream s(R"%(
<pre-reserved word> ::=
    
    ABSTRACT

  | AGGREGATE

  | AGGREGATES
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0021) {
  stringstream s(R"%(
<space> ::=
    " "
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0022) {

  stringstream s(R"%(
<right brace> ::=
    "}"
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0023) {

  stringstream s(R"%(
<reverse solidus> ::=
    "\\"

<right brace> ::=
    "}"
)%");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_0024) {

  stringstream s("<a> := b");

  Lexer lexer(&s);

  BisonParam bisonParam;
  LexParam lexParam;

  EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
    return lexer.yylex(lexParam);
  },
  bisonParam,
  lexParam);

  EXPECT_NE(parser(), 0);
}

}

