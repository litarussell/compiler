#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


void error(char *fmt, ...);

void parser(FILE* fp);

/**
 * 指令
 */
enum COMP_COMMOND {
  TC_ZERO,
  TC_ONE,
  TC_MINUS_ONE,
  TC_D,
  TC_A,
  TC_M,
  TC_NOT_D,
  TC_NOT_A,
  TC_NOT_M,
  TC_MINUS_D,
  TC_MINUS_A,
  TC_MINUS_M,
  TC_D_ADD_ONE,
  TC_A_ADD_ONE,
  TC_M_ADD_ONE,
  TC_D_MINUS_ONE,
  TC_A_MINUS_ONE,
  TC_M_MINUS_ONE,
  TC_D_ADD_A,
  TC_D_ADD_M,
  TC_D_MINUS_A,
  TC_D_MINUS_M,
  TC_A_MINUS_D,
  TC_M_MINUS_D,
  TC_D_AND_A,
  TC_D_AND_M,
  TC_D_OR_A,
  TC_D_OR_M
};

enum DEST_COMMOND {
  TD_NULL,
  TD_M,
  TD_D,
  TD_MD,
  TD_A,
  TD_AM,
  TD_AD,
  TD_AMD
};

enum JUMP_COMMOND {
  T_NULL,
  T_JGT,
  T_JEQ,
  T_JGE,
  T_JLT,
  T_JNE,
  T_JLE,
  T_JMP
};

const char* COMP[] = {
  "0101010",  // 0
  "0111111",  // 1
  "0111010",  // -1
  "0001100",  // D
  "0110000",  // A
  "1110000",  // M
  "0001101",  // !D
  "0110001",  // !A
  "1110001",  // !M
  "0001111",  // -D
  "0110011",  // -A
  "1110011",  // -M
  "0011111",  // D+1
  "0110111",  // A+1
  "1110111",  // M+1
  "0001110",  // D-1
  "0110010",  // A-1
  "1110010",  // M-1
  "0000010",  // D+A
  "1000010",  // D+M
  "0010011",  // D-A
  "1010011",  // D-M
  "0000111",  // A-D
  "1000111",  // M-D
  "0000000",  // D&A
  "1000000",  // D&M
  "0010101",  // D|A
  "1010101",  // D|M
};

const char* DEST[] = {
  "000",  // null
  "001",  // M
  "010",  // D
  "011",  // MD
  "100",  // A
  "101",  // AM
  "110",  // AD
  "111",  // AMD
};

const char* JUMP[] = {
  "000",  // null
  "001",  // JGT
  "010",  // JEQ
  "011",  // JGE
  "100",  // JLT
  "101",  // JNE
  "110",  // JLE
  "111",  // JMP
};

/**
 *  预定义符号
 */
enum SYMBOLS {
  SP, LCL, ARG, THIS, THAT,
  R0, R1, R2, R3, R4, R5, R6, R7, R8, R9,
  R10, R11, R12, R13, R14, R15,
  SCREEN, KBD
};

const int PRE_SYMS[] = {
  0, 1, 2, 3, 4,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 14, 15,
  16384, 24576
};
