# A Grammar and Parser for EBNF

This Bison BNF grammar was written to parse the extended BNF notation used to specify the GQL Graph Query Language in the ISO-39075:2024 standard.

```bison
grammar: header | header rules

rules: rule | rules RULE_SEP rule

rule: NONTERMINAL "::=" alternative

alternative: concatenation | alternative "|" concatenation

concatenation: repetition | concatenation repetition

repetition: item | item "..."

item: symbol | optional | group

optional: "[" alternative "]"

group: "{" alternative "}"

symbol: NONTERMINAL | TOKEN | LITERAL

header: %empty | header_lines

header_lines: HEADER_LINE | header_lines HEADER_LINE
```

The complete grammar is at [`src/grammar/ebnf_parser.rules.bison.y`](src/grammar/ebnf_parser.rules.bison.y).

The grammar allows freeform text before the first rule that may be considered a header section of sorts.

Comments in general are not supported elsewhere. But there is an element called `SeeTheRules` that looks like `!! See the Syntax Rules.` - it seems `SeeTheRules` occurs at the end of the RHS of a rule by itself or immediately after the alternatives. It may be thought of as a specific type of comment that has out-of-band information. It's not shown above because it makes the grammar a bit ugly and clunky.

The `RULE_SEP` token is not an actual character or string. Rather it's a virtual token returned by the lexer to signal the end of a rule when needed.

The two binary operators, `concatenation` and `alternative`, are left-associative by virtue of left-recursive sequence rules.

Also `concatenation` has higher precedence than `alternative` because `alternative` derives from `concatenation` by the rule

```
alternative: concatenation | alternative "|" concatenation
```

The three dots `...` repetition operator means repeat 1 or more times, not zero or more times like repetition in other EBNF variants. Use both optional and repetition operators to represent zero or more repetitions.

Here's the EBNF version of the the BNF grammar above - this is really meta and recursive but it's a good short example of the syntax - you just need to keep the literals for the grammar operators like `"..."` and the operators themselves like `...` straight.

```
grammar ::= [ header ] rules

rules ::= rule [ { RULE_SEP rule } ... ]

rule ::= NONTERMINAL "::=" alternative

alternative ::= concatenation [ { "|" concatenation } ... ]

concatenation ::= repetition ...

repetition ::= { symbol | optional | group } [ "..." ]

group ::= "{" alternative "}"

optional ::= "[" alternative "]"

symbol ::= NONTERMINAL | TOKEN | LITERAL

header ::= [ HEADER_LINE ... ]
```

The GQL EBNF notation is briefly described in https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.xml. The grammar is available in EBNF form at https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.txt.

This EBNF variant was first used to define the SQL language for ISO-9075. It's still used in the latest SQL standard ISO-9075:2023. It's described in the XML version of the SQL grammar at https://standards.iso.org/iso-iec/9075/-2/ed-6/en/ISO_IEC_9075-2(E)_Foundation.bnf.xml. The SQL grammar in EBNF format is at https://standards.iso.org/iso-iec/9075/-2/ed-6/en/ISO_IEC_9075-2(E)_Foundation.bnf.txt - but it doesn't describe the EBNF itself.

As far as I can tell this EBNF variant is unique to these two standards and hasn't been used anywhere else. But its features are typical of other extended BNF syntaxes.

The grammar is also described in Section 5.2 of ISO-39075:2024.

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

I could not find a BNF or Bison grammar for GQL when I wrote this parser. The parser fully parses the GQL specification in [`data/gqlgrammar.quotedliterals.txt`](data/gqlgrammar.quotedliterals.txt) which is a slightly modified and corrected version of https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.txt. The goal is to use the parser to generate a Bison grammar from GQL EBNF. The generated Bison grammar can then be used to write parsers for graph queries in GQL.

The differences between ISO_IEC_39075(en).bnf.txt and [`data/gqlgrammar.quotedliterals.txt`](data/gqlgrammar.quotedliterals.txt) are

- Literal strings and characters are quoted
- A stray alternative operator in the rule for `<pre-reserved word>` has been deleted
- The definition of `<space>` has been corrected to U+0020 from U+00A0
- A superfluous pair of group brackets has been removed

