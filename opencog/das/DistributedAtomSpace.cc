#include "DistributedAtomSpace.h"

using namespace opencog;

DistributedAtomSpace::DistributedAtomSpace() 
{
    _globalPatternIndex = PatternIndex::factory();
    _localPatternIndex = PatternIndex::factory();
    _atomSpace = new AtomSpace();
    _cacheManager = new CacheManager();
}

DistributedAtomSpace::~DistributedAtomSpace() 
{
    delete _globalPatternIndex;
    delete _localPatternIndex;
    delete _atomSpace;
    delete _cacheManager;
}
