// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 166 "./src/grammar/ebnf_parser.bison.y"

// %{ unnamed codeblock goes at very top of .cpp file before namespace and parser class

#include <sstream>


#line 48 "./src/flexbison.gen/ebnf_parser.bison.cpp"


#include "ebnf_parser.bison.h"


// Unqualified %code blocks.
#line 173 "./src/grammar/ebnf_parser.bison.y"

// %code codeblock goes at top of .cpp outside namespace and parser class

#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <chrono>
#include <print>
#include <ranges>

#include "ast/ebnf_ast.h"

using namespace std;
using views::as_rvalue;

namespace {
  const auto defaultInputName = "inputstream"s;
}

void ebnfparser::EbnfParser::error(const location& loc, const string& msg) {
  println("error at {}: {}", loc, msg);
}


#line 83 "./src/flexbison.gen/ebnf_parser.bison.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 121 "./src/grammar/ebnf_parser.bison.y"
namespace ebnfparser {
#line 176 "./src/flexbison.gen/ebnf_parser.bison.cpp"

  /// Build a parser object.
  EbnfParser::EbnfParser (function<EbnfParser::symbol_type(LexParam&)> yylex_yyarg, BisonParam& bisonParam_yyarg, LexParam& lexParam_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yylex (yylex_yyarg),
      bisonParam (bisonParam_yyarg),
      lexParam (lexParam_yyarg)
  {}

  EbnfParser::~EbnfParser ()
  {}

  EbnfParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  EbnfParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  EbnfParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  EbnfParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  EbnfParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  EbnfParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  EbnfParser::symbol_kind_type
  EbnfParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  EbnfParser::stack_symbol_type::stack_symbol_type ()
  {}

  EbnfParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_alternative: // alternative
        value.YY_MOVE_OR_COPY< Alternative > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_concatenation: // concatenation
        value.YY_MOVE_OR_COPY< Concatenation > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_grammar: // grammar
        value.YY_MOVE_OR_COPY< Grammar > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_group: // group
        value.YY_MOVE_OR_COPY< Group > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_header: // header
        value.YY_MOVE_OR_COPY< Header > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_item: // item
        value.YY_MOVE_OR_COPY< Item > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional: // optional
        value.YY_MOVE_OR_COPY< Optional > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repetition: // repetition
        value.YY_MOVE_OR_COPY< Repetition > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_rule: // rule
        value.YY_MOVE_OR_COPY< Rule > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NONTERMINAL: // NONTERMINAL
      case symbol_kind::S_TOKEN: // TOKEN
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_HEADER_LINE: // HEADER_LINE
      case symbol_kind::S_symbol: // symbol
        value.YY_MOVE_OR_COPY< string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_rhs: // rhs
        value.YY_MOVE_OR_COPY< vector<Alternative> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_rules: // rules
        value.YY_MOVE_OR_COPY< vector<Rule> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_header_lines: // header_lines
        value.YY_MOVE_OR_COPY< vector<string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  EbnfParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_alternative: // alternative
        value.move< Alternative > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_concatenation: // concatenation
        value.move< Concatenation > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_grammar: // grammar
        value.move< Grammar > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_group: // group
        value.move< Group > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_header: // header
        value.move< Header > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_item: // item
        value.move< Item > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional: // optional
        value.move< Optional > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repetition: // repetition
        value.move< Repetition > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_rule: // rule
        value.move< Rule > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NONTERMINAL: // NONTERMINAL
      case symbol_kind::S_TOKEN: // TOKEN
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_HEADER_LINE: // HEADER_LINE
      case symbol_kind::S_symbol: // symbol
        value.move< string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_rhs: // rhs
        value.move< vector<Alternative> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_rules: // rules
        value.move< vector<Rule> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_header_lines: // header_lines
        value.move< vector<string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  EbnfParser::stack_symbol_type&
  EbnfParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_alternative: // alternative
        value.copy< Alternative > (that.value);
        break;

      case symbol_kind::S_concatenation: // concatenation
        value.copy< Concatenation > (that.value);
        break;

      case symbol_kind::S_grammar: // grammar
        value.copy< Grammar > (that.value);
        break;

      case symbol_kind::S_group: // group
        value.copy< Group > (that.value);
        break;

      case symbol_kind::S_header: // header
        value.copy< Header > (that.value);
        break;

      case symbol_kind::S_item: // item
        value.copy< Item > (that.value);
        break;

      case symbol_kind::S_optional: // optional
        value.copy< Optional > (that.value);
        break;

      case symbol_kind::S_repetition: // repetition
        value.copy< Repetition > (that.value);
        break;

      case symbol_kind::S_rule: // rule
        value.copy< Rule > (that.value);
        break;

      case symbol_kind::S_NONTERMINAL: // NONTERMINAL
      case symbol_kind::S_TOKEN: // TOKEN
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_HEADER_LINE: // HEADER_LINE
      case symbol_kind::S_symbol: // symbol
        value.copy< string > (that.value);
        break;

      case symbol_kind::S_rhs: // rhs
        value.copy< vector<Alternative> > (that.value);
        break;

      case symbol_kind::S_rules: // rules
        value.copy< vector<Rule> > (that.value);
        break;

      case symbol_kind::S_header_lines: // header_lines
        value.copy< vector<string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  EbnfParser::stack_symbol_type&
  EbnfParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_alternative: // alternative
        value.move< Alternative > (that.value);
        break;

      case symbol_kind::S_concatenation: // concatenation
        value.move< Concatenation > (that.value);
        break;

      case symbol_kind::S_grammar: // grammar
        value.move< Grammar > (that.value);
        break;

      case symbol_kind::S_group: // group
        value.move< Group > (that.value);
        break;

      case symbol_kind::S_header: // header
        value.move< Header > (that.value);
        break;

      case symbol_kind::S_item: // item
        value.move< Item > (that.value);
        break;

      case symbol_kind::S_optional: // optional
        value.move< Optional > (that.value);
        break;

      case symbol_kind::S_repetition: // repetition
        value.move< Repetition > (that.value);
        break;

      case symbol_kind::S_rule: // rule
        value.move< Rule > (that.value);
        break;

      case symbol_kind::S_NONTERMINAL: // NONTERMINAL
      case symbol_kind::S_TOKEN: // TOKEN
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_HEADER_LINE: // HEADER_LINE
      case symbol_kind::S_symbol: // symbol
        value.move< string > (that.value);
        break;

      case symbol_kind::S_rhs: // rhs
        value.move< vector<Alternative> > (that.value);
        break;

      case symbol_kind::S_rules: // rules
        value.move< vector<Rule> > (that.value);
        break;

      case symbol_kind::S_header_lines: // header_lines
        value.move< vector<string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  EbnfParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  EbnfParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  EbnfParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  EbnfParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  EbnfParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  EbnfParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  EbnfParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  EbnfParser::debug_level_type
  EbnfParser::debug_level () const
  {
    return yydebug_;
  }

  void
  EbnfParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  EbnfParser::state_type
  EbnfParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  EbnfParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  EbnfParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  EbnfParser::operator() ()
  {
    return parse ();
  }

  int
  EbnfParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 201 "./src/grammar/ebnf_parser.bison.y"
{
// %initial-action codeblock goes inside parse() function in .cpp, it's a separate brace-scoped block, anything declared here is local to this block and cannot be used anywhere else in parse()

// suppress unused variable warning till fix in release after version 3.8.2, https://github.com/akimd/bison/commit/a166d5450e3f47587b98f6005f9f5627dbe21a5b
// yynerrs_ is local to generated parse function
  (void)yynerrs_;

  bisonParam.stats.parseStartTime = steady_clock::now();

  auto& loc = lexParam.loc;

  if(loc.begin.filename == nullptr) {
    loc.initialize(&defaultInputName);
  }
}

#line 679 "./src/flexbison.gen/ebnf_parser.bison.cpp"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (lexParam));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_alternative: // alternative
        yylhs.value.emplace< Alternative > ();
        break;

      case symbol_kind::S_concatenation: // concatenation
        yylhs.value.emplace< Concatenation > ();
        break;

      case symbol_kind::S_grammar: // grammar
        yylhs.value.emplace< Grammar > ();
        break;

      case symbol_kind::S_group: // group
        yylhs.value.emplace< Group > ();
        break;

      case symbol_kind::S_header: // header
        yylhs.value.emplace< Header > ();
        break;

      case symbol_kind::S_item: // item
        yylhs.value.emplace< Item > ();
        break;

      case symbol_kind::S_optional: // optional
        yylhs.value.emplace< Optional > ();
        break;

      case symbol_kind::S_repetition: // repetition
        yylhs.value.emplace< Repetition > ();
        break;

      case symbol_kind::S_rule: // rule
        yylhs.value.emplace< Rule > ();
        break;

      case symbol_kind::S_NONTERMINAL: // NONTERMINAL
      case symbol_kind::S_TOKEN: // TOKEN
      case symbol_kind::S_LITERAL: // LITERAL
      case symbol_kind::S_HEADER_LINE: // HEADER_LINE
      case symbol_kind::S_symbol: // symbol
        yylhs.value.emplace< string > ();
        break;

      case symbol_kind::S_rhs: // rhs
        yylhs.value.emplace< vector<Alternative> > ();
        break;

      case symbol_kind::S_rules: // rules
        yylhs.value.emplace< vector<Rule> > ();
        break;

      case symbol_kind::S_header_lines: // header_lines
        yylhs.value.emplace< vector<string> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // grammar: header postprocess
#line 257 "./src/grammar/ebnf_parser.bison.y"
                            {
  yylhs.value.as < Grammar > ().header = move(yystack_[1].value.as < Header > ());
  bisonParam.ast = move(yylhs.value.as < Grammar > ());
}
#line 876 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 3: // grammar: header rules postprocess
#line 261 "./src/grammar/ebnf_parser.bison.y"
                           {
  yylhs.value.as < Grammar > ().header = move(yystack_[2].value.as < Header > ());
  yylhs.value.as < Grammar > ().rules = move(yystack_[1].value.as < vector<Rule> > ());
  bisonParam.ast = move(yylhs.value.as < Grammar > ());
}
#line 886 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 4: // rules: rule
#line 267 "./src/grammar/ebnf_parser.bison.y"
            {
  if(yystack_[0].value.as < Rule > ().nonterminal.empty()) {
    break;
  }
  yylhs.value.as < vector<Rule> > ().push_back(move(yystack_[0].value.as < Rule > ()));
}
#line 897 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 5: // rules: rules V_RULE_SEP rule
#line 273 "./src/grammar/ebnf_parser.bison.y"
                        {
  if(yystack_[0].value.as < Rule > ().nonterminal.empty()) {
    break;
  }
  yylhs.value.as < vector<Rule> > () = move(yystack_[2].value.as < vector<Rule> > ());
  yylhs.value.as < vector<Rule> > ().push_back(move(yystack_[0].value.as < Rule > ()));
}
#line 909 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 6: // rule: NONTERMINAL "::=" rhs
#line 281 "./src/grammar/ebnf_parser.bison.y"
                            {
  yylhs.value.as < Rule > ().nonterminal = move(yystack_[2].value.as < string > ());
  yylhs.value.as < Rule > ().alts.insert_range(as_rvalue(yystack_[0].value.as < vector<Alternative> > ()));
}
#line 918 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 7: // rhs: alternative
#line 286 "./src/grammar/ebnf_parser.bison.y"
                 {
  yylhs.value.as < vector<Alternative> > ().push_back(yystack_[0].value.as < Alternative > ());
}
#line 926 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 8: // rhs: see_syntax_rules
#line 289 "./src/grammar/ebnf_parser.bison.y"
                   {
}
#line 933 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 9: // rhs: alternative see_syntax_rules
#line 291 "./src/grammar/ebnf_parser.bison.y"
                               {
  yylhs.value.as < vector<Alternative> > ().push_back(yystack_[1].value.as < Alternative > ());
}
#line 941 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 10: // alternative: concatenation
#line 295 "./src/grammar/ebnf_parser.bison.y"
                           {
  yylhs.value.as < Alternative > ().concats.emplace(yystack_[0].value.as < Concatenation > ());
}
#line 949 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 11: // alternative: alternative "|" concatenation
#line 298 "./src/grammar/ebnf_parser.bison.y"
                                {
  yylhs.value.as < Alternative > () = move(yystack_[2].value.as < Alternative > ());
  yylhs.value.as < Alternative > ().concats.emplace(yystack_[0].value.as < Concatenation > ());
}
#line 958 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 12: // concatenation: repetition
#line 303 "./src/grammar/ebnf_parser.bison.y"
                          {
  yylhs.value.as < Concatenation > ().reps.push_back(move(yystack_[0].value.as < Repetition > ()));
}
#line 966 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 13: // concatenation: concatenation repetition
#line 306 "./src/grammar/ebnf_parser.bison.y"
                           {
  yylhs.value.as < Concatenation > () = move(yystack_[1].value.as < Concatenation > ());
  yylhs.value.as < Concatenation > ().reps.push_back(move(yystack_[0].value.as < Repetition > ()));
}
#line 975 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 14: // repetition: item
#line 311 "./src/grammar/ebnf_parser.bison.y"
                 {
  yylhs.value.as < Repetition > ().item = move(yystack_[0].value.as < Item > ());
  yylhs.value.as < Repetition > ().isRepeated = false;
}
#line 984 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 15: // repetition: item "..."
#line 315 "./src/grammar/ebnf_parser.bison.y"
             {
  yylhs.value.as < Repetition > ().item = move(yystack_[1].value.as < Item > ());
  yylhs.value.as < Repetition > ().isRepeated = true;
}
#line 993 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 16: // item: symbol
#line 320 "./src/grammar/ebnf_parser.bison.y"
             {
  yylhs.value.as < Item > () = move(yystack_[0].value.as < string > ());
}
#line 1001 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 17: // item: optional
#line 323 "./src/grammar/ebnf_parser.bison.y"
           {
  yylhs.value.as < Item > () = move(yystack_[0].value.as < Optional > ());
}
#line 1009 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 18: // item: group
#line 326 "./src/grammar/ebnf_parser.bison.y"
        {
  yylhs.value.as < Item > () = move(yystack_[0].value.as < Group > ());
}
#line 1017 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 19: // optional: "[" alternative "]"
#line 330 "./src/grammar/ebnf_parser.bison.y"
                              {
  yylhs.value.as < Optional > ().concats = move(yystack_[1].value.as < Alternative > ().concats);
}
#line 1025 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 20: // group: "{" alternative "}"
#line 334 "./src/grammar/ebnf_parser.bison.y"
                           {
  yylhs.value.as < Group > ().concats = move(yystack_[1].value.as < Alternative > ().concats);
}
#line 1033 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 21: // symbol: NONTERMINAL
#line 338 "./src/grammar/ebnf_parser.bison.y"
                    {
  yylhs.value.as < string > () = move(yystack_[0].value.as < string > ());
}
#line 1041 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 22: // symbol: TOKEN
#line 341 "./src/grammar/ebnf_parser.bison.y"
        {
  yylhs.value.as < string > () = move(yystack_[0].value.as < string > ());
}
#line 1049 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 23: // symbol: LITERAL
#line 344 "./src/grammar/ebnf_parser.bison.y"
          {
  yylhs.value.as < string > () = move(yystack_[0].value.as < string > ());
}
#line 1057 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 24: // header: %empty
#line 348 "./src/grammar/ebnf_parser.bison.y"
               {
}
#line 1064 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 25: // header: header_lines
#line 350 "./src/grammar/ebnf_parser.bison.y"
               {
  yylhs.value.as < Header > ().lines = move(yystack_[0].value.as < vector<string> > ());
}
#line 1072 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 26: // header_lines: HEADER_LINE
#line 354 "./src/grammar/ebnf_parser.bison.y"
                          {
  yylhs.value.as < vector<string> > ().push_back(move(yystack_[0].value.as < string > ()));
}
#line 1080 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 27: // header_lines: header_lines HEADER_LINE
#line 357 "./src/grammar/ebnf_parser.bison.y"
                           {
  yylhs.value.as < vector<string> > () = move(yystack_[1].value.as < vector<string> > ());
  yylhs.value.as < vector<string> > ().push_back(move(yystack_[0].value.as < string > ()));
}
#line 1089 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;

  case 29: // postprocess: %empty
#line 365 "./src/grammar/ebnf_parser.bison.y"
                    {
  auto& [start, end, elapsed] = bisonParam.stats;
  end = steady_clock::now();
  elapsed = end - start;
}
#line 1099 "./src/flexbison.gen/ebnf_parser.bison.cpp"
    break;


#line 1103 "./src/flexbison.gen/ebnf_parser.bison.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  EbnfParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  EbnfParser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "::=", "[", "]", "{", "}",
  "...", "|", "!!", "see syntax rules", "V_RULE_SEP", "NONTERMINAL",
  "TOKEN", "LITERAL", "HEADER_LINE", "$accept", "grammar", "rules", "rule",
  "rhs", "alternative", "concatenation", "repetition", "item", "optional",
  "group", "symbol", "header", "header_lines", "see_syntax_rules",
  "postprocess", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // EbnfParser::context.
  EbnfParser::context::context (const EbnfParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  EbnfParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  EbnfParser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  EbnfParser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char EbnfParser::yypact_ninf_ = -23;

  const signed char EbnfParser::yytable_ninf_ = -1;

  const signed char
  EbnfParser::yypact_[] =
  {
     -14,   -23,     5,    13,     2,   -23,    21,    15,   -23,   -23,
     -23,    -3,    13,   -23,     0,     0,    14,   -23,   -23,   -23,
     -23,    -1,     0,   -23,    20,   -23,   -23,   -23,   -23,   -23,
      11,    12,   -23,     0,   -23,   -23,   -23,   -23,   -23,     0
  };

  const signed char
  EbnfParser::yydefact_[] =
  {
      24,    26,     0,    29,    25,     1,     0,    29,     4,     2,
      27,     0,     0,     3,     0,     0,     0,    21,    22,    23,
       6,     7,    10,    12,    14,    17,    18,    16,     8,     5,
       0,     0,    28,     0,     9,    13,    15,    19,    20,    11
  };

  const signed char
  EbnfParser::yypgoto_[] =
  {
     -23,   -23,   -23,    17,   -23,     8,    -2,   -22,   -23,   -23,
     -23,   -23,   -23,   -23,     9,    25
  };

  const signed char
  EbnfParser::yydefgoto_[] =
  {
       0,     2,     7,     8,    20,    21,    22,    23,    24,    25,
      26,    27,     3,     4,    28,     9
  };

  const signed char
  EbnfParser::yytable_[] =
  {
      35,    14,     1,    15,    14,     5,    15,    16,    33,    16,
      17,    18,    19,    17,    18,    19,    37,    35,    10,    38,
      33,    33,    30,    31,    11,    32,     6,    12,    36,    29,
      34,    39,    13
  };

  const signed char
  EbnfParser::yycheck_[] =
  {
      22,     4,    16,     6,     4,     0,     6,    10,     9,    10,
      13,    14,    15,    13,    14,    15,     5,    39,    16,     7,
       9,     9,    14,    15,     3,    11,    13,    12,     8,    12,
      21,    33,     7
  };

  const signed char
  EbnfParser::yystos_[] =
  {
       0,    16,    18,    29,    30,     0,    13,    19,    20,    32,
      16,     3,    12,    32,     4,     6,    10,    13,    14,    15,
      21,    22,    23,    24,    25,    26,    27,    28,    31,    20,
      22,    22,    11,     9,    31,    24,     8,     5,     7,    23
  };

  const signed char
  EbnfParser::yyr1_[] =
  {
       0,    17,    18,    18,    19,    19,    20,    21,    21,    21,
      22,    22,    23,    23,    24,    24,    25,    25,    25,    26,
      27,    28,    28,    28,    29,    29,    30,    30,    31,    32
  };

  const signed char
  EbnfParser::yyr2_[] =
  {
       0,     2,     2,     3,     1,     3,     3,     1,     1,     2,
       1,     3,     1,     2,     1,     2,     1,     1,     1,     3,
       3,     1,     1,     1,     0,     1,     1,     2,     2,     0
  };




#if YYDEBUG
  const short
  EbnfParser::yyrline_[] =
  {
       0,   257,   257,   261,   267,   273,   281,   286,   289,   291,
     295,   298,   303,   306,   311,   315,   320,   323,   326,   330,
     334,   338,   341,   344,   348,   350,   354,   357,   362,   365
  };

  void
  EbnfParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  EbnfParser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 121 "./src/grammar/ebnf_parser.bison.y"
} // ebnfparser
#line 1545 "./src/flexbison.gen/ebnf_parser.bison.cpp"

#line 371 "./src/grammar/ebnf_parser.bison.y"


