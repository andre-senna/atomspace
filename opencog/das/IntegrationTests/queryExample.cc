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

#include "../DistributedAtomSpace.h"

using namespace opencog;
using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <SCM file>\n", argv[0]);
        return 1;
    }

    DistributedAtomSpace das;
    das.loadSCM(argv[1]);

    std::string queryStr1 = "(AndLink\
                               (SimilarityLink\
                                 (VariableNode \"X\")\
                                 (VariableNode \"Y\")\
                               )\
                               (SimilarityLink\
                                 (VariableNode \"Y\")\
                                 (VariableNode \"Z\")\
                               )\
                             )";

    KnowledgeBuildingBlock pattern1;

    return 0;
}
