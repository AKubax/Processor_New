#include <stdexcept>
#include <math.h>
#include <stack>


const size_t PROC_BUF_SIZE = 8192;
const size_t PROC_STACK_SIZE = 32;

using byte = unsigned char;


#define DEF_REG(num, name) name = num,

enum REG_NAMES{
                #include "REGs.h"
                LAST_EL_REGs_NUM
              };
#undef DEF_REG

const byte REGs_NUM = LAST_EL_REGs_NUM;



class Processor{
private:
    FILE* outStream;

    double REGs[REGs_NUM] = {};

    std::stack<unsigned short> retStack;

    struct ProcStack{
        double* startPtr;

        size_t curSize;
        const size_t maxSize;

        ProcStack(byte* buf):
            startPtr ((double*) buf),
            curSize (0),
            maxSize (PROC_STACK_SIZE)
        {}

        void push(double val){
            startPtr[curSize] = val;
            curSize++;

            if(curSize == maxSize - 1)  throw std::runtime_error("Processor stack overflow");
        }

        double pop(){
            return startPtr[--curSize];
        }


        void dump(){
            printd("---\n");
            printd("Processor stack dump:\n[");
            for(double* ptr = startPtr; ptr < startPtr + curSize; ptr++) printd("%lf ", ptr[0]);
            printd("]\n\n");
        }
    };

    byte buf_[PROC_BUF_SIZE] = {};
    size_t progSize_ = 0;

public:
    Processor(const char* outS);

    int load(const char* inp, size_t count);

    int execute();
};

Processor::Processor(const char* outS):
    outStream ( fopen(outS, "w") )
{}

int Processor::load(const char* inp, size_t count){
//    return printf("TODO load func. inp is \"%s\"\n", inp), 0;
    progSize_ = count;

    memcpy(buf_, inp, count);
    return 0;
}

int Processor::execute(){
//    return printf("TODO execute func\n");
    ProcStack theStack(buf_ + progSize_);
    double* RAM = (double*) buf_ + progSize_ + PROC_STACK_SIZE;

    byte* const startPtr = &(buf_[0]);
    byte* curPtr = startPtr;

    for(;curPtr - startPtr < progSize_;){
        byte curCmd = *curPtr;

        printd("\ncurPtr = 0x%p; buf_ = 0x%p\n", curPtr, buf_);

        #define DEF_CMD(num, name, compCode, procCode) else if( (printd("numCmd = %d; curCmd = %d\n", num, curCmd), num == curCmd) ) { procCode; }

        if(0);
        #include "Commands.h"
        #undef DEF_CMD
        else throw std::runtime_error("Cmd does not exist");

        theStack.dump();
    }
    return 0;
}
