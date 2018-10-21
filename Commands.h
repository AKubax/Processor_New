#define EMIT_BYTE(val)   outBuf.push_back( (val) );
#define EMIT_SHORT(val)  outBuf.resize(outBuf.size() + sizeof( short)); ((short*)  (outBuf.data() + outBuf.size() - sizeof( short)))[0] = (val);
#define EMIT_USHORT(val) outBuf.resize(outBuf.size() + sizeof(unsigned short)); ((unsigned short*)  (outBuf.data() + outBuf.size() - sizeof(unsigned short)))[0] = (val);
#define EMIT_DBL(val)    outBuf.resize(outBuf.size() + sizeof(double)); ((double*) (outBuf.data() + outBuf.size() - sizeof(double)))[0] = (val);

#define DBL_PTR(ptr)    ((double*) ptr)


#define PP_BIT_s 1
#define PP_BIT_r 2
#define PP_BIT_m 4


#define PROC_PUSH_FUNC()  {                                                                               \
                            curPtr++;                                                                   \
                            byte mode = *curPtr;                                                        \
                            curPtr++;                                                                   \
                                                                                                        \
                            switch(mode){                                                                \
                                case 0:                                                                  \
                                    double val = DBL_PTR(curPtr)[0];                                     \
                                    theStack.push(val);                                                  \
                                                                                                         \
                                    break;                                                               \
                                default:                                                                 \
                                    printf("TODO: This push mode is yet to be done\n");                  \
                            }                                                                            \
                        }

#define COMP_PUSH_FUNC()  {                                                                        \
                            curPtr++;                                                              \
                                                                                                   \
                            EMIT_BYTE(7);                                                          \
                                                                                                   \
                            if(curPtr[0].isStr) printf("STR CASE OF PUSH IS YET TO BE DONE\n");    \
                            else                EMIT_DBL(curPtr[0].value.dbl);                     \
                                                                                                   \
                            curPtr++;                                                              \
                          }

// POP/PUSH :
//        00000 mrs   POP                  | PUSH
//
//        00000 001   -----                | PUSH {n}
//        00000 010   POP {DEL/R_X}        | PUSH {R_X}
//        00000 100   POP [{n}]            | PUSH [{n}]
//        00000 110   POP [{R_X}+{n}]      | PUSH [{R_X}+{n}]


#define COMP_POP_FUNC()   {                                                                                                                                 \
                            curPtr++;                                                                                                                       \
                                                                                                                                                            \
                            printd("Compiler POP func. Token == '%s'\n", curPtr[0].value.str);                                                              \
                                                                                                                                                            \
                            if(0);                                                                                                                          \
                            else if(curPtr[0].value.str[0] == '['){                                                                                         \
                                size_t len = strlen(curPtr[0].value.str);                                                                                   \
                                if( (curPtr[0].value.str + len - 1)[0] != ']' ) throw std::runtime_error("Unclosed parenthesis or space in POP argument\n");\
                                                                                                                                                            \
                                size_t inLen = len - 2;                                                                                                     \
                                const char* inPtr = curPtr[0].value.str + 1;                                                                                \
                                                                                                                                                            \
                                const char* findRes;                                                                                                        \
                                if((findRes = std::find(inPtr, inPtr + inLen, '+')) != inPtr + inLen){                                                      \
                                    EMIT_BYTE(8);                                                                                                           \
                                    EMIT_BYTE(PP_BIT_m | PP_BIT_r);                                                                                         \
                                                                                                                                                            \
                                    char* sNum = (char*) calloc(inPtr + inLen - findRes + 1, 1);                                                            \
                                    strncpy(sNum, findRes, inPtr + inLen - findRes);                                                                        \
                                    sNum[inPtr + inLen - findRes] = '\0';                                                                                   \
                                                                                                                                                            \
                                    int nNum = atoi(sNum);                                                                                                  \
                                    EMIT_USHORT( (unsigned short) nNum);                                                                                    \
                                                                                                                                                            \
                                                                                                                                                            \
                                    char* reg = (char*) calloc(findRes - inPtr + 1, 1);                                                                     \
                                    strncpy(reg, inPtr, findRes - inPtr);                                                                                   \
                                    reg[findRes - inPtr] = '\0';                                                                                            \
                                                                                                                                                            \
                                    EMIT_BYTE(regToNum(reg));                                                                                               \
                                }                                                                                                                           \
                                else{                                                                                                                       \
                                    EMIT_BYTE(8);                                                                                                           \
                                    EMIT_BYTE(PP_BIT_m);                                                                                                    \
                                                                                                                                                            \
                                    char* sNum = (char*) calloc(inLen + 1, 1);                                                                              \
                                    strncpy(sNum, inPtr, inLen);                                                                                            \
                                    int nNum = atoi(sNum);                                                                                                  \
                                    EMIT_USHORT( (unsigned short) nNum);                                                                                    \
                                }                                                                                                                           \
                            }                                                                                                                               \
                                                                                                                                                            \
                            else{                                                                                                                           \
                                EMIT_BYTE(8);                                                                                                               \
                                EMIT_BYTE(PP_BIT_r);                                                                                                        \
                                EMIT_BYTE(regToNum(curPtr[0].value.str));                                                                                   \
                            }                                                                                                                               \
                                                                                                                                                            \
                            curPtr++;                                                                                                                       \
                                                                                                                                                            \
                          }

