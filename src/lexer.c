/**
 * 词法分析器 lexer
 */
#include <string.h>
#include "lexer.h"

void next () {
  char *last_pos;
  int_ hash;
  while ((token = *src)) {
    ++src;
    if (token == '\n') {
      ++line;
    }
    // 处理注释
    else if (token == '#') {
      while (*src !=0 && *src != '\n') {src++;}
    }
    // 处理标识符
    else if ((token >= 'a' && token <= 'z')
      || (token >= 'A' && token <= 'Z')
      || (token == '_')) {
      last_pos = src - 1;
      hash = token;

      while (
        (*src >= 'a' && *src <= 'z')
        || (*src >= 'A' && *src <= 'Z')
        || (*src >= '0' && *src <= '9')
        || (*src == '_')) {
        hash = hash * 147 + *src;
        src++;
      }

      current_id = symbols; // 从符号表中线性查找标识符
      while (current_id[Token]) {
        if (current_id[Hash] == hash
          && !memcmp((char *)current_id[Name], last_pos, src - last_pos)) {
            token = current_id[Token];
            return;
        }
        // 如果该标识符不在表中, 那么就新建一个
        current_id = current_id + IdSize;
      }
      
      token = current_id[Token] = Id;    // 0
      current_id[Hash] = hash;           // 1
      current_id[Name] = (int_)last_pos; // 2
      return;
    }
    // 处理数字
    else if (token >= '0' && token <= '9') {
      token_val = token - '0';
      if (token_val > 0) {
        while (*src >= '0' && *src <= '9') {
          token_val = token_val * 10 + *src++ - '0';
        }
      } else {
        // 处理16进制 0x
        if (*src == 'x' || *src == 'X') {
          token = *++src;
          while ((token >= '0' && token <= '9')
            || (token >= 'a' && token <= 'f')
            || (token >= 'A' && token <= 'F')) {
            token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
            token = *++src;
          } 
        } else { // 8进制
          while (*src >= '0' && *src <= '7') {
            token_val = token_val * 8 + *src++ - '0';
          }
        }
      }
      token = Num;
      return;
    }
    // 处理字符串
    else if (token == '"' || token == '\'') {
      // last_pos = data;
    }
    // 处理注释 除法
    else if (token == '/') {
      if (*src == '/') {
        while (*src != 0 && *src != '\n') {
          ++src;
        }
      } else {
        token = Div;
        return;
      }
    }
    // 处理其他标记
    else if (token == '=') {
      if (*src == '=') {
        src++;
        token = Eq;
      } else {
        token = Assign;
      }
    }
    else if (token == '+') {
      if (*src == '+') {
          src ++;
          token = Inc;
      } else {
          token = Add;
      }
      return;
    }
    else if (token == '-') {
      if (*src == '-') {
          src ++;
          token = Dec;
      } else {
          token = Sub;
      }
      return;
    }
    else if (token == '!') {
      if (*src == '=') {
          src++;
          token = Ne;
      }
      return;
    }
    else if (token == '<') {
      if (*src == '=') {
          src++;
          token = Le;
      } else if (*src == '<') {
          src++;
          token = Shl;
      } else {
          token = Lt;
      }
      return;
    }
    else if (token == '>') {
      if (*src == '=') {
          src++;
          token = Ge;
      } else if (*src == '>') {
          src++;
          token = Shr;
      } else {
          token = Gt;
      }
      return;
    }
    else if (token == '|') {
      if (*src == '|') {
          src ++;
          token = Lor;
      } else {
          token = Or;
      }
      return;
    }
    else if (token == '&') {
      if (*src == '&') {
          src ++;
          token = Lan;
      } else {
          token = And;
      }
      return;
    }
    else if (token == '^') {
      token = Xor;
      return;
    }
    else if (token == '%') {
      token = Mod;
      return;
    }
    else if (token == '*') {
      token = Mul;
      return;
    }
    else if (token == '[') {
      token = Brak;
      return;
    }
    else if (token == '?') {
      token = Cond;
      return;
    }
    else if (token == '~' || token == ';'
      || token == '{' || token == '}' || token == '(' || token == ')'
      || token == ']' || token == ',' || token == ':') {
      // directly return the character as token;
      return;
    }
  }
  return;
}
