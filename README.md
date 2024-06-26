# ebnfparser
A Bison grammar and parser for the extended BNF (EBNF) syntax used to specify the GQL language in the ISO-39075 standard

This Bison grammar was written to parse the extended BNF notation used to specify the GQL Graph Query Language in the ISO-39075:2024 standard.

The GQL EBNF grammar is briefly described in the XML representation of the grammar at https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.xml. The grammar is available in EBNF form at https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.txt.

The grammar is fully described in Section 5.2 of ISO-39075:2024.

> < >  
> A character string enclosed in angle brackets is the name of a syntactic element (i.e., the name of a BNF non-terminal symbol) of the GQL language.
> 
> ::=  
> The definition operator is used in a production rule to separate the element defined by the rule from its definition. The element being defined appears to the left of the operator and the formula that defines the element > appears to the right.
> 
> [ ]  
> Square brackets indicate optional elements in a formula. The portion of the formula within the brackets may be explicitly specified or may be omitted.
> 
> { }  
> Braces group elements in a formula. The portion of the formula within the braces shall be explicitly specified.
> 
> |  
> The alternative operator. The vertical bar indicates that the portion of the formula following the bar is an alternative to the portion preceding the bar. If the vertical bar appears at a position where it is not enclosed in braces or square brackets, it specifies a complete alternative for the element defined by the production rule. If the vertical bar appears in a portion of a formula enclosed in braces or square brackets, it specifies alternatives for the content of the innermost pair of such braces or brackets.
> 
> ...  
> The ellipsis indicates that the element to which it applies in a formula may be repeated any number of times. If the ellipsis appears immediately after a closing brace “}”, then it applies to the portion of the formula enclosed between that closing brace and the corresponding opening brace “{”. If an ellipsis appears after any other element, then it applies only to that element. In Syntax Rules, General Rules, and Conformance Rules, a reference to the n-th element in such a list assumes the order in which these are specified, unless otherwise stated.
> 
> !!  
> Introduces either a reference to the Syntax Rules, used when the definition of a syntactic element is not expressed in BNF, or the Unicode code point or code point sequence that define the character(s) of the BNF production.

I could not find a BNF or Bison grammar for GQL when I wrote this parser. The parser fully parses the GQL specification in docs/gqlgrammar.txt which is a slightly modified and corrected version of https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.txt. The goal is to use the parser to generate a Bison grammar from GQL EBNF. The generated Bison grammar can then be used to write parsers for graph queries in GQL.

The differences between ISO_IEC_39075(en).bnf.txt and docs/gqlgrammar.txt are

- Literal strings and characters are quoted
- A stray alternative operator in the rule for `<pre-reserved word>` has been deleted
- The definition of `<space>` has been corrected to U+0020 from U+00A0
- A superfluous pair of group brackets has been removed

The EBNF defined for GQL in ISO-39075 is the same as that used for SQL in ISO-9075. As far as I can tell it is unique to these two standards and hasn't been used anywhere else. But its features are typical of other extended BNF syntaxes. It should be straightforward to modify the grammar and parser in this repo to convert other EBNF notations to BNF.

## A Grammar for EBNF

This is the grammar without semantic actions at [`src/ebnfparser.no_actions/grammar/ebnfparser.bison.y`](src/ebnfparser.no_actions/grammar/ebnfparser.bison.y). The same grammar with code to convert EBNF to BNF is in [`src/ebnftobison/grammar`](src/ebnftobison/grammar)

```
ebnf: header rule | header rule rules

rules: RULE_SEPARATOR rule | rules RULE_SEPARATOR rule

rule: NONTERMINAL "::=" production_combo

production_combo: concatenation | alternative | COMMENT

concatenation: production | concatenation production

alternative: production_combo "|" concatenation

production: element | optional | repetition | group

element: NONTERMINAL | TOKEN | LITERAL | NONTERMINAL COMMENT | TOKEN COMMENT

optional: "[" production_combo "]"

repetition: element "..." | group "..." | optional "..."

group: "{" production_combo "}"

header: %empty | header_lines

header_lines: HEADER_LINE | header_lines HEADER_LINE
```

The grammar recognizes `COMMENT` tokens in a few positions that correspond to where comments actually appear in the GQL grammar. If comments don't need to be preserved or transformed, then the COMMENT token can be completely dropped. On the other hand COMMENT tokens can be added to all or some positions as needed or every token can be turned into an object that has an optional comment field.

The GQL grammar has some header lines at the beginning of the file - these are accounted for by the `header` nonterminal.

The `RULE_SEPARATOR` token is not an actual character or string. Rather it's a token returned by the lexer when the start of a new rule is detected.

The two binary operators, `concatenation` and `alternative`, are left-associative by virtue of left-recursive sequence rules.

Also `concatenation` has higher precedence than `alternative` because `alternative` derives from `concatenation` by the rule

```
alternative: production_combo "|" concatenation
```

## Build And Test

The repo has a submodule dependency on GoogleTest that should be initialized with `git submodule update --init --recursive`

Build with `cmake` then `make`

```
cmake -B build -S .

make -C build
```

Run the converter `ebnftobison`
```
build/src/ebnftobison/parser/ebnftobison docs/gqlgrammar.quotedliterals.txt
```

Run unit tests with `ctest`
```
ctest --test-dir build
```

## Source Structure

Source code under [`src/`](src/) is divided into a parser without semantic actions in [`src/ebnfparser.no_actions/`](src/ebnfparser.no_actions/) and a parser that converts EBNF to Bison rules in [`src/ebnftobison/`](src/ebnftobison/). Both directories have Bison and Flex rules files in `grammar/` - source files generated by Bison and Flex are in the corresponding `grammar/` directory in the build tree. Parser tests and standalone parser executables are in `parser/`. The lexer class and tests are in `lexer/`.

The GQL grammar file is in [`docs/`](docs/).

## Note On Using C++ With Flex And Bison

The parser and lexer are written in C++. Both Flex and Bison support C++ very well. Using the Bison C++ skeleton offers tremendous advantages over C. The biggest benefit is one can use C++ objects natively as semantic values - i.e. the data structures used for tokens returned by the lexer and for values filled and passed around as parser rules run - you're no longer limited to pointers to types inside a union. This is enabled with the `%define api.value.type variant` Bison directive. This completely removes the need for memory management and frees your code of `new` and `delete`. The  `%define api.token.constructor` and `%define api.value.automove` settings offer more conveniences for C++.

Since C++ lambdas, I've always defined `yylex()` as a lambda member of the generated parser. This is done by passing `yylex()` as a parameter to the generated parser constructor.

```
%parse-param {function<EbnfParser::symbol_type(location&)> yylex}
```

This is a very powerful technique. Turning `yylex()` into a lambda is a generalization of long-standing conventions of defining `yylex()` as a global free function that wraps the call to the real `yylex()`. But using a lambda is so much easier and more powerful.

With `yylex()` as a lambda, the parser is completely decoupled from the lexer and Flex. You will find no references to the lexer or Flex in parser code generated by Bison. There is no `YY_DECL` macro in the Bison rules file. The lexer is of course needed in application code like tests or the converter executable - it is simply captured by the `yylex()` lambda where the parser is constructed.

```
Lexer lexer;
location loc{};

EbnfParser parser([&lexer](location& loc) -> EbnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  loc
);
```

A `yylex()` lambda lets the application capture anything it needs to essentially customize the parser every time it's used. This is particularly helpful for writing unit tests for the parser.

Flex supports C++ in a different way than Bison. The C++ parser generated by Bison is not designed for inheritance. It doesn't have protected members for example. But Flex provides a base class meant to be extended for each application. Simple grammars like ours for EBNF only need the minimal amount of boilerplate code for a working lexer. The derived lexer class can be enhanced as needed for more complex grammars. And could be passed to the parser for lexical feedback.

## Examples

Nonterminal names are made valid for Bison. Surrounding angle brackets are removed. Any characters not in `[a-zA-Z0-9_]` are replaced with underscores.

Input EBNF
```
<nested query specification> ::=
    <left brace> <query specification> <right brace>
```
Output BNF
```
nested_query_specification:
  left_brace  query_specification  right_brace
```

Input EBNF
```
<binding variable definition> ::=
    <graph variable definition>
  | <binding table variable definition>
  | <value variable definition>
```
Output BNF
```
binding_variable_definition:
  binding_table_variable_definition
|  graph_variable_definition
|  value_variable_definition
```

Each optional expression splits a production into two. One with the expression, the other without

Input EBNF
```
<next statement> ::=
    NEXT [ <yield clause> ] <statement>
```
Output BNF
```
next_statement:
  NEXT  statement
|  NEXT  yield_clause  statement
```

Optional expressions can be nested

Input EBNF
```
<opt typed graph initializer> ::=
    [ [ <typed> ] <graph reference value type> ] <graph initializer>
```
Output BNF
```
opt_typed_graph_initializer:
  graph_initializer
|  graph_reference_value_type  graph_initializer
|  typed  graph_reference_value_type  graph_initializer
```

A nonterminal with repetition is replaced with a new nonterminal with the same name plus a "\_list" suffix. Also a new left-recursive rule is created that generates one or more of the original nonterminal.

Input EBNF
```
<binding variable definition block> ::=
    <binding variable definition>...
```
Output BNF
```
binding_variable_definition_block:
  binding_variable_definition_list

binding_variable_definition_list:
  binding_variable_definition
|  binding_variable_definition_list  binding_variable_definition
```
Here's an example of an optional repetition of a single nonterminal

Input EBNF
```
<statement block> ::=
    <statement> [ <next statement>... ]
```
Output BNF
```
statement_block:
  statement
|  statement  next_statement_list

next_statement_list:
  next_statement
|  next_statement_list  next_statement
```
Groups are expanded by distributing the expression outside the group over the expressions inside the group

Input EBNF
```
<session set command> ::=
    SESSION SET { <session set schema clause>
  | <session set graph clause>
  | <session set time zone clause>
  | <session set parameter clause> }
```
Output BNF
```
session_set_command:
  SESSION  SET  session_set_graph_clause
|  SESSION  SET  session_set_parameter_clause
|  SESSION  SET  session_set_schema_clause
|  SESSION  SET  session_set_time_zone_clause
```

Repetition on a group is different based on whether it's a concatenation group or a group of alternative. Repetition of a concatenation group is transformed similarly to repetition of a symbol. A left-recursive rule is created to derive infinite sequences of the expression. The name of the new nonterminal is formed by joining the names of the elements in the expression with underscores and adding a "\_list" suffix.

Input EBNF
```
<transaction characteristics> ::=
    <transaction mode> [ { <comma> <transaction mode> }... ]
```
Output BNF
```
transaction_characteristics:
  transaction_mode
|  transaction_mode  comma_transaction_mode_list

comma_transaction_mode_list:
  comma  transaction_mode
|  comma_transaction_mode_list  comma  transaction_mode
```
Here's an example of a group with optionals on both sides of an alternative.

Input EBNF
```
<create graph type statement> ::=
    CREATE
         { [ PROPERTY ] GRAPH TYPE [ IF NOT EXISTS ]
  | OR REPLACE [ PROPERTY ] GRAPH TYPE }
         <catalog graph type parent and name> <graph type source>
```
Output BNF
```
create_graph_type_statement:
  CREATE  GRAPH  TYPE  IF  NOT  EXISTS  catalog_graph_type_parent_and_name  graph_type_source
|  CREATE  GRAPH  TYPE  catalog_graph_type_parent_and_name  graph_type_source
|  CREATE  OR  REPLACE  GRAPH  TYPE  catalog_graph_type_parent_and_name  graph_type_source
|  CREATE  OR  REPLACE  PROPERTY  GRAPH  TYPE  catalog_graph_type_parent_and_name  graph_type_source
|  CREATE  PROPERTY  GRAPH  TYPE  IF  NOT  EXISTS  catalog_graph_type_parent_and_name  graph_type_source
|  CREATE  PROPERTY  GRAPH  TYPE  catalog_graph_type_parent_and_name  graph_type_source
```

Repetition on group of alternative generates two new nonterminals and rules. First the group is replaced with a new nonterminal for the group. Then the single new nonterminal is replaced with another new nonterminal for a list.

Input EBNF
```
<separator> ::=
    { <comment>
  | <whitespace> }...
```
Output BNF
```
separator:
  choice_group_0_list

choice_group_0_list:
  choice_group_0
| choice_group_0_list choice_group_0

choice_group_0:
  comment
| whitespace
```

