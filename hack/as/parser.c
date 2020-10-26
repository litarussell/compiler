/**
 * 语法分析器
 * 读取、解析汇编程序文件
 */

#include "as.h"
#include "parser.h"

#define getch if(get_ch() == -1) return -1;
#define nextline if(-1 == next_line()) return -1;

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

typedef enum COMMOND_TYPE {
  A_COMMOND_ID,
  A_COMMOND_NUM,
  C_COMMOND,
  L_COMMOND,
  NOTE,
} c_type;


FILE* fin;            // 
int  num;             // 立即数
int  code;            // 从A指令中提取的符号地址、或者立即数
int  pc = -1;         // 程序计数器 每遇到A或C指令+1
char ch = ' ';        // 获取字符的缓冲区
char line[81];        // 读取行缓冲区
char id[maxsym + 1];  // 当前变量符号
char tmp[maxsym + 1]; // 临时符号, 多出的一个字节用于存放0

c_type sym_type;

int i = 0;
int j = 0;

// 提取@xxx
// 或提取 (xxx) 中的xxx
// int symbol(c_type type) {
//   // getch;
//   if (type == A_COMMOND_ID) { // a指令符号

//   }
//   else if (type == L_COMMOND) { // 

//   }
// }

// 处理当前C指令的dest助记符
d_token dest_token() {
  d_token td = TD_NULL;
  if (ch == 'A') {
    getch;
    if (ch == 'M') {
      getch;
      if (ch == 'D') td = TD_AMD;
      else {
        back_ch();
        td = TD_AM;
      }
    }
    else if (ch == 'D') td = TD_AD;
    else {
      back_ch();
      td = TD_A;
    }
  }
  else if (ch == 'M') {
    getch;
    if (ch == 'D') td = TD_MD;
    else {
      back_ch();
      td = TD_M;
    }
  }
  else if (ch == 'D') td = TD_D;

  getch;
  if (ch != '=') {
    back_ch(0);
  }
  return td;
}

// 处理当前C指令的comp助记符
c_token comp_token() {
  if (ch == '=') getch;
  
  if (ch == '-') {
    getch;
    switch (ch) {
      case '1':
        getch;
        return TC_MINUS_ONE;
      case 'A':
        getch;
        return TC_MINUS_A;
      case 'D':
        getch;
        return TC_MINUS_D;
      case 'M':
        getch;
        return TC_MINUS_M;
    }
    return -1;
  }

  if (ch == '!') {
    getch;
    switch (ch) {
      case 'A':
        getch;
        return TC_NOT_A;
      case 'D':
        getch;
        return TC_NOT_D;
      case 'M':
        getch;
        return TC_NOT_M;
    }
    return -1;
  }

  if (ch == '0') {
    getch;
    return TC_ZERO;
  }
  if (ch == '1') {
    getch;
    return TC_ONE;
  }
  
  if (ch == 'D') {
    getch;
    if (ch == '+') {
      getch;
      if (ch == '1') {
        getch;
        return TC_D_ADD_ONE;
      }
      else if (ch == 'A') {
        getch;
        return TC_D_ADD_A;
      }
      else if (ch == 'M') {
        getch;
        return TC_D_ADD_M;
      }
      back_ch();
    }
    else if (ch == '-') {
      getch;
      if (ch == '1') {
        getch;
        return TC_D_MINUS_ONE;
      }
      else if (ch == 'A') {
        getch;
        return TC_D_MINUS_A;
      }
      else if (ch == 'M') {
        getch;
        return TC_D_MINUS_M;
      }
      back_ch();
    }
    else if (ch == '&') {
      getch;
      if (ch == 'A') {
        getch;
        return TC_D_AND_A;
      }
      else if (ch == 'M') {
        getch;
        return TC_D_AND_M;
      }
      back_ch();
    }
    else if (ch == '|') {
      getch;
      if (ch == 'A') {
        getch;
        return TC_D_OR_A;
      }
      else if (ch == 'M') {
        getch;
        return TC_D_OR_M;
      }
      back_ch();
    }
    back_ch();
    return TC_D;
  }
  else if (ch == 'A') {
    getch;
    if (ch == '+') {
      getch;
      if (ch == '1') {
        getch;
        return TC_A_ADD_ONE;
      }
      back_ch();
    }
    else if (ch == '-') {
      getch;
      if (ch == 'D') {
        getch;
        return TC_A_MINUS_D;
      }
      back_ch();
    }
    back_ch();
    return TC_A;
  }
  else if (ch == 'M') {
    getch;
    if (ch == '+') {
      getch;
      if (ch == '1') {
        getch;
        return TC_M_ADD_ONE;
      }
      back_ch();
    }
    else if (ch == '-') {
      getch;
      if (ch == '1') {
        getch;
        return TC_M_MINUS_ONE;
      }
      else if (ch == 'D') {
        getch;
        return TC_M_MINUS_D;
      }
      back_ch();
    }
    back_ch();
    return TC_M;
  }

  back_ch();
  return -1;
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

// 回溯
void back_ch(int n) {
  if (n == 0) j = 0;
  else if (j > 0) j--;
}

// 获取下一个token
int get_token() {
  while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
    getch;
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
      sym_type = A_COMMOND_ID;
      return 0;
    }

    // 立即数
    if (ch >= '0' && ch <= '9') {
      int k = 0;
      int n = 0;
      do { // 拼接数字
        n = n * 10 + ch - '0';
        k++;
        getch;
      } while ((ch >= 'a' && ch <= 'z') || (ch>='0' && ch<='9')
              || (ch >= 'A' && ch <= 'Z'));
      k--;
      if (k > maxsym) {
        return -1;
      }
      num = n;
      sym_type = A_COMMOND_NUM;
      return 0;
    }

    return -1;
  }
  else {
    if (ch == '(') {  // 伪命令标签
      getch;
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

        if (ch != ')') {
          return -1;
        }
        strcpy(id, tmp);
        sym_type = L_COMMOND;
      }

      return -1;
    }
    else { // C指令
      d_token d = dest_token();
      c_token c = comp_token();
      j_token j = jump_token();
      pc++;
      sym_type = C_COMMOND;
    }
  }

  return 0;
}

void parser() {
  while (get_token() != -1) {
    printf("%u", sym_type);

    if (sym_type == A_COMMOND_ID) {  // 处理A指令-符号
      // 搜索符号地址
      // int addr = get_address(id);
      
      // if (addr == -1) {
      //   code = add_entry(id, -1);   // 创建符号
      // } else {
      //   code = addr;
      // }
      printf("%d\n", code);
      pc++;
    }
    else if (sym_type == A_COMMOND_NUM) { // 处理A指令-立即数
      code = num;
      printf("%d\n", code);
      pc++;
    }
    else if (sym_type == L_COMMOND) {  // 处理伪命令
      printf("%s\n", id);
      // add_entry(id);
      // int code = symbol(L_COMMOND);
    }
    else if (sym_type == C_COMMOND) {  // 处理C指令
      printf("%d\n", sym_type);
      pc++;
    }
  }
  fclose(fin);
}
