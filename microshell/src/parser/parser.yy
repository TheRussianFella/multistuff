%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2.3.2-5cce"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  # include <iostream>
  class Driver;
}

// The parsing context.
%param { Driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
  # include "driver.hh"
}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  PIPESEP "|"
  WRITETO ">"
  WRITEFROM "<"
;

%type <std::string> item;
%type <std::vector<std::string>> list;
%token <std::string> TEXT;
%token <int> NUMBER;

%%
%start result;

result:
  list  { std::cout << $1[0] << " " << $1[1] << '\n'; }
;

list:
  %empty     { /* Generates an empty string list */ }
| list item  { $$ = $1; $$.push_back ($2); }
;

item:
  TEXT    { $$ = std::string($1);  }
| NUMBER  { $$ = std::to_string ($1 + 2); }
;
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
