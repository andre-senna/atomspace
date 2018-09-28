#include "AtomeseParser.h"
#include "../PatternIndex/KnowledgeBuildingBlock.h"
#include <stdexcept>

using namespace opencog;
using namespace std;

const bool AtomeseParser::DEBUG = false;

AtomeseParser::AtomeseParser(KnowledgeBuildingBlock &kbb) 
{
    currentArity = 0;
    MODE = SINGLE_KBB;
    currentKBB = &kbb;
    currentKBB->clear();
}

AtomeseParser::AtomeseParser(list<KnowledgeBuildingBlock *> &kbbList) 
{
    currentArity = 0;
    MODE = KBB_LIST;
    currentKBB = new KnowledgeBuildingBlock();;
    currentKBBList = &kbbList;
}

AtomeseParser::~AtomeseParser() 
{
    for (auto kbb : kbbStack) {
        kbb->clear();
        delete kbb;
    }
}

// implemented in lexer.l
bool opencog_atomeseParser_parseFile(list<KnowledgeBuildingBlock *> &kbbList, const char *fname);
void opencog_atomeseParser_parseString(KnowledgeBuildingBlock &kbb, const char *s);

void AtomeseParser::parseFile(list<KnowledgeBuildingBlock *> &kbbList, const std::string &fname)
{
    if (opencog_atomeseParser_parseFile(kbbList, fname.c_str())) {
        throw runtime_error("Could not open file " + fname);
    }
}

void AtomeseParser::parseString(KnowledgeBuildingBlock &kbb, const string &txt)
{
    if (DEBUG) printf("Parsing: %s\n", txt.c_str());
    opencog_atomeseParser_parseString(kbb, txt.c_str());
    if (DEBUG) kbb.printForDebug("Parsed KBB: ", "\n");
}

// --------------------------------------------------------------------------------
// API used by Bison

void AtomeseParser::addNewNode(Type t, char *name)
{

    if (DEBUG) printf("addNewNode() %u %s\n", t, name);
    currentArity++;
    hashValue.reset();
    hashValue.feed(t);
    hashValue.feed(name);
    if (DEBUG) printf("Node hash value = %lu\n", hashValue.get());
    currentKBB->pushBack(t, 0, hashValue.get());
    if (! targetHashValuesStack.empty()) {
        targetHashValuesStack.back().push_back(hashValue.get());
    }

    if ((MODE == KBB_LIST) && (arityStack.empty())) {
        if (DEBUG) currentKBB->printForDebug("New Node: ", "\n");
        if (DEBUG) printf("--------------------------------------------------------------------------------\n");
        currentKBBList->push_back(currentKBB);
        currentKBB = new KnowledgeBuildingBlock();;
    }

    free(name);
}

void AtomeseParser::startNewLink(Type t)
{
    if (DEBUG) printf("startNewLink() %u\n", t);

    typeStack.push_back(t);
    currentArity++;
    arityStack.push_back(currentArity);
    currentArity = 0;
    targetHashValuesStack.emplace(targetHashValuesStack.end());
    KnowledgeBuildingBlock *kbb = new KnowledgeBuildingBlock();
    kbbStack.push_back(currentKBB);
    currentKBB = kbb;
}

void AtomeseParser::addNewLink()
{
    if (DEBUG) printf("addNewLink()\n");
    //if (DEBUG) currentKBB->printForDebug("currentKBB: ", "\n");

    //if (! kbbStack.empty()) {
        KnowledgeBuildingBlock *nextCurrentKBB = kbbStack.back();
        //if (DEBUG) nextCurrentKBB->printForDebug("nextCurrentKBB: ", "\n");
        Type currentLinkType = typeStack.back();
        typeStack.pop_back();
        if (nameserver().isA(currentLinkType, UNORDERED_LINK)) {
            std::sort(targetHashValuesStack.back().begin(), targetHashValuesStack.back().end());
        }
        CompoundHashValue h;
        h.feed(currentLinkType);
        h.feed(currentArity);
        for (const KBB_HASHCODE &n: targetHashValuesStack.back()) {
            h.feed(n);
        }
        targetHashValuesStack.pop_back();
        if (! targetHashValuesStack.empty()) {
            targetHashValuesStack.back().push_back(h.get());
        }
        currentKBB->pushFront(currentLinkType, currentArity, h.get());
        if (DEBUG) printf("Link hash value = %lu\n", hashValue.get());
        //if (DEBUG) currentKBB->printForDebug("currentKBB: ", "\n");
        nextCurrentKBB->pushBack(currentKBB);
        //if (DEBUG) nextCurrentKBB->printForDebug("nextCurrentKBB: ", "\n");
        currentArity = arityStack.back();
        kbbStack.pop_back();
        arityStack.pop_back();
        delete currentKBB;
        currentKBB = nextCurrentKBB;

        if ((MODE == KBB_LIST) && (arityStack.empty())) {
            if (DEBUG) currentKBB->printForDebug("New Link: ", "\n");
            if (DEBUG) printf("--------------------------------------------------------------------------------\n");
            currentKBBList->push_back(currentKBB);
            currentKBB = new KnowledgeBuildingBlock();;
        }
    //}
}

void AtomeseParser::addNewPattern(Type t)
{
    if (DEBUG) printf("addNewPattern() %u\n", t);

    if (t == 0) {
        t = KnowledgeBuildingBlock::ANY_KBB_PATTERN_MASK;
    } else {
        t |= KnowledgeBuildingBlock::TYPED_KBB_PATTERN_MASK;
    }

    currentArity++;
    hashValue.reset();
    hashValue.feed(t);
    if (DEBUG) printf("Pattern hash value = %lu\n", hashValue.get());
    currentKBB->pushBack(t, 0, hashValue.get());
    if (! targetHashValuesStack.empty()) {
        targetHashValuesStack.back().push_back(hashValue.get());
    }

    if ((MODE == KBB_LIST) && (arityStack.empty())) {
        if (DEBUG) currentKBB->printForDebug("New Pattern: ", "\n");
        if (DEBUG) printf("--------------------------------------------------------------------------------\n");
        currentKBBList->push_back(currentKBB);
        currentKBB = new KnowledgeBuildingBlock();;
    }
}

