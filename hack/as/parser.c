/**
 * 语法分析器
 * 读取、解析汇编程序文件
 */

#include "as.h"


#define getch if(-1 == get_ch()) return -1;
#define nextline if(-1 == next_line()) return -1;


typedef enum COMMOND_TYPE {
  A_COMMOND,
  C_COMMOND,
  L_COMMOND,
  NOTE,
} c_type;

int sym;
c_type sym_type;

int code;       // 从A指令中提取的符号地址、或者立即数

char ch = ' ';  // 获取字符的缓冲区
char line[81];  // 读取行缓冲区
FILE* fin;      // 

char tmp[maxsym + 1]; // 临时符号, 多出的一个字节用于存放0
char id[maxsym + 1];    // 当前变量符号

int i = 0;
int j = 0;

// 提取@xxx
// 或提取 (xxx) 中的xxx
int symbol(c_type type) {
  getch;
  if (type == A_COMMOND) { // a指令符号

  }
  else if (type == L_COMMOND) { // 

  }
}

// 处理当前C指令的dest助记符
d_token dest_token() {

}

// 处理当前C指令的comp助记符
c_token comp_token() {
  
}

// 处理当前C指令的jump助记符
j_token jump_token() {
  
}

// 每次读一行, 存入line缓冲区, line被getsym取空后再读一行
int next_line() {
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
  return 0;
}

// 漏掉空格 读取一个字符
int get_ch() {
  if (i == j) {
    nextline;
  }
  ch = line[j++];
  return 0;
}

// 获取下一个token
int get_token() {
  while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
    getch
  }

  if (ch == '/') {
    getch;
    if (ch == '/') { // 注释
      nextline;
      sym_type = NOTE;
      return 0;
    }
  }

  if (ch == '@') { // A指令
    getch;
    // 符号
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
      int k = 0;
      do {
        if (k < maxsym) {
          tmp[k++] = ch;
        }
        getch;
      } while ((ch >= 'a' && ch <= 'z') || (ch>='0' && ch<='9')
              || (ch >= 'A' && ch <= 'Z'));
      tmp[k] = 0;
      strcpy(id, tmp);
      // 搜索符号地址
      int addr = get_address(id);
      
      if (addr == -1) {
        code = add_entry(id);   // 创建符号
      } else {
        code = addr;
      }
      sym_type = A_COMMOND;

      return 0;
    }

    // 立即数
    if (ch >= '0' && ch <= '9') {
      int k = 0;
      int num = 0;
      do { // 拼接数字
        num = num * 10 + ch - '0';
        k++;
        getch;
      } while ((ch >= 'a' && ch <= 'z') || (ch>='0' && ch<='9')
              || (ch >= 'A' && ch <= 'Z'));
      k--;
      if (k > maxsym) {
        return -1;
      }
      code = num;
      sym_type = A_COMMOND;
    }
    // symbol(A_COMMOND);
  }
  else {
    if (ch == '(') {  // 伪命令标签
      sym_type = L_COMMOND;
    }
    else { // C指令
      sym_type = C_COMMOND;
    }
  }

  return 0;
}

void parser() {
  while (get_token() != -1) {
    printf("%u", sym_type);

    if (sym_type == A_COMMOND) {  // 处理A指令
      printf("%d\n", code);
    }
    else if (sym_type == L_COMMOND) {  // 处理伪命令
      int code = symbol(L_COMMOND);
    }
    else if (sym_type == C_COMMOND) {  // 处理C指令
      d_token d = dest_token();
      c_token c = comp_token();
      j_token j = jump_token();
    }
  }
  fclose(fin);
}
