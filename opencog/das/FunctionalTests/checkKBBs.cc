/*
 * checkKBBs.cc
 *
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

#include <opencog/guile/SchemeEval.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/base/Handle.h>
#include <opencog/util/Config.h>

#include "../PatternIndex/PatternIndex.h"
#include "PatternIndexBuilder.h"
#include "SCMLoader.h"

using namespace opencog;
using namespace std;

static PatternIndex *patternIndex;
static AtomSpace *atomSpace;

void createIndex(const string &scmPath)
{

    PatternIndexBuilder builder(patternIndex);

    if (SCMLoader::load(scmPath, atomSpace, &builder)) {
        string m = "Error creating PatternIndex. SCM file is invalid.\n";
        throw runtime_error(m);
    }
}

int main(int argc, char *argv[]) {

    int exitValue = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <SCM file>\n", argv[0]);
        exitValue = 1;
    } else {

        // AtomSpace setup
        atomSpace = new AtomSpace();
        SchemeEval::init_scheme();
        SchemeEval::set_scheme_as(atomSpace);
        //SchemeEval *schemeEval = new SchemeEval(atomSpace);
        
        // Create a new index given a SCM file
        patternIndex = PatternIndex::factory();
        createIndex(argv[1]);
        printf("Index created\n");
    }

    return exitValue;
}
