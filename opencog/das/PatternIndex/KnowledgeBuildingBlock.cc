#include "KnowledgeBuildingBlock.h"
#include "../CartesianProductGenerator.h"
#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/base/Link.h>

using namespace opencog;
using namespace std;

// Static initialization
const Type KnowledgeBuildingBlock::ANY_KBB_PATTERN_MASK   = (Type) ((((Type) 1) << (8 * (sizeof(Type) - 1))) << 6);
const Type KnowledgeBuildingBlock::TYPED_KBB_PATTERN_MASK = (Type) ((((Type) 2) << (8 * (sizeof(Type) - 1))) << 6);
const Type KnowledgeBuildingBlock::KBB_PATTERN_TYPE_MASK  = (Type) (~((((Type) 3) << (8 * (sizeof(Type) - 1))) << 6));

KnowledgeBuildingBlock::KnowledgeBuildingBlock() 
{
}

KnowledgeBuildingBlock::KnowledgeBuildingBlock(const KnowledgeBuildingBlock &other)
{
    pushBack(other);
}

KnowledgeBuildingBlock::~KnowledgeBuildingBlock() 
{
    definition.clear();
}

void KnowledgeBuildingBlock::pushFront(Type type, Arity arity, KBB_HASHCODE atomHash)
{
    KBBElement newElement;
    newElement.type = type;
    newElement.arity = arity;
    newElement.atomHash = atomHash;
    // TODO: use emplace()
    definition.insert(definition.begin(), newElement);
}


void KnowledgeBuildingBlock::pushBack(Type type, Arity arity, KBB_HASHCODE atomHash)
{
    KBBElement newElement;
    newElement.type = type;
    newElement.arity = arity;
    newElement.atomHash = atomHash;
    // TODO: use emplace()
    definition.push_back(newElement);
}

void KnowledgeBuildingBlock::replaceLast(Type type, Arity arity, KBB_HASHCODE atomHash)
{
    definition.back() = {type, arity, atomHash};
}

void KnowledgeBuildingBlock::pushBack(const KnowledgeBuildingBlock &kbb)
{
    for (const KBBElement &elem : kbb.definition) {
        pushBack(elem.type, elem.arity, elem.atomHash);
    }
}

void KnowledgeBuildingBlock::pushBack(const KnowledgeBuildingBlock &kbb, unsigned int n)
{
    pushBack(definition[n].type, definition[n].arity, definition[n].atomHash);
}

void KnowledgeBuildingBlock::pushBackAtom(const KnowledgeBuildingBlock &kbb, unsigned int cursor)
{
    unsigned int remaining = 1;
    while (remaining > 0) {
        definition.push_back(kbb.definition[cursor]);
        remaining = remaining - 1 + kbb.definition[cursor++].arity;
    }
}

void KnowledgeBuildingBlock::pushBack(KnowledgeBuildingBlock *kbb)
{
    for (const KBBElement &elem : kbb->definition) {
        pushBack(elem.type, elem.arity, elem.atomHash);
    }
}

void KnowledgeBuildingBlock::parseSCM(const std::string &str)
{
    clear();
    CompoundHashValue kbbHashValue;
    if (recursiveParse(str, kbbHashValue, 0) < 0) {
        throw runtime_error("Parse error\n");
    }
}

