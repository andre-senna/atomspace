/*
 * Copyright (C) 2017 OpenCog Foundation
 *
 * Author: Andre Senna <https://github.com/andre-senna>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_PATTERNINDEXIMPLEMENTATIONRAM_H
#define _OPENCOG_PATTERNINDEXIMPLEMENTATIONRAM_H

#include "PatternIndex.h"
#include <map>
#include <list>

//#define __DEBUG

namespace opencog
{

/**
 *
 */
class PatternIndexImplementationRAM: public PatternIndex
{

public:

    PatternIndexImplementationRAM();
    ~PatternIndexImplementationRAM();

    // PatternIndex API
    void index(const KnowledgeBuildingBlock &kbb, const Handle &toplevel);
    void index(const KnowledgeBuildingBlock &kbb, KBB_UUID id);
    void query(std::list<Handle> &answer, const KnowledgeBuildingBlock &key, bool localOnly = true);
    void query(std::list<KBB_UUID> &answer, const KnowledgeBuildingBlock &key, bool externOnly = true);

private:

    const bool DEBUG = false;

    class IndexNode
    {
        public:

            // Type and Arity are defined in Atom.h
            typedef std::pair<Type, Arity> IndexLinkTag;
            struct LessThan {
                bool operator()(const IndexLinkTag &a, const IndexLinkTag &b) const {
                    if (a.first < b.first) return true;
                    if (a.first > b.first) return false;
                    if (a.second < b.second) return true;
                    return false;
                }
            };
            typedef std::map<IndexLinkTag, IndexNode *, IndexNode::LessThan> TagToIndexNodeMap;

            TagToIndexNodeMap children;
            KBB_DBID kbbID;

            IndexNode();
            ~IndexNode();
    };

    // TODO Replace by Apache Ignite
    std::map<KBB_DBID, std::list<Handle> *> occurrencesLocal;
    std::map<KBB_DBID, std::list<KBB_UUID> *> occurrencesGlobal;

    IndexNode rootLocal;
    IndexNode rootGlobal;
    KBB_DBID nextKBBID;
    std::list<IndexNode *> patternMatched;

    void insertKBBOccurrence(const Handle &toplevel, const KBB_DBID &dbid);
    void insertKBBOccurrence(KBB_UUID uuid, const KBB_DBID &dbid);
    IndexNode *findIndexNode(const KnowledgeBuildingBlock &kbb, IndexNode *root, bool insertFlag);
    void traverse(IndexNode *node);
};

}

#endif // _OPENCOG_PATTERNINDEXIMPLEMENTATIONRAM_H
