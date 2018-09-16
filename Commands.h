



// DEF_CMD(num, name, compCode, procCode)

DEF_CMD(1,  CMD, printf("TODO COMP CODE"), { printd("TODO PROC CODE [CMD IS 1-\"CMD\"]\n"); curPtr++; })

DEF_CMD(0, EXIT, printf("TODO COMP CODE"), { printd("Execution of a programm successfully ended\n" "Quitting by EXIT\n"); return 0; })