#define PROC_POP_FUNC() {                                                            \
                            curPtr++;                                                \
                            byte mode = curPtr[0];                                   \
                            curPtr++;                                                \
                                                                                     \
                            switch(mode){                                            \
                                case 0:                                              \
                                    theBuf.pop();                                    \
                                                                                     \
                                    break;                                           \
                                                                                     \
                                default:                                             \
                                    printf("THIS CASE OF POP IS YET TO BE DONE\n");  \
                            }                                                        \
                        }

//-----------------------------------------------------------------------------


#define PROC_MATH_FUNC(sign)    {                                    \
                                curPtr++;                            \
                                                                     \
                                double second = theBuf.pop();        \
                                double first  = theBuf.pop();        \
                                                                    \
                                theBuf.push(first sign second);      \
                                                                     \
                                }                                    \



// DEF_CMD(num, name, compCode, procCode)



DEF_CMD(0, EXIT, { EMIT_BYTE(0); curPtr++; }, { printd("Execution of a programm successfully ended\n" "Quitting by EXIT\n"); return 0; })

DEF_CMD(1,  ADD, { EMIT_BYTE(1); curPtr++; }, {PROC_MATH_FUNC(+);} )

DEF_CMD(2,  SUB, { EMIT_BYTE(2); curPtr++; }, {PROC_MATH_FUNC(-);} )

DEF_CMD(3,  MUL, { EMIT_BYTE(3); curPtr++; }, {PROC_MATH_FUNC(*);} )

DEF_CMD(4,  DIV, { EMIT_BYTE(4); curPtr++; }, {PROC_MATH_FUNC();} )

DEF_CMD(5,  POW, { EMIT_BYTE(5); curPtr++; }, {curPtr++; double power = theBuf.pop(); double base = theBuf.pop(); theBuf.push(pow(base, power)); } )

DEF_CMD(6, SQRT, { EMIT_BYTE(6); curPtr++; }, {curPtr++; double value = theBuf.pop();  theBuf.push(sqrt(value)); } )


DEF_CMD(7, PUSH, {COMP_PUSH_FUNC();} , {PROC_PUSH_FUNC();} )

DEF_CMD(8, POP,  {COMP_POP_FUNC();} , {PROC_PUSH_FUNC();} )

DEF_CMD(9, OUT, {EMIT_BYTE(9); curPtr++;}, { printf("%lf\n", theStack.pop()); curPtr++; } )





//-----------------------------------------------------------------------------
//UNIT TEST CMDs
//-----------------------------------------------------------------------------


DEF_CMD(255, DEBUG_INSERT_BYTES, {curPtr++; if(curPtr[0].isStr) throw std::runtime_error("WRONG PARAMS FOR CMD DEBUG_INSERT_BYTES"); else for(byte i = 0; i < curPtr[0].value.dbl; i++) EMIT_BYTE(i); curPtr++; }, {})
