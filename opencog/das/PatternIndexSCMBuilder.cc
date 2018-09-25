#include "PatternIndexSCMBuilder.h"
#include <opencog/util/Logger.h>
#include <stdexcept>

using namespace opencog;
using namespace std;

PatternIndexSCMBuilder::PatternIndexSCMBuilder(PatternIndex *index)
{
    DEBUG = true;
    this->index = index;
}

PatternIndexSCMBuilder::~PatternIndexSCMBuilder() 
{
}

void PatternIndexSCMBuilder::beforeInserting(const std::string &schemeStr)
{
    parseSCMFragment(schemeStr);
}

void PatternIndexSCMBuilder::afterInserting(Handle &toplevelHandle)
{
    KBBReference reference;
    // TODO use shared pointer
    //reference.handle(toplevelHandle);
    reference = toplevelHandle;
    index->index(kbb, reference);
    if (DEBUG) kbb.printForDebug("Indexed KBB: ", "\n");
}

void PatternIndexSCMBuilder::parseSCMFragment(const std::string &str)
{
    //if (DEBUG) printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
    if (DEBUG) printf("%s\n", str.c_str());

    kbb.parseSCM(str);
}
