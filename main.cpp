#include <io.h>
#include "../AKTools/akio.h"

#include "Processor.h"


int main(int argc, const char* argv[]){
    if(argc - 1 < 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("TODO ERROR MESSAGE"), 2;

    std::pair<const char*, size_t> lftbRes = loadFileToBuf(f);

    const char* programm = lftbRes.first;
    fclose(f);

    Processor proc;
    proc.load(programm, lftbRes.second);
    proc.execute();
}
