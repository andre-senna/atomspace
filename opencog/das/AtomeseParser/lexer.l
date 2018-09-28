%{
#include <list>
#include <opencog/atoms/base/Atom.h>
#include "parser.hh"
#include "../opencog/das/PatternIndex/KnowledgeBuildingBlock.h"
#include "../opencog/das/AtomeseParser/AtomeseParser.h"
#include "../opencog/das/CompoundHashValue.h"
static bool DEBUG = false;

%}
%option noyywrap
%%
[ \t\n] ;
[sS][tT][vV] {
    if (DEBUG) printf("LEXER: STV\n");
    return STV;
}
[aA][vV] {
    if (DEBUG) printf("LEXER: AV\n");
    return AV;
}
[0-9]+\.[0-9]+ {
    yylval.fval = atof(yytext);
    if (DEBUG) printf("LEXER: FLOAT %f\n", yylval.fval);
    return FLOAT;
}
[0-9]+ {
    yylval.ival = atoi(yytext);
    if (DEBUG) printf("LEXER: INT %d\n", yylval.ival);
    return INT;
}
[a-zA-Z]+ {
    opencog::Type t = opencog::nameserver().getType(yytext);
    if (opencog::nameserver().isNode(t)) {
        yylval.tval = t;
        if (DEBUG) printf("LEXER: NODE_TYPE %u\n", yylval.tval);
        return NODE_TYPE;
    } else if (opencog::nameserver().isLink(t)) {
        yylval.tval = t;
        if (DEBUG) printf("LEXER: LINK_TYPE %u\n", yylval.tval);
        return LINK_TYPE;
    } else {
        if (DEBUG) printf("LEXER: UNKNOWN_TYPE %u\n", t);
        return UNKNOWN_TYPE;
    }
}
\"(\\.|[^\\"])*\" {
    yylval.sval = (char *) malloc((strlen(yytext) - 1) * sizeof(char));
    unsigned int i = 1;
    while (yytext[i] != '\"') {
        yylval.sval[i - 1] = yytext[i];
        i++;
    }
    yylval.sval[i - 1] = '\0';
    if (DEBUG) printf("LEXER: NODE_NAME <%s>\n", yylval.sval);
    return NODE_NAME;
}
\* {
    if (DEBUG) printf("LEXER: STAR\n");
    return STAR;
}
. {
    if (DEBUG) printf("LEXER: '%c'\n", yytext[0]);
    return yytext[0];
}
%%

using namespace opencog;
using namespace std;

AtomeseParser *opencog_atomeseParser_environment;

// --------------------------------------------------------------------------------
// API used by AtomeseParser class

void opencog_atomeseParser_parseString(KnowledgeBuildingBlock &kbb, const char *s) {

    // Setup new input buffer for Flex
    YY_BUFFER_STATE inputStringBuffer = yy_scan_string(s);
    yy_switch_to_buffer(inputStringBuffer);
    BEGIN(INITIAL);

    AtomeseParser ap(kbb);
    opencog_atomeseParser_environment = &ap;
    
    yyparse();
    yy_delete_buffer(inputStringBuffer);

    // just a hack to avoid a warning message when compiling
    if (false) yyunput(0, 0);
}

bool opencog_atomeseParser_parseFile(list<KnowledgeBuildingBlock *> &kbbList, const char *fname) {

    // Setup new input buffer for Flex
    FILE *f = fopen(fname, "r");
    if (! f) return true; // could not open file
    YY_BUFFER_STATE inputFileBuffer = yy_create_buffer(f, YY_BUF_SIZE);
    yy_switch_to_buffer(inputFileBuffer);
    BEGIN(INITIAL);

    AtomeseParser ap(kbbList);
    opencog_atomeseParser_environment = &ap;

    yyparse();

    yy_delete_buffer(inputFileBuffer);
    fclose(f);

    return false;
}

// --------------------------------------------------------------------------------
// API used by Bison parser

void yyerror(const char *s) {
    printf("Parse error: %s\n", s);
    exit(-1);
}
