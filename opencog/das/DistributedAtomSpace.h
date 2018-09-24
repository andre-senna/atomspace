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

#ifndef _OPENCOG_DISTRIBUTEDATOMSPACE_H
#define _OPENCOG_DISTRIBUTEDATOMSPACE_H

#include <opencog/atomspace/AtomSpace.h>
#include "PatternIndex/PatternIndex.h"
#include "CacheManager.h"
#include <string>

namespace opencog
{

/**
 *
 */
class DistributedAtomSpace 
{

public:

    DistributedAtomSpace();
    ~DistributedAtomSpace();

    void loadSCM(const std::string &fileName);

    // Prevent accidental copies
    // It's public to provide better error messages
    DistributedAtomSpace(const DistributedAtomSpace&) = delete;
    DistributedAtomSpace& operator=(const DistributedAtomSpace&) = delete;

private:

    PatternIndex* globalPatternIndex;
    PatternIndex* localPatternIndex;
    CacheManager* cacheManager;
    AtomSpace* atomSpace;

};

}

#endif // _OPENCOG_DISTRIBUTEDATOMSPACE_H
