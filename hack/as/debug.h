#pragma once

const char* COMP_MAP[] = {
  "TC_ZERO",
  "TC_ONE",
  "TC_MINUS_ONE",
  "TC_D",
  "TC_A",
  "TC_M",
  "TC_NOT_D",
  "TC_NOT_A",
  "TC_NOT_M",
  "TC_MINUS_D",
  "TC_MINUS_A",
  "TC_MINUS_M",
  "TC_D_ADD_ONE",
  "TC_A_ADD_ONE",
  "TC_M_ADD_ONE",
  "TC_D_MINUS_ONE",
  "TC_A_MINUS_ONE",
  "TC_M_MINUS_ONE",
  "TC_D_ADD_A",
  "TC_D_ADD_M",
  "TC_D_MINUS_A",
  "TC_D_MINUS_M",
  "TC_A_MINUS_D",
  "TC_M_MINUS_D",
  "TC_D_AND_A",
  "TC_D_AND_M",
  "TC_D_OR_A",
  "TC_D_OR_M"
};

const char* DEST_MAP[] = {
  "TD_NULL",
  "TD_M",
  "TD_D",
  "TD_MD",
  "TD_A",
  "TD_AM",
  "TD_AD",
  "TD_AMD"
};

const char* JUMP_MAP[] = {
  "T_NULL",
  "T_JGT",
  "T_JEQ",
  "T_JGE",
  "T_JLT",
  "T_JNE",
  "T_JLE",
  "T_JMP"
};
