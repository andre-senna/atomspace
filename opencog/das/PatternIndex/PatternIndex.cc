#include "PatternIndex.h"
#include "PatternIndexImplementationRAM.h"

using namespace opencog;

PatternIndex::~PatternIndex()
{
}

PatternIndex *PatternIndex::factory()
{
    return new PatternIndexImplementationRAM();
}
