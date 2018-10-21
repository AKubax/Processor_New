#include "../AKTools/akio.h"
#include "../AKTools/AKTokenizer.h"

#include <math.h>
#include <map>
#include <algorithm>

using byte = unsigned char;

const size_t REG_NUM = 5;
const char* REG_NAMES[REG_NUM] = { "DEL", "RAX", "RBX", "RCX", "RDX" };

byte regToNum(const char* reg);
byte regToNum(const char* reg){
    for(size_t i = 0; i < REG_NUM; i++) if(strcmp(reg, REG_NAMES[i]) == 0) return i;

    throw std::runtime_error("Reg with name such as arg to regToNum does not exist\n");
    return -1;
}

std::map<const char*, size_t> makeLabels(std::vector<Token> inp);
std::map<const char*, size_t> makeLabels(std::vector<Token> inp){

    Token* curPtr = inp.data();

    std::vector<byte> outBuf;
    std::map<const char*, size_t> labels;// = makeLabels(inp);

    for(; curPtr - inp.data() < (ptrdiff_t) inp.size() ;){
//        if(curPtr[0].isStr) printd("New iteration of compiling loop. Token = '%s'\n", curPtr[0].value.str);
//        else                printd("!!!TOKEN IS NOT A STRING!!!\n#New iteration of compiling loop. Token = '%lf'\n", curPtr[0].value.dbl);

        if(!curPtr[0].isStr) return printf("Invalid cmd (cmd can't be a number)\n"), std::map<const char*, size_t>();

        if(curPtr[0].value.str[0] == ':'){
            labels[curPtr[0].value.str] = outBuf.size();

            printd("Found label '%s' [%u]\n", curPtr[0].value.str, outBuf.size());

            curPtr++;
            continue;
        }

        #define DEF_CMD(num, name, compCode, procCode) else if (strcmp(#name, curPtr[0].value.str) == 0) { /*printd("Got match. CMD = '%s'\n", #name);*/ {compCode;}; }

        if(0);
        #include "Commands.h"
        #undef DEF_CMD

        else return printf("Invalid cmd. No such cmd = \"%s\"\n", curPtr[0].value.str), std::map<const char*, size_t>();
    }

    return labels;
}

int compile(std::vector<Token> inp, FILE* outFile);
int compile(std::vector<Token> inp, FILE* outFile){

    printd("\n\n---\n#Compile func start\n#inp = {");
    for(auto& el : inp) el.isStr? printd(" '%s',", el.value.str) : printd(" %lf,", el.value.dbl);
    printd("}\n");


    Token* curPtr = inp.data();

    std::vector<byte> outBuf;
    std::map<const char*, size_t> labels = makeLabels(inp);

    for(; curPtr - inp.data() < (ptrdiff_t) inp.size() ;){
        if(curPtr[0].isStr) printd("New iteration of compiling loop. Token = '%s'\n", curPtr[0].value.str);
        else                printd("!!!TOKEN IS NOT A STRING!!!\n#New iteration of compiling loop. Token = '%lf'\n", curPtr[0].value.dbl);

        if(!curPtr[0].isStr) return printf("Invalid cmd (cmd can't be a number)\n"), 3;

        if(curPtr[0].value.str[0] == ':'){
            curPtr++;
            continue;
        }

        #define DEF_CMD(num, name, compCode, procCode) else if (strcmp(#name, curPtr[0].value.str) == 0) { printd("Got match. CMD = '%s'\n", #name); {compCode;}; }

        if(0);
        #include "Commands.h"
        #undef DEF_CMD

        else return printf("Invalid cmd. No such cmd = \"%s\"\n", curPtr[0].value.str), 3;
    }

    writeBufToFile( ((const char*) outBuf.data()), outBuf.size(), outFile );

    printd("Compile func end\n---\n\n");
    return 0;
}
