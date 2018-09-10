#include <TXLib.h>

#include "../AKTools/akio.h"
#include "../AKTools/AKString.h"

#include "Processor.h"


#undef DEBUG_LOG_MODE
#define DEBUG_LOG_MODE 1

int main(int argc, const char* argv[]){
    printd("argv [%d]:\n", argc);
    for(int i = 0; i < argc; i++){
        printd("    %d: \"%s\"\n", i, argv[i]);
    }
    printd("\n");

    if(argc - 1 < 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;

    AKString logFileName = argv[1];
    logFileName.change(logFileName.find('.') - logFileName.data(), logFileName.size(), ".log");
    printd("%s\n", logFileName.c_str());

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("File containing programm to execute could not have been able to be opened\n"), 2;

    std::pair<const char*, size_t> wftbRes = writeFileToBuf(f);

    printd("Length of programm read is %llu\n", wftbRes.second);

    const char* programm = wftbRes.first;
    printd("Loaded from file. buf = \"%s\"\n", programm);
    fclose(f);

    printd("Starting proc:\n\n");

    Processor proc;
    proc.load(programm, wftbRes.second);
    proc.execute();
}
