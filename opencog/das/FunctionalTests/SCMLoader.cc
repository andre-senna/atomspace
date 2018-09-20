#include "SCMLoader.h"
#include <opencog/guile/SchemeEval.h>
#include <opencog/util/Logger.h>


using namespace opencog;
using namespace std;

bool SCMLoader::load(const string &fileName, 
                     AtomSpace *atomSpace, 
                     SCMLoaderCallback *listener)
{
    bool exitValue = false;

    logger().set_sync_flag(true);
    fstream fin(fileName, fstream::in);
    if (fin.good()) {
        ifstream fcount(fileName, ios::binary | ios::ate);
        // Just to provide log message with % done
        int fileSize = fcount.tellg();
        fcount.close();
        logger().info("[PatternIndex] Loading file: \"%s\"", fileName.c_str());
        parseFile(fin, atomSpace, fileSize, listener);
        logger().info("[PatternIndex] Loading file 100%% done");
        logger().info("[PatternIndex] Loading finished");
        fin.close();
    } else {
        logger().error("Could not open file: \"%s\"", fileName.c_str());
        exitValue = true;
    }

    return exitValue;
}

void SCMLoader::parseFile(fstream &fin, 
                          AtomSpace *atomSpace, 
                          int inputFileSize, 
                          SCMLoaderCallback *listener)
{
    SchemeEval *schemeEval = SchemeEval::get_evaluator(atomSpace);
    int percentDone = 0;
    int inputFileCharCount = 0;
    int level = 0;
    bool inNodeName = false;
    bool inComment = false;
    char c;
    string line = "";
    string output1 = schemeEval->eval("(count-all)");
    logger().info("[PatternIndex] Atom count before loading: %s", output1.c_str());
    while (fin >> noskipws >> c) {
        inputFileCharCount++;
        if (c == '\n') {
            if (inputFileSize > 0) {
                // Logs current % done
                int n = (((float) inputFileCharCount) / inputFileSize) * 100;
                if ((n > percentDone) && (n < 100)) {
                    percentDone = n;
                    logger().info("[PatternIndex] Loading file %d%% done", percentDone);
                }
            }
            inComment = false;
        } else {
            if (((! inNodeName) && (c == ';')) || inComment) {
                inComment = true;
            } else {
                if ((line.length() != 0) || (c != ' ')) {
                    line += c;
                    if (inNodeName) {
                        if (c == '\"') inNodeName = false;
                    } else {
                        if (c == '\"') inNodeName = true;
                        if (c == '(') level++;
                        if (c == ')') {
                            if (--level == 0) {
                                if (listener != NULL) listener->beforeInserting(line);
                                Handle h = schemeEval->eval_h(line);
                                if (listener != NULL) listener->afterInserting(h);
                                line = "";
                            }
                        }
                    }
                }
            }
        }
    }
    string output2 = schemeEval->eval("(count-all)");
    logger().info("[PatternIndex] Atom count after loading: %s", output2.c_str());
}
