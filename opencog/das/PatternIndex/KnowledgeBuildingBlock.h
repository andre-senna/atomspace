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

#ifndef _OPENCOG_KNOWLEDGEBUILDINGBLOCK_H
#define _OPENCOG_KNOWLEDGEBUILDINGBLOCK_H

#include <string>
#include <opencog/atoms/base/Atom.h>
#include "../utils.h"
#include "../CompoundHashValue.h"

class PatternIndexImplementationRAMUTest; // to declare as friend (see below)

namespace opencog
{

/**
 *
 */
class KnowledgeBuildingBlock 
{

// uses "package" visible methods (see below)
friend class PatternIndexImplementationRAM;
friend class PatternIndexSCMBuilder;
friend class ::PatternIndexImplementationRAMUTest;

private:

    // Type and Arity are defined in Atom.h
    typedef struct {
        Type type;
        Arity arity;
        // TODO Is atomHash needed?
        KBB_HASHCODE atomHash;
    } KBBElement;

    static const Type ANY_KBB;
    static const Type ANY_TYPE;
    static const Arity ANY_ARITY;
    std::vector<KBBElement> definition;


    int countTargets(const std::string &txt, unsigned int begin);
    int recursiveParse(const std::string &txt,
                       CompoundHashValue &kbbHashValue,
                       unsigned int begin);
    bool recursiveMatches(unsigned int cursor, const Handle &handle) const;


    // "package" visibility
    // the following methods are accesed by "friend" classes 

    inline Type getTypeAt(unsigned short int pos) const
    {
        return definition[pos].type;
    }
    inline Arity getArityAt(unsigned short int pos) const
    {
        return definition[pos].arity;
    }
    inline KBB_HASHCODE getAtomIDAt(unsigned short int pos) const
    {
        return definition[pos].atomHash;
    }
    void pushBack(const KnowledgeBuildingBlock &kbb);
    void pushBack(Type type, Arity arity, KBB_HASHCODE atomHash = 0);
    void pushFront(Type type, Arity arity, KBB_HASHCODE atomHash = 0);

public:

    KnowledgeBuildingBlock();
    ~KnowledgeBuildingBlock();

    inline unsigned short int size() const { return definition.size(); }
    inline void clear() { definition.clear(); }

    void parseSCM(const std::string &str);
    bool matches(const Handle &handle) const;

    void printForDebug(const std::string &prefix, const std::string &sufix) const;

};

}

#endif // _OPENCOG_KNOWLEDGEBUILDINGBLOCK_H
