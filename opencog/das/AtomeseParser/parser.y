%{
#include <opencog/atoms/base/Atom.h>
#include "../opencog/das/AtomeseParser/AtomeseParser.h"

// Functions defined in lexer.cc (generated by Flex)
extern int yylex();
//extern int yyparse();
//extern FILE *yyin;

// Functions defined in lexer.l
extern void yyerror(const char *s);

static bool DEBUG = false;
extern opencog::AtomeseParser *opencog_atomeseParser_environment;
%}

%union {
    int ival;
    float fval;
    char *sval;
    opencog::Type tval;
}

%token STV AV STAR

%token UNKNOWN_TYPE
%token <ival> INT
%token <fval> FLOAT
%token <tval> NODE_TYPE
%token <tval> LINK_TYPE
%token <sval> NODE_NAME

%%
atom_list : atom_list atom
          | atom
          ;

atom : node
     | link
     | pattern
     ;
               
node : '('
       NODE_TYPE
       NODE_NAME
       atom_values
       ')'
       comment { opencog_atomeseParser_environment->addNewNode($2, $3); }
     | '('
       NODE_TYPE
       NODE_NAME
       ')'
       comment { opencog_atomeseParser_environment->addNewNode($2, $3); }
     ;

link : '('
       LINK_TYPE { opencog_atomeseParser_environment->startNewLink($2); }
       link_body
       ')'
       comment { opencog_atomeseParser_environment->addNewLink(); }
       ;

pattern : '(' STAR ')' { opencog_atomeseParser_environment->addNewPattern(0); }
        | '(' NODE_TYPE STAR')' { opencog_atomeseParser_environment->addNewPattern($2); }
        | '(' LINK_TYPE STAR')' { opencog_atomeseParser_environment->addNewPattern($2); }

link_body : atom_values
            atom_list
          | atom_list

atom_values : atom_values atom_value
            | atom_value

atom_value : av
           | stv
           ;

av : '(' AV number number number ')' { if (DEBUG) printf("PARSER: av\n"); } ;

stv : '(' STV number number ')' { if (DEBUG) printf("PARSER: stv\n"); } ;

number: FLOAT | INT

comment : %empty
        | ';' square_bracket_number { if (DEBUG) printf("PARSER: non-empty comment\n"); }

square_bracket_number : '[' INT ']';

%%