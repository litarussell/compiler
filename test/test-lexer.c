#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "lexer.h"

int main (int argc, char **argv) {
  int_ i;

  argc--;
  argv++;

  poolsize = 256 * 1024; // arbitrary size
  line = 1;

  if (!(symbols = malloc(poolsize))) {
    printf("无法为symbol table分配内存:%ld\n", poolsize);
    return -1;
  }

  memset(symbols, 0, poolsize);

  src = "char enum else if int_ return sizeof while "
        "void main";
  i = Char;

  while (i <= While) {
    next();
    current_id[Token] = i++;
  }

  next(); current_id[Token] = Char; // handle void type
  next(); idmain = current_id; // keep track of main

  return 0;
}