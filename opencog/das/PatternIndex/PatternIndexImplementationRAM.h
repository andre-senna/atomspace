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

    void index(const KnowledgeBuildingBlock &kbb, const KBBReference &toplevel);
    void query(std::list<KBBReference> &answer, const KnowledgeBuildingBlock &key);

private:

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
    std::map<KBB_DBID, std::list<KBBReference> *> occurrences;

    IndexNode root;
    KBB_DBID nextKBBID;

    void insertKBBOccurrence(const KBBReference &toplevelKBBReference, const KBB_DBID &kbbID);
};

}

#endif // _OPENCOG_PATTERNINDEXIMPLEMENTATIONRAM_H
