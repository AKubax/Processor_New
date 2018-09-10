#include "../AKTools/akio.h"
#include <stdexcept>


#undef DEBUG_LOG_MODE
#define DEBUG_LOG_MODE 1


#define PROC_BUF_SIZE 1024

#undef DEBUG_LOG_MODE
#define DEBUG_LOG_MODE 1

using byte = unsigned char;

class Processor{
private:

    byte buf_[PROC_BUF_SIZE] = {};
    size_t progSize_ = 0;

public:
    int load(const char* inp, size_t count);

    int execute();
};


int Processor::load(const char* inp, size_t count){
//    return printf("TODO load func. inp is \"%s\"\n", inp), 0;
    progSize_ = count;

    memcpy(buf_, inp, count);
    return 0;
}

int Processor::execute(){
//    return printf("TODO execute func\n");

    byte* const startPtr = &(buf_[0]);
    byte* curPtr = startPtr;

    for(;curPtr - startPtr < progSize_;){
        byte curCmd = *curPtr;

        printd("curPtr = 0x%p; buf_ = 0x%p\n", curPtr, buf_);

        #define DEF_CMD(num, name, compCode, procCode) else if( (printd("num = %d; curCmd = %d\n", num, curCmd), num == curCmd) ) { procCode; }

        if(0);
        #include "Commands.h"
        #undef DEF_CMD
        else throw std::runtime_error("Cmd does not exist");
    }
    return 0;
}
