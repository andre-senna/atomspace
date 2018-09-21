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

#ifndef _OPENCOG_PATTERNINDEX_H
#define _OPENCOG_PATTERNINDEX_H

#include "KnowledgeBuildingBlock.h"
#include "../utils.h"
#include <opencog/atoms/base/Atom.h>
#include <list>

namespace opencog
{

    union KBBReference {
        const Handle *handle;
        KBB_UUID uuid;

        KBBReference() {uuid = 0;}
        KBBReference(const KBBReference &other) {uuid = other.uuid;}
        ~KBBReference() {}
        KBBReference& operator=(const KBBReference &other) {return (KBBReference &) (uuid = other.uuid);}
        bool operator==(const KBBReference &other) {return uuid == other.uuid;}
    };

/**
 *
 */
class PatternIndex 
{

public:

    static PatternIndex *factory();
    virtual ~PatternIndex();

    // Public API implemented by subclasses
    virtual void index(KnowledgeBuildingBlock *kbb, KBBReference &toplevel) = 0;
    // TODO: change query() interface to something more flexible
    virtual void query(std::list<KBBReference> &answer, KnowledgeBuildingBlock *key) = 0;

};

}

#endif // _OPENCOG_PATTERNINDEX_H
