/**
 * 语法分析器
 * 读取、解析汇编程序文件
 */

#include "as.h"

#define getch if(-1 == get_ch()) return -1;

typedef enum COMMOND_TYPE {
  A_COMMOND,
  C_COMMOND,
  L_COMMOND
} c_type;

char ch = ' ';  // 获取字符的缓冲区
char line[81];  // 读取行缓冲区
FILE* fin;      // 

int i = 0;
int j = 0;

int get_ch() {
  if (i == j) {
    if (feof(fin)) {
      printf("程序读取完成");
      return -1;
    }
    i = j = 0;
    ch = ' ';
    while (ch != '\n') {
      if (fscanf(fin, "%c", &ch) == EOF) {
        line[i] = 0;
        break;
      }
      line[i++] = ch;
    }
  }
  ch = line[j++];
  return 0;
}

int get_token() {
  while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
    getch
  }
}



// 返回指令类型
c_type commandType() {

}

// 提取@xxx 或 (xxx) 中的xxx
int symbol() {

}

// 返回当前C指令的助记符
int dest() {

}


void parser(FILE* fp) {
  fin = fp;
  while (get_token() != -1) {
    printf("%s", line);
  }
  
  fclose(fp);
}
