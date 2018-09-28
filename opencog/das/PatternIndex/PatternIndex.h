/*
 * Copyright (C) 2018 OpenCog Foundation
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

#ifndef _OPENCOG_PATTERNINDEX_H
#define _OPENCOG_PATTERNINDEX_H

#include "KnowledgeBuildingBlock.h"
#include "../utils.h"
#include <opencog/atoms/base/Atom.h>
#include <list>

namespace opencog
{

    // TODO: fix this
    /*
    union KBBReference {
        Handle handle;
        KBB_UUID uuid;

        KBBReference() {uuid = 0;}
        KBBReference(const KBBReference &other) {uuid = other.uuid;}
        ~KBBReference() {}
        KBBReference& operator=(const KBBReference &other) {return (KBBReference &) (uuid = other.uuid);}
        bool operator==(const KBBReference &other) {return uuid == other.uuid;}
    };
    */
    //typedef Handle KBBReference;

/**
 *
 */
class PatternIndex 
{

public:

    static PatternIndex *factory();
    virtual ~PatternIndex();

    // Public API implemented by subclasses
    virtual void index(const KnowledgeBuildingBlock &kbb, const Handle &toplevel) = 0;
    virtual void index(const KnowledgeBuildingBlock &kbb, KBB_UUID id) = 0;
    // TODO: change query() interface to something more flexible
    virtual void query(std::list<Handle> &answer, const KnowledgeBuildingBlock &key, bool localOnly = true) = 0;
    virtual void query(std::list<KBB_UUID> &answer, const KnowledgeBuildingBlock &key, bool externOnly = true) = 0;

};

}

#endif // _OPENCOG_PATTERNINDEX_H
