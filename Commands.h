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


// DEF_CMD(num, name, compCode, procCode)

DEF_CMD(0, EXIT, printf("TODO COMP CODE"), { printd("Execution of a programm successfully ended\n" "Quitting by EXIT\n"); return 0; })

DEF_CMD(5, PUSH, printf("TODO COMP CODE"), PROC_PUSH_FUNC(); )

DEF_CMD(7, OUT, printf("TODO COMP CODE"), { printf("%lf\n", theStack.pop()); curPtr++; }
