#include "../AKTools/akio.h"
#include "../AKTools/AKTokenizer.h"

FILE* DebugLogPathFile = 0;
bool DebugLogModeVar = 0;
#undef DEBUG_LOG_MODE
#define DEBUG_LOG_MODE DebugLogModeVar

#undef DEBUG_LOG_PATH
#define DEBUG_LOG_PATH DebugLogPathFile

#include "Compiler.h"


int main(int argc, const char* argv[]){
    bool DebugLogModeVar = 1;//argc - 1 >= 3;
    DebugLogPathFile = stdout;

    printd("argv [%d]:\n", argc);
    for(int i = 0; i < argc; i++){
        printd("    %d: \"%s\"\n", i, argv[i]);
    }
    printd("\n");

    if(argc - 1 < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;

    if(argc - 1 >= 3){
        printd("Everything will be logged into \"%s\"\n", argv[3]);
        DebugLogPathFile = fopen(argv[3], "w");
    }

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("File containing program to compile could not have been opened\n"), 2;

    std::pair<const char*, size_t> wftbRes = writeFileToBuf(f);
    fclose(f);

    char* fileBuf = (char*) calloc(wftbRes.second, 1);
    memcpy(fileBuf, wftbRes.first, wftbRes.second);

    std::vector<Token> tokens = tokenize(fileBuf);

    FILE* outF = fopen(argv[2], "wb");
    if (!outF)  return printf("The destination file could not have been opened\n"), 2;

    compile(tokens, outF);

    fclose(outF);
}


