#include "../AKTools/akio.h"
#include "../AKTools/AKTokenizer.h"

#include "Compiler.h"

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

    if(argc - 1 < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("File containing program to compile could not have been opened\n"), 2;

    const char* program = writeFileToBuf(f);
    fclose(f);

    std::vector<Token> tokens = tokenize(program);


    FILE* outF = fopen(argv[2], "wb");
    if (!outF)  return printf("The destination file could not have been opened\n"), 2;

    Compiler comp;
    comp.compile(tokens, outF);

    fclose(outF);
}


