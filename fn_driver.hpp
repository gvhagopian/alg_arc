#ifndef __FN_DRIVER_HPP__
#define __FN_DRIVER_HPP__

#include <string>
#include <cstddef>
#include <istream>
#include <iostream>

#include "fn_scanner.hpp"
#include "fn_parser.tab.hh"

using namespace std;

namespace FnParse {

class Driver {
public:
    Driver();

    virtual ~Driver();

    void parse(const char * const filename);
    void parse(istream &iss);
    
    ostream& print(ostream &stream);
    
    int parse_result = -1;
    
    Node *fnroot = nullptr;
    
private:
    
    void parse_helper(istream &stream);
    
    FnParse::Parser  *parser  = nullptr;
    FnParse::Scanner *scanner = nullptr;
};


string tok_to_str(Parser::token::yytokentype tok);

class Node {
public:
    Node();
    Node(string varname);
    Node(double lit_val);
    Node(Parser::token::yytokentype op, Node *left, Node *right);
    Node(Parser::token::yytokentype op, Node *operand);
    
    virtual ~Node();
    
    enum Type {
        lit,
        var,
        bin_op,
        unr_op
    };
    
    double eval(double var_x);
    
    Node::Type type;
    
    Parser::token::yytokentype op;
    double value = 0.0f;
    string varname;
    
    friend ostream& operator<<(ostream &os, Node *n);
    
private:
    Node *left  = nullptr;
    Node *right = nullptr;
};

}

#endif
