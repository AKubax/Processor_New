#define EMIT_BYTE(val)  outBuf.push_back( (val) );
#define EMIT_SHORT(val) outBuf.resize(outBuf.size() + sizeof( short)); ((short*)  (outBuf.data() + outBuf.size() - sizeof( short)))[0] = (val);
#define EMIT_DBL(val)   outBuf.resize(outBuf.size() + sizeof(double)); ((double*) (outBuf.data() + outBuf.size() - sizeof(double)))[0] = (val);

#define DBL_PTR(ptr)    ((double*) ptr)


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
                            if(curPtr[0].isStr) printf("STR CASE OF PUSH IS YET TO BE DONE\n");      \
                            else                EMIT_DBL(curPtr[0].value.dbl);                     \
                          }

#define COMP_POP_FUNC()   {                                                         \
                            curPtr++;                                               \
                                                                                    \
                            if(strcmp(curPtr[0].value.str, "DEL") == 0){            \
                                EMIT_BYTE(8);                                       \
                                EMIT_BYTE(0);                                       \
                            }                                                       \
                            else printf("THIS CASE OF POP IS YET TO BE DONE\n");      \
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
