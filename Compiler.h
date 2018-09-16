#include "../AKTools/akio.h"
#include "AKTokenizer.h"



class Compiler{
private:

public:

    void compile(std::vector<Token> inp, FILE* outF);
};


void Compiler::compile(std::vector<Token> inp, FILE* outF){
    Token* curPtr = inp.data();

    for(; curPtr - inp.data() < inp.size() ;){
        if(!curPtr[0].isStr) return printf("Invalid cmd. Cmd can't be a number\n"), 3;

        #define DEF_CMD(num, name, compCode, procCode)
    }
}
