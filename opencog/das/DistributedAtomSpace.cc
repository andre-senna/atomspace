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
    patternIndex = PatternIndex::factory();
    cacheManager = new CacheManager();
}

DistributedAtomSpace::~DistributedAtomSpace() 
{
    delete patternIndex;
    delete atomSpace;
    delete cacheManager;
}

void DistributedAtomSpace::getLocal(list<Handle> &answer, const KnowledgeBuildingBlock &key)
{
    list<Handle> indexAnswer;
    patternIndex->query(indexAnswer, key);
    for (const Handle &ref : indexAnswer) {
        if (key.matches(ref)) {
            answer.push_back(ref);
        }
    }
}


void DistributedAtomSpace::loadSCM(const std::string &fileName)
{
    PatternIndexSCMBuilder builder(patternIndex);

    if (SCMLoader::load(fileName, atomSpace, &builder)) {
        throw runtime_error("SCM file is invalid.");
    }
}
