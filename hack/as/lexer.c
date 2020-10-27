#include "lexer.h"

#define backleft j = 0;
#define backch if(j > 0) j--;
#define getch if(get_ch() == -1) return -1;
#define nextline if(-1 == next_line()) return -1;

FILE* fin;

int  i = 0;
int  j = 0;
char ch = ' ';        // 获取字符的缓冲区
char line[81];        // 读取行缓冲区
char tmp[maxsym + 1]; // 临时符号, 多出的一个字节用于存放0

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

// 处理当前C指令的dest助记符
d_token dest_token() {
  d_token td = TD_NULL;
  if (ch == 'A') {
    getch;
    if (ch == 'M') {
      getch;
      if (ch == 'D') td = TD_AMD;
      else {
        backch;
        td = TD_AM;
      }
    }
    else if (ch == 'D') td = TD_AD;
    else {
      backch;
      td = TD_A;
    }
  }
  else if (ch == 'M') {
    getch;
    if (ch == 'D') td = TD_MD;
    else {
      backch;
      td = TD_M;
    }
  }
  else if (ch == 'D') td = TD_D;

  getch;
  if (ch == '=' && td == TD_NULL) return -1;
  if (ch != '=') {
    td = TD_NULL;
    backleft;
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
      backch;
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
      backch;
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
      backch;
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
      backch;
    }
    backch;
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
      backch;
    }
    else if (ch == '-') {
      getch;
      if (ch == 'D') {
        getch;
        return TC_A_MINUS_D;
      }
      backch;
    }
    backch;
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
      backch;
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
      backch;
    }
    backch;
    return TC_M;
  }

  backch;
  return -1;
}

// 处理当前C指令的jump助记符
j_token jump_token() {
  if (ch != ';') return T_NULL;
  getch;
  if (ch != 'J') return -1;
  getch;
  if (ch == 'G') {
    getch;
    if (ch == 'T') {
      getch;
      return T_JGT;
    }
    else if (ch == 'E') {
      getch;
      return T_JGE;
    }
    backch;
    return -1;
  }
  else if (ch == 'L') {
    getch;
    if (ch == 'T') {
      getch;
      return T_JLT;
    }
    else if (ch == 'E') {
      getch;
      return T_JLE;
    }
    backch;
    return -1;
  }
  else if (ch == 'E') {
    getch;
    if (ch == 'Q') {
      getch;
      return T_JEQ;
    }
    backch;
    return -1;
  }
  else if (ch == 'N') {
    getch;
    if (ch == 'E') {
      getch;
      return T_JNE;
    }
    backch;
    return -1;
  }
  else if (ch == 'M') {
    getch;
    if (ch == 'P') {
      getch;
      return T_JMP;
    }
    backch;
    return -1;
  }
  backch;
  return -1;
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
      if (d == -1) return -1;
      c_token c = comp_token();
      if (c == -1) return -1;
      j_token j = jump_token();
      if (j == -1) return -1;
      sym_type = C_COMMOND;
      dest_type = d;
      comp_type = c;
      jump_type = j;
    }
  }

  return 0;
}
