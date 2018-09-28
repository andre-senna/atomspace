#include "PatternIndexImplementationRAM.h"
#include <stdexcept>

using namespace opencog;
using namespace std;

PatternIndexImplementationRAM::PatternIndexImplementationRAM() 
{
    nextKBBID = 1;
}

PatternIndexImplementationRAM::~PatternIndexImplementationRAM() 
{
}

// --------------------------------------------------------------------------------
// Public PatternIndex API

// TODO Add subpatterns as well
void PatternIndexImplementationRAM::index(const KnowledgeBuildingBlock &kbb, 
                                          const Handle &toplevel)
{
    IndexNode *node = findIndexNode(kbb, &rootLocal, true);
    insertKBBOccurrence(toplevel, node->kbbID);
    if (DEBUG) printf("New KBB indexed\n");
}

// TODO Add subpatterns as well
void PatternIndexImplementationRAM::index(const KnowledgeBuildingBlock &kbb, 
                                          KBB_UUID id)
{
    IndexNode *node = findIndexNode(kbb, &rootGlobal, true);
    insertKBBOccurrence(id, node->kbbID);
    if (DEBUG) printf("New KBB indexed\n");
}

void PatternIndexImplementationRAM::query(list<Handle> &answer, 
                                          const KnowledgeBuildingBlock &key,
                                          bool localOnly)
{
    IndexNode *node = findIndexNode(key, &rootLocal, false);
    if (node == nullptr) return;
    auto it = occurrencesLocal.find(node->kbbID);
    if (it == occurrencesLocal.end()) throw runtime_error("Mapping not found");
    if (DEBUG) printf("Query: leaf has %lu UUIds\n", (*it).second->size());
    answer.insert(answer.begin(), (*it).second->begin(), (*it).second->end());
}

void PatternIndexImplementationRAM::query(list<KBB_UUID> &answer, 
                                          const KnowledgeBuildingBlock &key,
                                          bool externOnly)
{
    IndexNode *node = findIndexNode(key, &rootGlobal, false);
    if (node == nullptr) return;
    auto it = occurrencesGlobal.find(node->kbbID);
    if (it == occurrencesGlobal.end()) throw runtime_error("Mapping not found");
    if (DEBUG) printf("Query: leaf has %lu UUIds\n", (*it).second->size());
    answer.insert(answer.begin(), (*it).second->begin(), (*it).second->end());
}

// --------------------------------------------------------------------------------
// Private methods

void PatternIndexImplementationRAM::insertKBBOccurrence(const Handle &kbbReference,
                                                        const KBB_DBID &kbbID)
{
    auto it = occurrencesLocal.find(kbbID);
    if (it == occurrencesLocal.end()) {
        list<Handle> *newList = new list<Handle>();
        newList->push_back(kbbReference);
        occurrencesLocal.emplace(kbbID, newList);
    } else {
        (*it).second->push_back(kbbReference);
    }
}

void PatternIndexImplementationRAM::insertKBBOccurrence(KBB_UUID kbbReference,
                                                        const KBB_DBID &kbbID)
{
    auto it = occurrencesGlobal.find(kbbID);
    if (it == occurrencesGlobal.end()) {
        list<KBB_UUID> *newList = new list<KBB_UUID>();
        newList->push_back(kbbReference);
        occurrencesGlobal.emplace(kbbID, newList);
    } else {
        (*it).second->push_back(kbbReference);
    }
}

PatternIndexImplementationRAM::IndexNode *PatternIndexImplementationRAM::findIndexNode(
                                              const KnowledgeBuildingBlock &kbb,
                                              IndexNode *root,
                                              bool insertFlag)
{
    unsigned short int size = kbb.size();
    if (size == 0) {
        if (insertFlag) {
            throw runtime_error("Attempt to index an empty KBB");
        } else {
            return nullptr;
        }
    }

    // Traverse the prefix index tree to find the leaf corresponding to the
    // passed KBB. If the leaf already exists, add another occurrence to
    // the corresponding KBB ID. Otherwise a new KDD ID is created and the
    // occurrence is added.

    unsigned short int kbbCursor = 0;
    IndexNode *node = root;
    IndexNode::IndexLinkTag tag;

    while (kbbCursor < size) {
        tag.first = kbb.getTypeAt(kbbCursor);
        tag.second = kbb.getArityAt(kbbCursor);
        IndexNode::TagToIndexNodeMap::iterator it = (node->children).find(tag);
        if (it == (node->children).end()) {
            // Mismatch. if inserting, a new IndexNode is created.
            if (insertFlag) {
                IndexNode *newNode = new IndexNode();
                (node->children).emplace(tag, newNode);
                node = newNode;
            } else {
                return nullptr;
            }
        } else {
            // Reuse branch
            node = (*it).second;
        }
        kbbCursor++;
    }

    if (insertFlag) {
        // At this point, node is pointing to the proper leaf, which can be a newly
        // created one. In this case, a new KBB ID need to be assigned to this leaf
        if (node->kbbID == 0) {
            node->kbbID = nextKBBID++;
        }
    }

    return node;
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
