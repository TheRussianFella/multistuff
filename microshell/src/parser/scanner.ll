%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hh"
# include "parser.hh"
%}

%{
// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 6.4.0, 7.3.0.
#if defined __GNUC__ && !defined __clang__ && 6 <= __GNUC__
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

// Of course, when compiling C as C++, expect warnings about NULL.
#if defined __clang__
# pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#elif defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
# pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
%}

%option noyywrap nounput noinput batch debug

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

com_part \"([^\\\"]|\\.)*\"|[=]|[^|<>\"*?= \t\n]+

%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

[ \t]+     loc.step ();
\n+        loc.lines (yyleng); loc.step ();

"|"            return yy::parser::make_PIPESEP(loc);
">"            return yy::parser::make_WRITETO(loc);
"<"            return yy::parser::make_WRITEFROM(loc);

{com_part}     return yy::parser::make_COMMAND_PART (yytext, loc);
.              {
                throw yy::parser::syntax_error
                  (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>        return yy::parser::make_END (loc);
%%



void
Driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;

  yy_scan_string(target.c_str());

}

void
Driver::scan_end ()
{
  yylex_destroy();
}
