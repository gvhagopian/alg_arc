%{
#include <string>

#include "fn_scanner.hpp"

#undef  YY_DECL
#define YY_DECL int FnParse::Scanner::yylex(FnParse::Parser::semantic_type * const lval, FnParse::Parser::location_type *loc)

using token = FnParse::Parser::token;

#define yyterminate() return( token::END )

#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option yyclass="FnParse::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

[ \t]
[0-9]+(\.[0-9]+)?                                   { yylval->build(atof(YYText())); return token::NUMBER; }
"+"                                                 { return token::ADD; }
"-"                                                 { return token::SUB; }
"*"                                                 { return token::MUL; }
"/"                                                 { return token::DIV; }
"%"                                                 { return token::MOD; }
"^"                                                 { return token::POW; }
"("                                                 { return token::L_BRACKET; }
")"                                                 { return token::R_BRACKET; }
"!"                                                 { return token::FACTORIAL; }
(?i:pi)                                             { return token::PI; }
(?i:mod)                                            { return token::MOD; }
"^"                                                 { return token::POW; }
(?i:floor)                                          { return token::FLOOR; }
(?i:ceil)                                           { return token::CEIL; }
(?i:abs)                                            { return token::ABS; }
(?i:cos)                                            { return token::COS; }
(?i:sin)                                            { return token::SIN; }
(?i:tan)                                            { return token::TAN; }
(?i:sinh)                                           { return token::SINH; }
(?i:cosh)                                           { return token::COSH; }
(?i:tanh)                                           { return token::TANH; }
(?i:sqrt)                                           { return token::SQRT; }
(?i:log2)                                           { return token::LOG2; }
(?i:log10)                                          { return token::LOG10; }
[xX]                                                { yylval->build<std::string>(YYText()); return token::VARIABLE; }
.                                                   { yyterminate(); }

%%
