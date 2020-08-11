#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define maxsym 10  // 符号的最大长度

FILE* fin;

void parser();

int add_entry(char*, int);

int get_address(char*);

/**
 *  汇编token
 */
typedef enum COMP_TOKEN {
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
} c_token;

typedef enum DEST_TOKEN {
  TD_NULL,
  TD_M,
  TD_D,
  TD_MD,
  TD_A,
  TD_AM,
  TD_AD,
  TD_AMD
} d_token;

typedef enum JUMP_TOKEN {
  T_NULL,
  T_JGT,
  T_JEQ,
  T_JGE,
  T_JLT,
  T_JNE,
  T_JLE,
  T_JMP
} j_token;
