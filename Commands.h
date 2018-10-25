#define EMIT_BYTE(val)   outBuf.push_back( (val) );
#define EMIT_SHORT(val)  outBuf.resize(outBuf.size() + sizeof( short)); ((short*)  (outBuf.data() + outBuf.size() - sizeof( short)))[0] = (val);
#define EMIT_USHORT(val) outBuf.resize(outBuf.size() + sizeof(unsigned short)); ((unsigned short*)  (outBuf.data() + outBuf.size() - sizeof(unsigned short)))[0] = (val);
#define EMIT_DBL(val)    outBuf.resize(outBuf.size() + sizeof(double)); ((double*) (outBuf.data() + outBuf.size() - sizeof(double)))[0] = (val);

#define GET_BYTE()   ( curPtr[0] )
#define GET_SHORT()  ( ((short*) curPtr)[0] )
#define GET_USHORT() ( ((unsigned short*) curPtr)[0] )
#define GET_DBL()    ( ((double*) curPtr)[0] )


#define DBL_PTR(ptr)    ((double*) ptr)


#define PP_BIT_s 1
#define PP_BIT_r 2
#define PP_BIT_m 4


#define PROC_PUSH_FUNC()  {                                                                     \
                            curPtr++;                                                           \
                                                                                                \
                            if(curPtr[0] == PP_BIT_s){                                          \
                                curPtr++;                                                       \
                                                                                                \
                                theStack.push( GET_DBL() );                                     \
                                curPtr += sizeof(double);                                       \
                            }                                                                   \
                                                                                                \
                            else if(curPtr[0] == PP_BIT_r){                                     \
                                curPtr++;                                                       \
                                                                                                \
                                byte regNum = GET_BYTE();                                       \
                                curPtr++;                                                       \
                                                                                                \
                                theStack.push( REGs[regNum] );                                  \
                            }                                                                   \
                                                                                                \
                            else if(curPtr[0] == PP_BIT_m){                                     \
                                curPtr++;                                                       \
                                                                                                \
                                unsigned short mPoint = GET_USHORT();                           \
                                curPtr += sizeof(short);                                        \
                                                                                                \
                                theStack.push( RAM[mPoint] );                                   \
                                                                                                \
                            }                                                                   \
                                                                                                \
                            else if (curPtr[0] == (PP_BIT_m | PP_BIT_r)){                       \
                                curPtr++;                                                       \
                                                                                                \
                                unsigned short mPoint = GET_USHORT();                           \
                                curPtr += sizeof(short);                                        \
                                                                                                \
                                byte regNum = GET_BYTE();                                       \
                                curPtr++;                                                       \
                                                                                                \
                                theStack.push( RAM[mPoint + ((int) REGs[regNum])] );            \
                            }                                                                   \
                          }