## Build And Test

Build with `cmake` and `make`
```
cmake -B build -S .

make -C build
```

Test the parser. It's very fast - it parses nearly 1,000 rules of GQL in 15 milliseconds
```
build/src/parser/ebnfparse --stats data/gqlgrammar.quotedliterals.txt

parse time: 0.015 sec
lex_time 0.002 sec
```

Run the converter `ebnfconvert`
```
build/src/ebnf_convert/ebnfconvert data/gqlgrammar.quotedliterals.txt
```

Run unit tests with `ctest`
```
ctest --test-dir build
```

## Source Structure

Bison and Flex rules files are in `src/grammar` - source files generated by Bison and Flex are in the corresponding `src/grammar` directory in the build tree. Parser tests and a standalone parser executable are in `src/parser`. The lexer class and tests are in `src/lexer`.

The original GQL grammar files are in [`docs/`](docs/).

## Note On Using C++ With Flex And Bison

The parser and lexer are written in C++. Both Flex and Bison support C++ very well. Using the Bison C++ `lalr1.cc` skeleton offers tremendous advantages over C. The biggest benefit is one can use C++ objects natively as semantic values - i.e. the data structures used for tokens returned by the lexer and for values filled and passed around as parser rules run. You're no longer limited to pointers to C++ types inside a union.

This is enabled with the `%define api.value.type variant` Bison directive. It completely removes the need for memory management and frees your code of `new` and `delete`. Even better there's no pointer-chasing  `->` syntax to read or write. Just that simple elimination makes the code immensely more readable and easier to understand.

After lambdas were introduced in C++, I discovered the technique of defining `yylex()` to be a lambda member of the parser. This is done by passing `yylex()` as a parameter to the parser constructor.

```
%parse-param {function<EbnfParser::symbol_type(LexParam&)> yylex}
```

I cannot overstate the power of this technique. Turning `yylex()` into a lambda is a generalization of long-standing hacks of defining `yylex()` as a global free function that wraps the call to the real `yylex()`. But using a lambda is so much easier and more powerful.

With `yylex()` as a lambda, the parser is completely decoupled from the lexer and Flex. You will find no references to the lexer or Flex in parser code generated by Bison. There is no `YY_DECL` macro in the Bison rules file. The lexer is of course needed in application code like tests or the converter executable - it is simply captured by the `yylex()` lambda where the parser is constructed.

```c++
Lexer lexer;
BisonParam bisonParam;
LexParam lexParam;

EbnfParser parser([&lexer](LexParam& lexParam) -> EbnfParser::symbol_type {
  return lexer.yylex(lexParam);
},
bisonParam,
lexParam);
```

A `yylex()` lambda lets the application capture anything it needs to essentially customize the parser every time it's used. This is particularly helpful for writing unit tests for the parser. Or for preprocessing right before entering `yylex`. Or to postprocess the token returned by `yylex` before it's handed to the parser.

The improvements to `variant`, `visit` and recursive lambdas with deducing this in C++26 have simplified the design of abstract syntax trees for parsers. It's now trivial to design extremely basic POD data structures to hold semantic values during parsing. These data structures have obvious 1-1 correspondences to grammar rules.

Here are the data structures for the EBNF grammar above

```c++
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
```
And how their types are used in Bison

```bison
%token <string> NONTERMINAL
%token <string> TOKEN
%token <string> LITERAL
%token <string> HEADER_LINE

%nterm <Grammar> grammar
%nterm <Header> header
%nterm <Rule> rule
%nterm <Alternative> alternative
%nterm <Optional> optional
%nterm <Group> group
%nterm <Concatenation> concatenation
%nterm <Repetition> repetition
%nterm <Item> item

%nterm <string> symbol

%nterm <vector<string>> header_lines
%nterm <vector<Rule>> rules
%nterm <vector<Alternative>> rhs
```

Each Bison symbol and token maps to its obvious value type. The only exception is the EBNF `symbol` element which maps to `string` instead of `Symbol` due to a Bison limitation on mixing typedefs and underlying types. We choose to use `string` instead of `Symbol` because it's a basic and commonly needed type. We could eliminate the conflict by making `Symbol` a first-class type instead of a typedef - but it seems an unnecessary complication.

