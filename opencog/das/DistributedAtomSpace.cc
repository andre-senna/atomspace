#include "DistributedAtomSpace.h"
#include "PatternIndexSCMBuilder.h"
#include "SCMLoader.h"
#include <opencog/guile/SchemeEval.h>

using namespace opencog;
using namespace std;

DistributedAtomSpace::DistributedAtomSpace() 
{
    atomSpace = new AtomSpace();
    SchemeEval::init_scheme();
    SchemeEval::set_scheme_as(atomSpace);
    globalPatternIndex = PatternIndex::factory();
    localPatternIndex = PatternIndex::factory();
    cacheManager = new CacheManager();
}

DistributedAtomSpace::~DistributedAtomSpace() 
{
    delete globalPatternIndex;
    delete localPatternIndex;
    delete atomSpace;
    delete cacheManager;
}

void DistributedAtomSpace::loadSCM(const std::string &fileName)
{
    PatternIndexSCMBuilder builder(localPatternIndex);

    if (SCMLoader::load(fileName, atomSpace, &builder)) {
        throw runtime_error("SCM file is invalid.");
    }
}
