#include "type.h"

int eval ();

int_ *pc,  // 程序计数器
    *bp,  // 基址指针
    *sp,  // 栈顶指针
    ax;   // 通用寄存器

enum {
  LEA,  IMM , JMP,  CALL, JZ,   JNZ,  ENT,  ADJ, LEV, LI,  LC,  SI,  SC,  PUSH,
  OR,   XOR , AND , EQ,   NE,   LT,   GT,   LE,  GE,  SHL, SHR, ADD, SUB, MUL, DIV, MOD,
  OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
};

int_ poolsize; // default size of text/data/stack

int_ *text,      // text segment
    *old_text,  // for dump text segment
    *stack;     // stack
char *data;     // data segment