#define COMP_PUSH_FUNC()  {                                                                                                                                 \
                            printd("Begin of COMP_PUSH_FUNC()\n");                                                                                          \
                            curPtr++;                                                                                                                       \
                                                                                                                                                            \
                            if(!curPtr[0].isStr){                                                                                                           \
                                EMIT_BYTE (7);                                                                                                              \
                                EMIT_BYTE (PP_BIT_s);                                                                                                       \
                                EMIT_DBL  (curPtr[0].value.dbl);                                                                                            \
                            }                                                                                                                               \
                            else if(curPtr[0].value.str[0] == '['){                                                                                         \
                                size_t len = strlen(curPtr[0].value.str);                                                                                   \
                                if( (curPtr[0].value.str + len - 1)[0] != ']' ) throw std::runtime_error("Unclosed parenthesis or space in POP argument\n");\
                                                                                                                                                            \
                                size_t inLen = len - 2;                                                                                                     \
                                const char* inPtr = curPtr[0].value.str + 1;                                                                                \
                                                                                                                                                            \
                                const char* findRes;                                                                                                        \
                                if((findRes = std::find(inPtr, inPtr + inLen, '+')) != inPtr + inLen){                                                      \
                                    EMIT_BYTE(7);                                                                                                           \
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
                                    EMIT_BYTE(7);                                                                                                           \
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
                                EMIT_BYTE(7);                                                                                                               \
                                EMIT_BYTE(PP_BIT_r);                                                                                                        \
                                EMIT_BYTE(regToNum(curPtr[0].value.str));                                                                                   \
                            }                                                                                                                               \
                                                                                                                                                            \
                            curPtr++;                                                                                                                       \
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

#define PROC_POP_FUNC() {                                                                   \
                            curPtr++;                                                       \
                            printd("PROC_POP_FUNC() started. curPtr[0] == %d\n", *curPtr);  \
                                                                                            \
                            if(curPtr[0] == PP_BIT_r){                                      \
                                    printd("PP_BIT_r case\n");                              \
                                                                                            \
                                    curPtr++;                                               \
                                    byte regNum = GET_BYTE();                               \
                                    curPtr++;                                               \
                                    REGs[regNum] = theStack.pop();                          \
                                                                                            \
                            }                                                               \
                                                                                            \
                            else if(curPtr[0] == PP_BIT_m){                                 \
                                    printd("PP_BIT_m case\n");                              \
                                                                                            \
                                    curPtr++;                                               \
                                    unsigned short mPoint = GET_USHORT();                   \
                                    curPtr += sizeof(unsigned short);                       \
                                                                                            \
                                    RAM[mPoint] = theStack.pop();                           \
                                                                                            \
                            }                                                               \
                                                                                            \
                            else if(curPtr[0] == (PP_BIT_m | PP_BIT_r)){                    \
                                    printd("PP_BIT_m | PP_BIT_r case\n");                   \
                                                                                            \
                                    curPtr++;                                               \
                                    unsigned short mPoint = GET_USHORT();                   \
                                    curPtr += sizeof(unsigned short);                       \
                                    byte regNum = GET_BYTE();                               \
                                    curPtr++;                                               \
                                                                                            \
                                    RAM[ mPoint + ((int) REGs[regNum]) ] = theStack.pop();  \
                            }                                                               \
                        }

//-----------------------------------------------------------------------------


#define PROC_MATH_FUNC(sign)    {                                    \
                                curPtr++;                            \
                                                                     \
                                double second = theStack.pop();      \
                                double first  = theStack.pop();      \
                                                                     \
                                theStack.push(first sign second);    \
                                                                     \
                                }                                    \



// DEF_CMD(num, name, compCode, procCode)



DEF_CMD(0, EXIT, { EMIT_BYTE(0); curPtr++; }, { printd("Execution of a programm successfully ended\n" "Quitting by EXIT\n"); return 0; })

DEF_CMD(1,  ADD, { EMIT_BYTE(1); curPtr++; }, {PROC_MATH_FUNC(+);} )

DEF_CMD(2,  SUB, { EMIT_BYTE(2); curPtr++; }, {PROC_MATH_FUNC(-);} )

DEF_CMD(3,  MUL, { EMIT_BYTE(3); curPtr++; }, {PROC_MATH_FUNC(*);} )

DEF_CMD(4,  DIV, { EMIT_BYTE(4); curPtr++; }, {PROC_MATH_FUNC(/);} )

DEF_CMD(5,  POW, { EMIT_BYTE(5); curPtr++; }, {curPtr++; double power = theStack.pop(); double base = theStack.pop(); theStack.push(pow(base, power)); } )

DEF_CMD(6, SQRT, { EMIT_BYTE(6); curPtr++; }, {curPtr++; double value = theStack.pop();  theStack.push(sqrt(value)); } )


DEF_CMD(7, PUSH, {COMP_PUSH_FUNC();} , {PROC_PUSH_FUNC();} )

DEF_CMD(8, POP,  {COMP_POP_FUNC();} , {PROC_POP_FUNC();} )

DEF_CMD(9, OUT,  {EMIT_BYTE(9); curPtr++;}, { printf("%lf\n", theStack.pop()); curPtr++; } )

DEF_CMD(0xDA, NOP, {EMIT_BYTE(0xDA); EMIT_BYTE(0xDA); curPtr++;}, {curPtr+= 2;} )





//-----------------------------------------------------------------------------
//UNIT TEST CMDs
//-----------------------------------------------------------------------------


DEF_CMD(255, DEBUG_INSERT_BYTES, {curPtr++; if(curPtr[0].isStr) throw std::runtime_error("WRONG PARAMS FOR CMD DEBUG_INSERT_BYTES"); else for(byte i = 0; i < curPtr[0].value.dbl; i++) EMIT_BYTE(i); curPtr++; }, {})
