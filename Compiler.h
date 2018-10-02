#include "../AKTools/akio.h"
#include "../AKTools/AKTokenizer.h"

#include <math.h>



int compile(std::vector<Token> inp, FILE* outFile);
int compile(std::vector<Token> inp, FILE* outFile){
    Token* curPtr = inp.data();

    std::vector<char> outBuf;

    for(; curPtr - inp.data() < (ptrdiff_t) inp.size() ;){
        if(!curPtr[0].isStr) return printf("Invalid cmd (ñmd can't be a number)\n"), 3;

        #define DEF_CMD(num, name, compCode, procCode) else if (strcmp(#name, curPtr[0].value.str)) { compCode; }

        if(0);
        #include "Commands.h"

        else return printf("Invalid cmd. No such cmd = \"%s\"\n", curPtr[0].value.str), 3;
    }

    writeBufToFile(outBuf.data(), outBuf.size(), outFile);
    return 0;
}
