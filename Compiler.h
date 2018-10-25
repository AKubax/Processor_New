//#include "../AKTools/akio.h"
//#include "../AKTools/AKTokenizer.h"

#include <math.h>
#include <map>
#include <algorithm>

using byte = unsigned char;

#define DEF_REG(num, name) name = num,

enum REG_NAMES{
                #include "REGs.h"
                LAST_EL_REGs_NUM
              };
#undef DEF_REG

const byte REGs_NUM = LAST_EL_REGs_NUM;

byte regToNum(const char* reg);
byte regToNum(const char* reg){
    #define DEF_REG(num, name) else if(strcmp(reg, #name) == 0){ printd("Found REG '%s' == %d\n", #name, num); return num; }

    if(0);
    #include "REGs.h"
    #undef DEF_REG

    printf("REG with name '%s' not found\n", reg);
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
