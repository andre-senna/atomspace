#include "KnowledgeBuildingBlock.h"

using namespace opencog;
using namespace std;

KnowledgeBuildingBlock::KnowledgeBuildingBlock() 
{
}

KnowledgeBuildingBlock::~KnowledgeBuildingBlock() 
{
    definition.clear();
}

void KnowledgeBuildingBlock::pushFront(Type type, Arity arity, KBB_HASHCODE atomID)
{
    KBBElement newElement;
    newElement.type = type;
    newElement.arity = arity;
    newElement.atomID = atomID;
    // TODO: use emplace()
    definition.insert(definition.begin(), newElement);
}


void KnowledgeBuildingBlock::pushBack(Type type, Arity arity, KBB_HASHCODE atomID)
{
    KBBElement newElement;
    newElement.type = type;
    newElement.arity = arity;
    newElement.atomID = atomID;
    // TODO: use emplace()
    definition.push_back(newElement);
}

void KnowledgeBuildingBlock::pushBack(const KnowledgeBuildingBlock &kbb)
{
    for (const KBBElement &elem : kbb.definition) {
        pushBack(elem.type, elem.arity, elem.atomID);
    }
}

void KnowledgeBuildingBlock::printForDebug(const string &prefix, const string &suffix)
{
    printf("%s", prefix.c_str());
    for (const KBBElement &elem : definition) {
        printf("(%u %lu %lu) ", elem.type, elem.arity, elem.atomID);
    }
    printf("%s", suffix.c_str());
}

