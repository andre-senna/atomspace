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

#ifndef _OPENCOG_ATOMESEPARSER_H
#define _OPENCOG_ATOMESEPARSER_H

#include "../PatternIndex/KnowledgeBuildingBlock.h"
#include <opencog/atoms/base/Atom.h>
#include <string>
#include <list>

namespace opencog
{

/**
 *
 */
class AtomeseParser 
{

public:

    AtomeseParser(KnowledgeBuildingBlock &kbb);
    AtomeseParser(std::list<KnowledgeBuildingBlock *> &kbbList);
    ~AtomeseParser();

    static void parseString(KnowledgeBuildingBlock &kbb,
                            const std::string &txt);
    static void parseFile(std::list<KnowledgeBuildingBlock *> &kbbList, const std::string &fname);

    // The following methods are called by Bison during parsing.
    // They are not actually part of the public API.

    void addNewNode(Type t, char *name);
    void startNewLink(Type t);
    void addNewLink();
      
private:

    static const bool DEBUG;

    const unsigned int SINGLE_KBB = 1;
    const unsigned int KBB_LIST = 2;
    unsigned int MODE;
    std::list<KnowledgeBuildingBlock *> *currentKBBList;
    KnowledgeBuildingBlock *currentKBB;
    CompoundHashValue hashValue;
    std::list<std::vector<KBB_HASHCODE>> targetHashValuesStack;
    std::list<KnowledgeBuildingBlock *> kbbStack;
    std::list<Arity> arityStack;
    std::list<Type> typeStack;
    Arity currentArity;
};

}

#endif // _OPENCOG_ATOMESEPARSER_H