bool KnowledgeBuildingBlock::recursiveMatches(unsigned int cursor,
                                              const Handle &handle) const
{
    Type t = definition[cursor].type;
    CompoundHashValue hash;

//printForDebug("kbb: ", "\n");
//printf("Cursor: %u\n", cursor);
//printf("Handle: %s", handle->to_string().c_str());

    if ((t & ANY_KBB_PATTERN_MASK) > 0) {
        // it's a wildcard that matches any subgraph
        return true;
    } else {
        Type ht = handle->get_type();
//printf("ht: %u\n", ht);
//printf("t: %u\n", t);
//printf("t & PATTERN MASK: %u\n", (t & TYPED_KBB_PATTERN_MASK));
//printf("t & TYPE MASK: %u\n", (t & KBB_PATTERN_TYPE_MASK));
        if (((t & KBB_PATTERN_TYPE_MASK) == ht) &&
            ((t & TYPED_KBB_PATTERN_MASK) > 0)) {
            // it's a "typed" wildcard. Matches any subgraph whose
            // toplevel atom matches the given type (if type is a link, its
            // outgoing doesn't matter, if it's a node its name doesn't matter)
            return true;
        } else {
            // If reached here then pattern's toplevel is not a wildcard.
            Arity a = definition[cursor].arity;
            Arity ha = (handle->is_link() ? LinkCast(handle)->get_arity() : 0);
            if ((t != ht) || (a != ha)) return false; // mismatch
            if (a == 0) {
                // Handle is a Node. Use hashcode to check match.
                hash.reset();
                hash.feed(ht);
                hash.feed(NodeCast(handle)->get_name());
//printf("%u %lu %s\n", ht, ha, NodeCast(handle)->get_name().c_str());
//printf("%lu %lu\n", definition[cursor].atomHash, hash.get());
//printf("%s\n", (definition[cursor].atomHash == hash.get()) ? "true" : "false");
                return (definition[cursor].atomHash == hash.get());
            } else {
                // Handle is a Link.
                if (nameserver().isA(t, UNORDERED_LINK)) {
                    // It's an UNORDERED_LINK so any choice of Handle's
                    // outgoing set to pattern's outgoing set that
                    // matches are OK. For example, if Handle's outgoing
                    // is (a, b) and pattern's outgoing is (x, y),
                    // so we may have either:
                    //
                    // (1) a matches x, AND b matches y
                    // (2) a matches y AND b matches x
                    //
                    // So if (1) or (2) passes, it's a match! If none
                    // passes, it's a mismatch.
                    CartesianProductGenerator cp(a, a, true);
                    while (! cp.depleted()) {
//cp.printForDebug("cp: ", "\n");
                        bool flag = true;
                        for (unsigned int i = 0; i < a; i++) {
                            unsigned int targetCursor = cursor + 1;
                            for (unsigned int j = 0; j < i; j++) {
                                targetCursor = targetCursor + definition[targetCursor].arity + 1;
                            }
                            if (! recursiveMatches(targetCursor, 
                                                   LinkCast(handle)->getOutgoingAtom(cp.at(i)))) {
                                flag = false;
                                break;
                            }
                        }
                        if (flag) {
                            //printf("Matched handle: %s", handle->to_string().c_str());
                            return true;
                        }
                        cp.next();
                    }
//printf("Don't matched handle: %s", handle->to_string().c_str());
                    return false;
                } else {
                    // It's an ORDERED_LINK so each handle in outgoing
                    // set must match.
                    unsigned int nextTarget = cursor + 1;
                    for (unsigned int i = 0; i < a; i++) {
                        if (! recursiveMatches(nextTarget, LinkCast(handle)->getOutgoingAtom(i))) {
                            return false;
                        }
                        nextTarget = nextTarget + definition[nextTarget].arity + 1;
                    }
                    return true;
                }
            }
        }
    }
}

bool KnowledgeBuildingBlock::matches(const Handle &handle) const
{
    // empty pattern never matches
    return (definition.size() > 0) && recursiveMatches(0, handle);
}


int KnowledgeBuildingBlock::recursiveParse(const string &txt, 
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
            targetEnd = subkbb.recursiveParse(txt, subhash, targetBegin);
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
        this->pushBack(subkbb);
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
                this->pushBack(type, 0, kbbHashValue.get());
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

int KnowledgeBuildingBlock::countTargets(const string &txt, unsigned int begin)
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

void KnowledgeBuildingBlock::printForDebug(const string &prefix, const string &suffix) const
{
    printf("%s", prefix.c_str());
    for (const KBBElement &elem : definition) {
        printf("(%u %lu %lu) ", elem.type, elem.arity, elem.atomHash);
    }
    printf("%s", suffix.c_str());
}

