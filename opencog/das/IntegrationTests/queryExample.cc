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
#include "../AtomeseParser/AtomeseParser.h"

using namespace opencog;
using namespace std;

void parseString(const char *s);

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <SCM file>\n", argv[0]);
        return 1;
    }

    DistributedAtomSpace das;
    das.loadSCM(argv[1]);

    std::string queryStr[100];
    unsigned int count = 0;

    queryStr[count++] = "(SimilarityLink\
                            (ConceptNode \"snake\")\
                            (ConceptNode \"vine\")\
                         )";
    queryStr[count++] = "(SimilarityLink\
                            (ConceptNode \"vine\")\
                            (ConceptNode \"snake\")\
                         )";
    queryStr[count++] = "(SimilarityLink\
                            (ConceptNode *)\
                            (ConceptNode \"snake\")\
                         )";
    queryStr[count++] = "(SimilarityLink\
                            (NumberNode *)\
                            (ConceptNode \"snake\")\
                         )";
    queryStr[count++] = "(SimilarityLink\
                            (*)\
                            (ConceptNode \"snake\")\
                         )";
    queryStr[count++] = "(SimilarityLink\
                            (ConceptNode \"snake\")\
                            (*)\
                         )";
    queryStr[count++] = "(InheritanceLink\
                            (*)\
                            (ConceptNode \"mammal\")\
                         )";
    queryStr[count++] = "(InheritanceLink\
                            (ConceptNode \"mammal\")\
                            (*)\
                         )";
    queryStr[count++] = "(SimilarityLink\
                            (*)\
                            (*)\
                         )";
    queryStr[count++] = "(InheritanceLink\
                            (*)\
                            (*)\
                         )";
    queryStr[count++] = "(AndLink\
                           (SimilarityLink\
                             (VariableNode \"X\")\
                             (VariableNode \"Y\")\
                           )\
                           (SimilarityLink\
                             (VariableNode \"Y\")\
                             (VariableNode \"Z\")\
                           )\
                         )";

    KnowledgeBuildingBlock pattern;
    list<Handle> queryAnswer;
    for (unsigned int i = 0; i < count; i++) {
        //pattern.parseSCM(queryStr[i]);
        AtomeseParser::parseString(pattern, queryStr[i]);
        queryAnswer.clear();
        das.getLocal(queryAnswer, pattern);
        printf("Query %u:\n", i);
        pattern.printForDebug("Pattern: ", "\n");
        //printf("Query %u: %s\n", i, queryStr[i].c_str());
        printf("%lu matches\n", queryAnswer.size());
        for (Handle &handle : queryAnswer) {
            printf("%s\n", handle->to_string().c_str());
            //printf("%p\n", handle.atom_ptr());
        }
    }

    return 0;
}
