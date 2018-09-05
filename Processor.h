

#define PROC_BUF_SIZE 1024

using byte = unsigned char;

class Processor{
private:

    byte buf_[PROC_BUF_SIZE];

public:
    int load(const char* inp, size_t count);

    int execute();
};


int Processor::load(const char* inp, size_t count){
//    return printf("TODO load func. inp is \"%s\"\n", inp), 0;

    memcpy(buf_, inp, count);
    return 0;
}

int Processor::execute(){
//    return printf("TODO execute func\n");

    byte* curPtr = &(buf_[0]);
    for(;;){
        byte curCmd = *curPtr;

        printf("curPtr = 0x%p; buf_ = 0x%p\n", curPtr, buf_);

        #define DEF_CMD(num, name, compCode, procCode) else if((printf("num = %d; curCmd = %d\n", num, curCmd), num == curCmd)) { procCode; }

        if(0);
        #include "Commands.h"
        #undef DEF_CMD
    }
    return 0;
}
