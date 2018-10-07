#include "../AKTools/akio.h"
#include "../AKTools/AKTokenizer.h"

#include "Compiler.h"

void kostyl();
void kostyl(){
    char c[2] = {1, 0};
    FILE* f = fopen("./noCode/proc.akprc", "wb");
    writeBufToFile(c, 2, f);
    fclose(f);
}

int main(int argc, const char* argv[]){
    printd("argv [%d]:\n", argc);
    for(int i = 0; i < argc; i++){
        printd("    %d: \"%s\"\n", i, argv[i]);
    }
    printd("\n");

    if(argc - 1 < 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;  //TODO NORMAL LAUNCH

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("File containing program to compile could not have been opened\n"), 2;

    std::pair<const char*, size_t> wftbRes = writeFileToBuf(f);
    fclose(f);

    char* fileBuf = (char*) calloc(wftbRes.second, 1);
    memcpy(fileBuf, wftbRes.first, wftbRes.second);

    std::vector<Token> tokens = tokenize(fileBuf);

    FILE* outF = fopen(/*argv[2]*/"test.akprc", "wb");
    if (!outF)  return printf("The destination file could not have been opened\n"), 2;

    compile(tokens, outF);

    fclose(outF);
}


