%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%locations
%defines 
%define api.namespace {FnParse}
%define parser_class_name {Parser}

%code requires{
    namespace FnParse {
        class Driver;
        class Scanner;
        class Node;
    }

}

%parse-param {Scanner  &scanner}
%parse-param {Driver  &driver}

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    
    #include "fn_driver.hpp"

    #undef yylex
    #define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert


%token                  END            0     "end of file"

%token  <double>        NUMBER
%token                  DIV MUL ADD SUB
%token                  L_BRACKET R_BRACKET
%token                  PI
%token                  POW SQRT FACTORIAL MOD
%token                  LOG2 LOG10
%token                  FLOOR CEIL ABS
%token                  COS SIN TAN COSH SINH TANH
%token  <std::string>   VARIABLE

%type   <Node *>        calculation
%type   <Node *>        constant
%type   <Node *>        expr
%type   <Node *>        function
%type   <Node *>        log_function
%type   <Node *>        trig_function
%type   <Node *>        hyperbolic_function

// Operator Precedence

%left SUB
%left ADD
%left MUL
%left DIV
%left POW SQRT
%left LOG2 LOG10
%left SIN COS TAN COSH SINH TANH
%left ABS FLOOR CEIL
%right UMINUS

%start calculation

%%

calculation:
      expr END                      { $$ = $1; driver.fnroot = $$; }
    ;

constant:
      PI                            { $$ = new Node(3.141596d); }
    ;

expr:
      SUB expr %prec UMINUS         { $$ = new Node(Parser::token::MUL, $2, new Node(-1.0d)); }
    | NUMBER                        { $$ = new Node($1); }
    | VARIABLE                      { $$ = new Node($1); }
    | constant                      { $$ = $1; }
    | function                      { $$ = $1; }
    | expr DIV expr                 { $$ = new Node(Parser::token::DIV, $1, $3); }
    | expr MUL expr                 { $$ = new Node(Parser::token::MUL, $1, $3); }
    | L_BRACKET expr R_BRACKET      { $$ = $2; }
    | expr ADD expr                 { $$ = new Node(Parser::token::ADD, $1, $3); }
    | expr SUB expr                 { $$ = new Node(Parser::token::SUB, $1, $3); }
    | expr POW expr                 { $$ = new Node(Parser::token::POW, $1, $3); }
    | expr MOD expr                 { $$ = new Node(Parser::token::MOD, $1, $3); }
    ;

function:
      log_function                  { $$ = $1; }
    | trig_function                 { $$ = $1; }
    | hyperbolic_function           { $$ = $1; }
    | SQRT expr                     { $$ = new Node(Parser::token::SQRT, $2); }
    | expr FACTORIAL                { $$ = new Node(Parser::token::FACTORIAL, $1); }
    | ABS expr                      { $$ = new Node(Parser::token::ABS, $2); }
    | FLOOR expr                    { $$ = new Node(Parser::token::FLOOR, $2); }
    | CEIL expr                     { $$ = new Node(Parser::token::CEIL, $2); }
    ;

trig_function:
      COS expr                      { $$ = new Node(Parser::token::COS, $2); }
    | SIN expr                      { $$ = new Node(Parser::token::SIN, $2); }
    | TAN expr                      { $$ = new Node(Parser::token::TAN, $2); }
    ;

log_function:
      LOG2 expr                     { $$ = new Node(Parser::token::LOG2, $2); }
    | LOG10 expr                    { $$ = new Node(Parser::token::LOG10, $2); }
    ;

hyperbolic_function:
      COSH expr                     { $$ = new Node(Parser::token::COSH, $2); }
    | SINH expr                     { $$ = new Node(Parser::token::SINH, $2); }
    | TANH expr                     { $$ = new Node(Parser::token::TANH, $2); }
    ;

%%


void 
FnParse::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
