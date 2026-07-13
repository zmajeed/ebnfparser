/* ebnf_lexer.flex.l */

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

/* flex -Cfe -o ebnf_lexer.flex.cpp ebnf_lexer.flex.l */
/* g++ -O0 -Wall -Wextra -Werror -std=c++2b -o ebnflexer ebnf_lexer.flex.cpp */

 // flex directives

 // turn on warnings
%option warn

 // always generate 8bit lexer
%option 8bit

 // C++ lexer class
%option c++

 // custom C++ lexer classname
%option yyclass="Lexer"

 // custom prefix to turn yyFlexLexer into EbnfFlexLexer
%option prefix="Ebnf"

%option debug

 // disable default rule that echoes unmatched input, gives flex scanner jammed error for any unmatched input
%option nodefault

 // noninteractive lexer has better performance, use with -Cfe
%option never-interactive

%option yylineno

%option noyywrap
 // disable various options for interactive lexer
%option nounput noinput batch

 // flex start conditions ie states
%x RULES
%x RULE_START
%x BANG_BANG

%top{
// top of generated .cpp file

#include <stdint.h>
#ifdef _WIN32
#  undef INT8_MIN
#endif

#include <string>

#ifdef _MSC_VER
#pragma warning(push)
// disable vc++ warning C4244, return conversion from streamsize to int possible loss of data, in yyFlexLexer::LexerInput() in .cpp file
#pragma warning(disable: 4244)
#endif

}

%{
// bison generated header with C++ namespace and token definitions
#include "ebnf_parser.bison.h"
#include "lexer/ebnf_lexer.h"

#undef YY_DECL
// dummy parameter because yyFlexLexer::yylex(void) in static flex C++ header yyFlexLexer.h cannot be overridden, only overloaded
// parameter has no name to avoid unused parameter warning
#define YY_DECL ebnfparser::EbnfParser::symbol_type ebnfparser::Lexer::yylex(LexParam& param)

// fix flex error could not convert 0 from int to symbol_type for #define YY_NULL 0
// caused by turning on bison %locations because symbol_type no longer has single int constructor for implicit conversion
#define yyterminate() return symbol_type(YY_NULL, loc)

using namespace std;

using namespace ebnfparser;

// needed for custom yyterminate
using symbol_type = EbnfParser::symbol_type;

%}

%%

 // code appears inside generated yylex function at start

  string headerLine;

 // position in input stream
  auto& loc = param.loc;
 // update current position to previous line and column numbers
  loc.step();

 // flex rules section
 /* only c-style comments starting at second column allowed inside rules section */
 /* first flex rule starts at beginning of line */

 /* ebnf rule begins if nonterminal is first non-whitespace on line */
^([[:space:]]{-}[\n])*"<"[-A-Za-z0-9 _/]+">" {
  BEGIN(RULES);
  loc.columns(yyleng);
  return EbnfParser::make_NONTERMINAL(yytext, loc);
}

 /* everything above first rule is header */
. {
  loc.columns(yyleng);
  headerLine += yytext;
}

 /* accumlate and send header line by line */
\n {
  loc.lines();
  headerLine += yytext;
  return EbnfParser::make_HEADER_LINE(headerLine, loc);
}

 /* patterns for rules body of grammar */
<RULES>{

 /* start of rule marks end of previous rule */
 /* put matched start of rule back to match again after returning rule separator token */
  "<"[-A-Za-z0-9 _/]+">"[[:space:]]*::= {
    BEGIN(RULE_START);
    yyless(0);
    return EbnfParser::make_V_RULE_SEP(loc);
  }

 /* rematch start of rule that was matched earlier */
  <RULE_START>"<"[-A-Za-z0-9 _/]+">" {
    BEGIN(RULES);
    loc.columns(yyleng);
    return EbnfParser::make_NONTERMINAL(yytext, loc);
  }

 /* match rule assignment operator */
  ::= {
    loc.columns(yyleng);
    return EbnfParser::make_COLON_EQUAL(loc);
  }

 /* assumes multiline string literals are not allowed */
  (?x: ["] ( [^"\n] | \\["] | \\\\ )* ["] ) {
    loc.columns(yyleng);
    return EbnfParser::make_LITERAL(yytext, loc);
  }

 /* assumes typical identifier syntax for grammar token */
  [A-Za-z0-9_]+ {
    loc.columns(yyleng);
    return EbnfParser::make_TOKEN(yytext, loc);
  }

 /* alternative operator */
  "|" {
    loc.columns(yyleng);
    return EbnfParser::make_BAR(loc);
  }

 /* start of optional operator */
  "[" {
    loc.columns(yyleng);
    return EbnfParser::make_LEFT_BRACKET(loc);
  }

 /* end of optional operator */
  "]" {
    loc.columns(yyleng);
    return EbnfParser::make_RIGHT_BRACKET(loc);
  }

 /* start of group operator */
  "{" {
    loc.columns(yyleng);
    return EbnfParser::make_LEFT_BRACE(loc);
  }

 /* end of group operator */
  "}" {
    loc.columns(yyleng);
    return EbnfParser::make_RIGHT_BRACE(loc);
  }

 /* repetition operator */
  "..." {
    loc.columns(yyleng);
    return EbnfParser::make_ELLIPSIS(loc);
  }

  "!! " {
    BEGIN(BANG_BANG);
    loc.columns(yyleng);
    return EbnfParser::make_BANG_BANG(loc);
  }

 /* match newlines separately to correctly update line numbers */
  \n {
    loc.lines(yyleng);
  }

 /* whitespace except newline, same as [ \t\v\f\r] but easier to understand */
  ([[:space:]]{-}[\n])+ {
    loc.columns(yyleng);
  }

}

<BANG_BANG>{
  "See the Syntax Rules." {
    BEGIN(RULES);
    loc.columns(yyleng);
    return EbnfParser::make_SEE_SYNTAX(loc);
  }
}

 /* all input must be consumed in every state to prevent flex scanner jammed error */
<*>.|\n {
  loc.columns(yyleng);
  throw EbnfParser::syntax_error(loc, "bad input \""s + yytext + "\""s);
}

%%

#ifdef _MSC_VER
#pragma warning(pop)
#endif

