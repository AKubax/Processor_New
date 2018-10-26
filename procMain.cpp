//#include <TXLib.h>

#include "../AKTools/akio.h"
//#include "../AKTools/AKString.h"
//#include <string>

FILE* DebugLogPathFile = 0;
bool DebugLogModeVar = 0;
#undef DEBUG_LOG_MODE
#define DEBUG_LOG_MODE DebugLogModeVar
#undef DEBUG_LOG_PATH
#define DEBUG_LOG_PATH DebugLogPathFile



#include "Processor.h"


int main(int argc, const char* argv[]){
    DebugLogModeVar = argc - 1 >= 3;
    DebugLogPathFile = stdout; // fopen("processorLog.log", "w");

    printd("DebugLogModeVar = %d; DEBUG_LOG_MODE = %d; argv [%d]:\n", DebugLogModeVar, DEBUG_LOG_MODE, argc);
    for(int i = 0; i < argc; i++){
        printd("    %d: \"%s\"\n", i, argv[i]);
    }
    printd("\n");

    if(argc - 1 < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;

    if(argc - 1 >= 3){
        printd("Everything will be logged into \"%s\"\n", argv[3]);
        DebugLogPathFile = fopen(argv[3], "w");
    }

//    std::string logFileName = argv[1];
//    logFileName.change(logFileName.find('.') - logFileName.data(), logFileName.size(), ".log");
//    printd("%s\n", logFileName.c_str());

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("File containing programm to execute could not have been able to be opened\n"), 2;

    std::pair<const char*, size_t> wftbRes = writeFileToBuf(f);

    printd("Length of programm read is %llu\n", wftbRes.second);

    const char* programm = wftbRes.first;
    printd("Loaded from file. buf = \"%s\"\n", programm);
    fclose(f);

    printd("Starting proc:\n\n");

    Processor proc(argv[2]);
    proc.load(programm, wftbRes.second);
    proc.execute();
}
