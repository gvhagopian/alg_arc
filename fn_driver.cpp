#include <cctype>
#include <fstream>
#include <cassert>

#include "fn_driver.hpp"
#include <math.h>

using tok = FnParse::Parser::token;
using namespace std;


FnParse::Driver::Driver() {
    /* void */
}

FnParse::Driver::~Driver() {
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void 
FnParse::Driver::parse(const char * const filename) {
    
    assert(filename != nullptr);
    ifstream in_file(filename);
    
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    
    parse_helper(in_file);
    return;
}

void
FnParse::Driver::parse(istream &stream) {
    if(!stream.good() && stream.eof()) {
        return;
    }
    parse_helper(stream); 
    return;
}


void 
FnParse::Driver::parse_helper(istream &stream) {
    
    if (fnroot != nullptr) {
        cout << "deleting" << fnroot << endl;
        delete(fnroot);
    }
    
    delete(scanner);
    
    try {
        scanner = new FnParse::Scanner(&stream);
    } catch(bad_alloc &ba) {
        cerr << "Failed to allocate scanner: (" <<
        ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete(parser); 
    try {
        parser = new FnParse::Parser(
            (*scanner) /* scanner */, 
            (*this) /* driver */
        );
    } catch(bad_alloc &ba) {
        cerr << "Failed to allocate parser: (" << 
        ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    
    parse_result = parser->parse();
    
    return;
}

ostream& 
FnParse::Driver::print(ostream &stream) {
    stream << "Fn: " << this->fnroot << "\n";
    stream << "F(10): " << this->fnroot->eval(10.0d) << "\n";
    return stream;
}

FnParse::Node::Node(string varname) {
    this->type = var;
    this->varname = varname;
    
    cout << "var node (" << varname << ")" << endl;
}

FnParse::Node::Node(Parser::token::yytokentype new_op, Node *left_op, Node *right_op) {
    this->type = bin_op;
    this->op = new_op;
    
    assert(left_op != nullptr);
    assert(right_op != nullptr);
    
    this->left = left_op;
    this->right = right_op;
    
    cout << "2 op node " << left << " " << right << endl;
}

FnParse::Node::Node(Parser::token::yytokentype op, Node *operand) {
    this->type = unr_op;
    this->op = op;
    
    assert(operand != nullptr);
    this->left = operand;
}

FnParse::Node::Node(double lit_val) {
    this->type = lit;
    this->value = lit_val;
    cout << "lit node " << lit_val << endl;
}

double 
FnParse::Node::eval(double var_x) {
    double lval = 0.0d;
    double rval = 0.0d;
    
    switch (type) {
        case lit: return value;
        case var: return var_x;
        case bin_op:
            rval = right->eval(var_x);
        case unr_op:
            lval = left->eval(var_x);
            break;
    }
    
    switch (op) {
        case tok::POW:      return pow(lval, rval);
        case tok::SQRT:     return pow(lval, 0.5d);
        case tok::LOG2:     return log(lval);
        case tok::LOG10:    return log10(lval);
        
        case tok::SINH:     return sinh(lval);
        case tok::COSH:     return cosh(lval);
        case tok::TANH:     return tanh(lval);
        
        case tok::SIN:      return sin(lval);
        case tok::COS:      return cos(lval);
        case tok::TAN:      return tan(lval);
        
        case tok::ADD:      return lval + rval;
        case tok::SUB:      return lval - rval;
        case tok::MUL:      return lval * rval;
        case tok::DIV:      return lval / rval;
        
        case tok::ABS:      return abs(lval);
        case tok::FLOOR:    return floor(lval);
        case tok::CEIL:     return ceil(lval);
        
        default: cout << "unknown op " << op << endl; return 0.0d;
    }
}

FnParse::Node::~Node() {
    delete(this->left);
    delete(this->right);
}


ostream &
FnParse::operator<<(ostream &os, FnParse::Node *n) {
    
    switch (n->type) {
        case FnParse::Node::Type::lit:
            os << n->value;
            break;
        case FnParse::Node::Type::var: 
            os << "x";
            break;
        case FnParse::Node::Type::unr_op:
            os << tok_to_str(n->op) << '(' << n->left << ')';
            break;
        case FnParse::Node::Type::bin_op:
            if (n->op == tok::POW or n->op == tok::MOD) {
                os << tok_to_str(n->op) << '(' << n->left << ", " << n->right << ')';
            } else {
                os << '(' << n->left << ' ' << tok_to_str(n->op) << ' ' << n->right << ')';
            }
            break;
    }
    
    return os;
    
}

string
FnParse::tok_to_str(Parser::token::yytokentype tok) {
    switch (tok) {
        case tok::POW:      return "pow";
        case tok::SQRT:     return "sqrt";
        case tok::LOG2:     return "log2";
        case tok::LOG10:    return "log10";
        
        case tok::SINH:     return "sinh";
        case tok::COSH:     return "cosh";
        case tok::TANH:     return "tanh";
        
        case tok::SIN:      return "sin";
        case tok::COS:      return "cos";
        case tok::TAN:      return "tan";
        
        case tok::ADD:      return "+";
        case tok::SUB:      return "-";
        case tok::MUL:      return "*";
        case tok::DIV:      return "/";
        
        case tok::ABS:      return "abs";
        case tok::FLOOR:    return "floor";
        case tok::CEIL:     return "ceil"; 
    }
    return "?";
}

