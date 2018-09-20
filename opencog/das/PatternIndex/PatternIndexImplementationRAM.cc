#include "PatternIndexImplementationRAM.h"
#include <stdexcept>

using namespace opencog;
using namespace std;

PatternIndexImplementationRAM::PatternIndexImplementationRAM() 
{
    _nextKBBID = 1;
}

PatternIndexImplementationRAM::~PatternIndexImplementationRAM() 
{
}

// --------------------------------------------------------------------------------
// Public PatternIndex API

// TODO Add subpatterns as well
void PatternIndexImplementationRAM::index(KnowledgeBuildingBlock *kbb, 
                                          KBB_HASHCODE toplevelAtomID)
{

    unsigned short int size = kbb->size();
    if (size == 0) {
        #ifdef __DEBUG
            printf("Attempt to index empty KBB. Discarding it...\n");
        #endif
        // Empty KBB. Nothing is done.
        return;
    }

    // Traverse the prefix index tree to find the leaf corresponding to the
    // passed KBB. If the leaf already exists, add another occurrence to
    // the corresponding KBB ID. Otherwise a new KDD ID is created and the
    // occurrence is added.

    unsigned short int kbbCursor = 0;
    IndexNode *node = &_root;
    IndexNode::IndexLinkTag tag;

    while (kbbCursor < size) {
        tag.first = kbb->getTypeAt(kbbCursor);
        tag.second = kbb->getArityAt(kbbCursor);
        IndexNode::TagToIndexNodeMap::iterator it = (node->children).find(tag);
        if (it == (node->children).end()) {
            // There is no branch in this direction yet. So a new node is
            // created.
            IndexNode *newNode = new IndexNode();
            (node->children).emplace(tag, newNode);
            node = newNode;
        } else {
            // Reuse branch
            node = (*it).second;
        }
        kbbCursor++;
    }

    // At this point, node is pointing to the proper leaf, which can be a newly
    // created one. In this case, a new KBB ID need to be assigned to this leaf
    if (node->kbbID == 0) {
        node->kbbID = _nextKBBID++;
    }

    // Actually inserts the new occurrence of the passed KBB
    insertKBBOccurrence(node->kbbID, toplevelAtomID);
    #ifdef __DEBUG
        printf("New KBB indexed\n");
    #endif
}

void PatternIndexImplementationRAM::query(list<KBB_HASHCODE> &answer, KnowledgeBuildingBlock *key)
{
    unsigned short int size = key->size();
    if (size == 0) {
        // Empty KBB. Nothing is done.
        return;
    }

    // Traverse the prefix index tree to find the leaf corresponding to the
    // passed KBB.

    unsigned short int kbbCursor = 0;
    IndexNode *node = &_root;
    IndexNode::IndexLinkTag tag;

    while (kbbCursor < size) {
        tag.first = key->getTypeAt(kbbCursor);
        tag.second = key->getArityAt(kbbCursor);
        IndexNode::TagToIndexNodeMap::iterator it = (node->children).find(tag);
        if (it == (node->children).end()) {
            // Mismatch. Nothing is added to 'answer'
            return;
        } else {
            node = (*it).second;
        }
        kbbCursor++;
    }

    // At this point, node is pointing to the corresponding leaf.
   
    TestContainer::iterator it = _occurrences.find(node->kbbID);
    if (it == _occurrences.end()) {
        throw runtime_error("Mapping not found");
    }

    #ifdef __DEBUG
        printf("Query: leaf has %lu UUIds\n", (*it).second->size());
    #endif

    answer.insert(answer.begin(), (*it).second->begin(), (*it).second->end());
}

// --------------------------------------------------------------------------------
// Private methods

void PatternIndexImplementationRAM::insertKBBOccurrence(unsigned int kbbID, 
                                                        KBB_HASHCODE toplevelAtomID)
{
    TestContainer::iterator it = _occurrences.find(kbbID);
    if (it == _occurrences.end()) {
        list<KBB_HASHCODE> *newList = new list<KBB_HASHCODE>();
        newList->push_back(toplevelAtomID);
        _occurrences.emplace(kbbID, newList);
    } else {
        (*it).second->push_back(toplevelAtomID);
    }
}

// --------------------------------------------------------------------------------
// IndexNode (inner class) methods

PatternIndexImplementationRAM::IndexNode::IndexNode()
{
    // 0 means NO ID assigned yet
    kbbID = 0;
}

PatternIndexImplementationRAM::IndexNode::~IndexNode()
{
    for (const auto& element : children) {
        delete element.second;
    }
}
