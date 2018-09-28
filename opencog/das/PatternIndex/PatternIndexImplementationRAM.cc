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
    if (findIndexNode(key, &rootLocal, false) == nullptr) return;
    for (auto node : patternMatched) {
        auto it = occurrencesLocal.find(node->kbbID);
        if (it == occurrencesLocal.end()) throw runtime_error("Mapping not found");
        if (DEBUG) printf("Query: leaf has %lu UUIds\n", (*it).second->size());
        answer.insert(answer.end(), (*it).second->begin(), (*it).second->end());
    }
}

void PatternIndexImplementationRAM::query(list<KBB_UUID> &answer, 
                                          const KnowledgeBuildingBlock &key,
                                          bool externOnly)
{
    IndexNode *node = findIndexNode(key, &rootGlobal, false);
    if (node == nullptr) return;
    if (findIndexNode(key, &rootLocal, false) == nullptr) return;
    for (auto node : patternMatched) {
        auto it = occurrencesGlobal.find(node->kbbID);
        if (it == occurrencesGlobal.end()) throw runtime_error("Mapping not found");
        if (DEBUG) printf("Query: leaf has %lu UUIds\n", (*it).second->size());
        answer.insert(answer.end(), (*it).second->begin(), (*it).second->end());
    }   
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

void PatternIndexImplementationRAM::traverse(IndexNode *node)
{
    if (node->kbbID != 0) {
        patternMatched.push_back(node);
    }
    for (auto p : node->children) {
        traverse(p.second);
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
    patternMatched.clear();

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
                if ((tag.first & KnowledgeBuildingBlock::ANY_KBB_PATTERN_MASK) > 0) {
                    // Matches any subgraph. So all the leafs hereafter will be
                    // put in "patternMatched".
                    //printf("XXX ANY\n");
                    traverse(node);
                    return node;
                } else if ((tag.first & KnowledgeBuildingBlock::KBB_PATTERN_TYPE_MASK) > 0) {
                    // Matches any subgraphs whose toplevel == tag.first
                    Type t = (Type) (tag.first & KnowledgeBuildingBlock::KBB_PATTERN_TYPE_MASK);
                    //printf("XXX TYPE %u\n", t);
                    bool flag = false;
                    for (auto p : node->children) {
                        if (p.first.first == t) {
                            flag = true;
                            traverse(p.second);
                        }
                    }
                    return (flag ? node : nullptr);
                } else {
                    // No pattern match.
                    return nullptr;
                }
                return nullptr;
            }
        } else {
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
    } else {
        // Node matches key but key has no wildcards
        patternMatched.push_back(node);
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
