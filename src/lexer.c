/**
 * 词法分析器 lexer
 */
#include <stdio.h>
#include "lexer.h"

int ch;
int line_num;
FILE *infile;

// 解析标识符
int parse_ident () {}

void next_ch () {
  ch = fgetc(infile);
  if (ch = '\n')
    line_num++;
}

int next_token () {
  const u_int8_t *p;
  int c;
  switch (c) {
    case 0:
      break;
    case ' ' :
    case '\t': break;

    case '`' : break;
    case '\'': break;
    case '\"': break;
    case '\r': break;
    case '\n': break;
    case '/' : break;

    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_' :
    case '$' :
      break;

    case '.' : break;
    case '0' : break;
    case '1' ... '9': break;

    case '*' : break;
    case '%' : break;
    case '+' : break;
    case '-' : break;
    case '<' : break;
    case '>' : break;
    case '=' : break;
    case '!' : break;
    case '&' : break;
    case '^' : break;
    case '|' : break;
    case '?' : break;
    case ':' : break;

    default:
      break;
  }
}
