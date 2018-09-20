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

void PatternIndexSCMBuilder::afterInserting(const Handle &toplevelAtom)
{
}

void PatternIndexSCMBuilder::parseSCMFragment(const std::string &str)
{
    //if (DEBUG) printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
    if (DEBUG) printf("%s\n", str.c_str());

    KnowledgeBuildingBlock kbb;
    CompoundHashValue kbbHashValue;

    if (recursiveParse(str, kbb, kbbHashValue, 0) < 0) {
        throw runtime_error("Parse error\n");
    }

    index->index(&kbb, kbbHashValue.get());
    if (DEBUG) kbb.printForDebug("Indexed KBB: ", "\n");
}


int PatternIndexSCMBuilder::countTargets(const string &txt, unsigned int begin)
{
    int answer = 0;
    unsigned int cursor = begin;
    int level = 0;
    bool inStr = false;
    while (true) {
        if (cursor >= txt.length()) return -1;
        char c = txt.at(cursor++);
        if (inStr) {
            if (c == '\"') inStr = false;
        } else {
            if (c == '\"') {
                inStr = true;
            } else {
                if (c == '(') {
                    if (level == 0) answer++;
                    level++;
                }
                if (c == ')') {
                    if (level == 0) return answer;
                    level--;
                }
            }
        }
    }
}

int PatternIndexSCMBuilder::recursiveParse(const string &txt, 
                                        KnowledgeBuildingBlock &kbb, 
                                        CompoundHashValue &kbbHashValue,
                                        unsigned int begin)
{
    if (txt.at(begin) != '(') {
        fprintf(stderr, "Expected \'(\' at index %u of string %s\n", begin, txt.c_str());
        return -1;
    }
    int separatorPos = txt.find_first_of(" (", begin + 1);
    string typeName = txt.substr(begin + 1, separatorPos - begin - 1);
    Type type = nameserver().getType(typeName);
    if (type == NOTYPE) {
        fprintf(stderr, "Unknown type name: %s\n", typeName.c_str());
        return -1;
    }
    if (nameserver().isLink(type)) {
        int tvBegin = txt.find_first_of("(", separatorPos);
        if (tvBegin == -1) {
            fprintf(stderr, "Unable to parse link\n");
            return -1;
        }
        string checkTV = txt.substr(tvBegin + 1, 3);
        int targetBegin;
        if (checkTV == "stv") {
            int tvEnd = txt.find_first_of(")", tvBegin + 1);
            targetBegin = txt.find_first_of("(", tvEnd + 1);
        } else {
            targetBegin = tvBegin;
        }
        int targetEnd = 0;
        Arity targetCount = countTargets(txt, targetBegin);
        if (targetCount < 0){
            fprintf(stderr, "Unable to compute targetCount\n");
            return -1;
        }
        std::vector<KBB_HASHCODE> targetHashValues;
        KnowledgeBuildingBlock subkbb;
        CompoundHashValue subhash;
        for (unsigned i = 0; i < targetCount; i++) {
            targetEnd = recursiveParse(txt, subkbb, subhash, targetBegin);
            targetHashValues.push_back(subhash.get());
            if (targetEnd < 0) return -1;
            if (i != (targetCount -1)) {
                targetBegin = txt.find_first_of("(", targetEnd + 1);
            }
            subhash.reset();
        }
        if (nameserver().isA(type, UNORDERED_LINK)) {
            std::sort(targetHashValues.begin(), targetHashValues.end());
        }
        kbbHashValue.feed((KBB_HASHCODE) type);
        kbbHashValue.feed((KBB_HASHCODE) targetCount);
        for (const KBB_HASHCODE &n: targetHashValues) {
            kbbHashValue.feed(n);
        }
        subkbb.pushFront(type, targetCount, kbbHashValue.get());
        kbb.pushBack(subkbb);
        int DUMMY = txt.find_first_of(")", targetEnd + 1);
        return DUMMY;
    } else {
        int cursor = separatorPos + 2;
        bool nameEndedFlag = false;
        int level = 1;
        string nodeName = "";
        while (true) {
            char c = txt.at(cursor++);
            if (c == '\"') {
                nameEndedFlag = true;
                kbbHashValue.feed((KBB_HASHCODE) type);
                kbbHashValue.feed(nodeName);
                kbb.pushBack(type, 0, kbbHashValue.get());
            } else {
                if (nameEndedFlag) {
                    if (c == '(') level++;
                    if ((c == ')') && (--level == 0)) {
                        return cursor - 1;
                    }
                } else {
                    nodeName += c;
                }
            }
        }
    }
}

