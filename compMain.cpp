#include "../AKTools/akio.h"

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

    if(argc - 1 < 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) return printf("TODO HELP"), 1;

    FILE* f = fopen(argv[1], "rb");
    if(!f) return printf("File containing program to compile could not have been opened\n"), 2;

    const char* program = writeFileToBuf(f);
    fclose(f);

    Token* tokens = tokenize(program);

    Compiler comp;
    comp.compile(tokens);
}


