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

#ifndef _OPENCOG_PATTERNINDEXBUILDER_H
#define _OPENCOG_PATTERNINDEXBUILDER_H

#include "SCMLoaderCallback.h"
#include "PatternIndex/PatternIndex.h"
#include "CompoundHashValue.h"

namespace opencog
{

class PatternIndexSCMBuilder : public SCMLoaderCallback
{

public:

    PatternIndexSCMBuilder(PatternIndex *index);
    ~PatternIndexSCMBuilder();

    bool DEBUG;

    // SCMLoaderCallback interface
    void beforeInserting(const std::string &schemeStr);
    void afterInserting(const Handle &toplevelAtom);

private:

    PatternIndex *index;
    KnowledgeBuildingBlock kbb;
    CompoundHashValue kbbHashValue;

    void parseSCMFragment(const std::string &str);
    int countTargets(const std::string &txt, unsigned int begin);
    int recursiveParse(const std::string &txt,
                       KnowledgeBuildingBlock &kbb,
                       CompoundHashValue &kbbHashValue,
                       unsigned int begin);
};

}

#endif // _OPENCOG_PATTERNINDEXBUILDER_H
