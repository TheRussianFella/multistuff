%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2.3.2-5cce"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  # include <vector>
  # include <iostream>
  # include "pipepart.hpp"
  class Driver;
  class PipePart;
}

// The parsing context.
%param { Driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
  # include "driver.hh"
  # include "pipepart.hpp"
}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  PIPESEP "|"
  WRITETO ">"
  WRITEFROM "<"
;

%type  <PipePart> command;
%type  <std::vector<PipePart>> list;
%token <std::string> COMMAND_PART;

%%
%start result;

result:
  list              { drv.result = $1; }
;

list:
  %empty            { /* Generates an empty string list */ }
| list "|" command  { $$ = $1; $3.init(); $$.push_back ($3); }
| list command      { $$ = $1; $2.init(); $$.push_back ($2); }
;

command:
  %empty                   {}
| command ">" COMMAND_PART { $$ = $1; $$.output_file = std::string($3); }
| command "<" COMMAND_PART { $$ = $1; $$.input_file  = std::string($3); }
| command COMMAND_PART     { $$ = $1; $$.arguments.push_back(std::string($2)); }
;
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
