
DEF_MACRO(1, TEST, 1, "PUSH $1 DUP MUL PUSH 2 MUL OUT")

DEF_MACRO(2, ADD_TWO, 2, "PUSH $1 PUSH $2 ADD")

DEF_MACRO(3, MUL_BY_TWO, 1, "ADD_TWO $1 $1")
