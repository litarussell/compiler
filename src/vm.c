#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "vm.h"

int eval () {
  INT op, *tmp;
  while (1) {
    op = *pc++;
    if (op == IMM)       {ax = *pc++;}                // load immediate value to ax
    else if (op == LC)   {ax = *(char *)ax;}          // load character to ax, address in ax
    else if (op == LI)   {ax = *(INT *)ax;}           // load integer to ax, address in ax
    else if (op == SC)   {ax = *(char *)*sp++ = ax;}  // save character to address, value in ax, address on stack
    else if (op == SI)   {*(INT *)*sp++ = ax;}        // save integer to address, value in ax, address on stack
    else if (op == PUSH) {*(--sp) = ax;}                // push the value of ax onto the stack
    else if (op == JMP)  {pc = (INT *)*pc;}           // jump to the address
    else if (op == JZ)   {pc = ax ? pc + 1 : (INT *)*pc;}        // jump if ax is zero
    else if (op == JNZ)  {pc = ax ? (INT *)*pc : pc + 1;}        // jump if ax is not zero
    else if (op == CALL) {*--sp = (INT)(pc+1); pc = (INT *)*pc;} // call subroutine
    // else if (op == RET)  {pc = (INT *)*sp++;}                    // return from subroutine;
    else if (op == ENT)  {*--sp = (INT)bp; bp = sp; sp = sp - *pc++;}     // make new stack frame
    else if (op == LEV)  {sp = bp; bp = (INT *)*sp++; pc = (INT *)*sp++;} // restore call frame and PC
    else if (op == ADJ)  {sp = sp + *pc++;}                               // add esp, <size>
    else if (op == LEV)  {sp = bp; bp = (INT *)*sp++; pc = (INT *)*sp++;} // restore call frame and PC
    else if (op == LEA)  {ax = (INT)(bp + *pc++);}                        // load address for arguments.

    else if (op == OR)  ax = *sp++ | ax;
    else if (op == XOR) ax = *sp++ ^ ax;
    else if (op == AND) ax = *sp++ & ax;
    else if (op == EQ)  ax = *sp++ == ax;
    else if (op == NE)  ax = *sp++ != ax;
    else if (op == LT)  ax = *sp++ < ax;
    else if (op == LE)  ax = *sp++ <= ax;
    else if (op == GT)  ax = *sp++ >  ax;
    else if (op == GE)  ax = *sp++ >= ax;
    else if (op == SHL) ax = *sp++ << ax;
    else if (op == SHR) ax = *sp++ >> ax;
    else if (op == ADD) ax = *sp++ + ax;
    else if (op == SUB) ax = *sp++ - ax;
    else if (op == MUL) ax = *sp++ * ax;
    else if (op == DIV) ax = *sp++ / ax;
    else if (op == MOD) ax = *sp++ % ax;

    else if (op == EXIT) { printf("exit(%ld)", *sp); return *sp;}
    // else if (op == OPEN) { ax = fopen((char *)sp[1], sp[0]); }
    // else if (op == CLOS) { ax = fclose(*sp);}
    // else if (op == READ) { ax = fread(sp[2], (char *)sp[1], *sp); }
    else if (op == PRTF) { tmp = sp + pc[1]; ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]); }
    else if (op == MALC) { ax = (INT)malloc(*sp);}
    else if (op == MSET) { ax = (INT)memset((char *)sp[2], sp[1], *sp);}
    else if (op == MCMP) { ax = memcmp((char *)sp[2], (char *)sp[1], *sp);}

    else {
      printf("unknown instruction:%ld\n", op);
      return -1;
    }
  }
  return 0;
}