Another huge benefit of POD structures for senantic values is it separates the need for simple data containers at parse time while the AST is being constructed from complex transformations that need to traverse the fully-constructed AST later.

Transformations that traverse the AST are done through a `variant` for all the nodes of the AST

```c++
struct AstNode: variant<Grammar, Header, Rule, Alternative, Concatenation, Repetition,, Group, Optional, Item, Symbol>

```

that offers each transformation as a `visit` of all nodes in the AST


```c++
void printAst() const {

  visit(overload{

    [](this auto&& self, const Grammar& g) -> void {
      println("grammar:");
      self(g.header);
      println("{} rules:", g.rules.size());
      for(auto& rule: g.rules) {
        self(rule);
      }
    },

    [](this auto&& self, const Rule& r) -> void {
      println("rule:");
      println("{}:", r.nonterminal);
      for(auto& alt: r.alternatives) {
        self(alt);
      }
    },

    [](this auto&& self, const Alternative& a) -> void {
      println("alternative:");
      for(auto& concat: a.concats) {
        self(concat);
      }
    },

    [](this auto&& self, const Group& g) -> void {
      println("group:");
      for(auto& concat: g.concats) {
        self(concat);
      }
    },

    [](this auto&& self, const Optional& o) -> void {
      println("optional:");
      for(auto& concat: o.concats) {
        self(concat);
      }
    },

    [](this auto&& self, const Concatenation& c) -> void {
      println("concatenation:");
      for(auto& rep: c.reps) {
        self(rep);
      }
    },

    [](this auto&& self, const Repetition& r) -> void {
      println("repetition:");
      r.item.visit(self);
    },

    [](this auto&& self, const Item& i) -> void {
      println("item:");
      i.visit(self);
    },

    [](this auto&&, const Symbol& s) -> void {
      println("symbol: {}", s);
    },

    [](this auto&&, const Header& h) -> void {
      println("header:");
      for(auto& line: h.lines) {
        print("{}", line);
      }
    },

    [](this auto&&, const auto&) -> void {
      println("default visit");
    },

  });
}

```

This way the data structures for each node remain simple and uncluttered yet still allow arbitrary operations on them to be independently defined.

Flex supports C++ in a different way than Bison. The C++ parser generated by Bison is not designed for inheritance. It doesn't have protected members for example. But Flex provides a base class meant to be extended for each application. Simple grammars like ours for EBNF only need the minimal amount of boilerplate code for a working lexer. The derived lexer class can be enhanced as needed for more complex grammars. And could be passed to the parser for lexical feedback.

## Converting EBNF to BNF

EBNF has 3 features that extend BNF syntax - group, optional, repetition. These 3 features can compress multiple BNF rules into a single rule.

### Group

Group factors out common elements from multiple choices leaving distinct elements in a group of multiple choices - the common elements are placed before or after the group depending on their original relative positions in the different choices

#### BNF

r: A B | A C | A D |
s: X Z | Y Z

#### EBNF

r ::= A { B | C | D }
s ::= { X | Y } Z

### Optional

#### BNF

r: A | A B
s: t C
t: %empty | T

#### EBNF

r ::= A [ B ]
s ::= [ T ] C


### Repetition

#### BNF

r: A | r A
s: B | s "," B

#### EBNF

r ::= A ...
s ::= B { "," B } ...

Thus converting EBNF to BNF means a single rule can turn into multiple rules.

The result of parsing EBNF is an EBNF abstract syntax tree. Each node of the AST is a specific type of AstNode described above.

The `ebnf_convert` program is an application of the AST. Its `convert` function is a simple tree-walker that returns a BnfNode for each AstNode it visits - transforming in a sense each EBNF rule to a BNF rule. But as noted above, sometimes an EBNF rule is the equivalent of more than one BNF rule.

The set of equivalent BNF rules fall into two categories. A new BNF rule could be just a new production for the original EBNF nonterminal being processed - like for optional. In this case the new BNF rules can be returned as a set of choices to transform and add to the original EBNF rule. The other possibility is that the new BNF rule is for a new synthesized nonterminal - like for Repetition. In this case the new BNF rule should be collected without further transformation.

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

