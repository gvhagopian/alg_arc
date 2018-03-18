#ifndef __FN_SCANNER_HPP__
#define __FN_SCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "fn_parser.tab.hh"
#include "location.hh"

namespace FnParse {

class Scanner : public yyFlexLexer {
public:
   
    Scanner(std::istream *in) : yyFlexLexer(in) { };
    virtual ~Scanner() { };
    
    using FlexLexer::yylex;
    
    virtual int yylex(
        FnParse::Parser::semantic_type * const lval, 
        FnParse::Parser::location_type *location
    );
    
    float val;
    
private:
    FnParse::Parser::semantic_type *yylval = nullptr;
};

}

#endif