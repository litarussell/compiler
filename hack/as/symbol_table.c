#include "as.h"

#define norw 23     // 预定义符号个数
#define txmax 50    // 名字表容量

char word[norw][8]; // 保留字

int ptx = 0;        // 记录名字表最后一个元素的索引

int ram_address = 16;

// 名字表结构
struct tablestruct {
    char name[maxsym];  // 名字
    int adr;            // 地址
};
struct tablestruct table[txmax];  // 名字表

/**
 *  预定义符号
 */
enum SYMBOLS {
  ARG = 0, KBD, LCL,
  R0, R1, R2, R3, R4, R5, R6, R7, R8, R9,
  R10, R11, R12, R13, R14, R15,
  SCREEN, SP,
  THAT, THIS,
};

const int PRE_SYMS[] = {
  2, 24576, 1,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 14, 15,
  16384, 0,
  4, 3
};

// 检测预定义符号
static int contains(char* id) {
  int i = 0;
  int j = norw;
  int k;
  do {
    k = (i + j) / 2;
    if (strcmp(id, word[k]) <= 0) j = k - 1;
    if (strcmp(id, word[k]) >= 0) j = k + 1;
  } while(i <= j);

  return i - 1 > j ? k : -1;
}

// 检测符号表中是否有指定符号
static int position(char* id) {
  int i = ptx;
  while (i >= 0 && strcmp(table[i].name, id) != 0) i--;
  return i;
}


/* ------------------------------- 供外部调用 ------------------------------- */
// 添加 符号-地址 到符号表中
int add_entry(char* id, int addr) {
  if (ptx >= txmax) return -1;
  strcpy(table[ptx].name, id);
  if (addr == -1) {
    table[ptx++].adr = ram_address++;
    return ram_address - 1;
  }
  table[ptx++].adr = addr;
  return addr;
}

// 返回指定符号的地址
int get_address(char* id) {
  int c = contains(id);
  if (c != -1) return PRE_SYMS[c];

  c = position(id);
  if (c > 0) return table[c].adr;

  return -1;
}

// 初始化符号、名字表
int init_table() {
  /* 设置保留字名字,按照字母顺序，便于折半查找 */
	strcpy(&(word[ARG][0]), "ARG");
	strcpy(&(word[KBD][0]), "KBD");
	strcpy(&(word[LCL][0]), "LCL");
	strcpy(&(word[R0][0]), "R0");
	strcpy(&(word[R1][0]), "R1");
	strcpy(&(word[R2][0]), "R2");
	strcpy(&(word[R3][0]), "R3");
	strcpy(&(word[R4][0]), "R4");
	strcpy(&(word[R5][0]), "R5");
	strcpy(&(word[R6][0]), "R6");
	strcpy(&(word[R7][0]), "R7");
	strcpy(&(word[R8][0]), "R8");
	strcpy(&(word[R9][0]), "R9");
	strcpy(&(word[R10][0]), "R10");
	strcpy(&(word[R11][0]), "R11");
	strcpy(&(word[R12][0]), "R12");
	strcpy(&(word[R13][0]), "R13");
	strcpy(&(word[R14][0]), "R14");
	strcpy(&(word[R15][0]), "R15");
	strcpy(&(word[SCREEN][0]), "SCREEN");
	strcpy(&(word[SP][0]), "SP");
	strcpy(&(word[THAT][0]), "THAT");
	strcpy(&(word[THIS][0]), "THIS");
}
