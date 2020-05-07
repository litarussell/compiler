#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "vm.h"

int main (int argc, char **argv) {
  int i, fd;

  argc--;
  argv++;

  poolsize = 256 * 1024; // arbitrary size


  // if ((fd = fopen(*argv, 0)) < 0) {
  //   printf('无法打开文件:(%s)\n', *argv);
  //   return -1;
  // }

  if (!(text = malloc(poolsize))) {
    printf("无法为text段分配内存\n");
    return -1;
  }
  if (!(data = malloc(poolsize))) {
    printf("无法为text段分配内存\n");
    return -1;
  }
  if (!(stack = malloc(poolsize))) {
    printf("无法为text段分配内存\n");
    return -1;
  }

  memset(text, 0, poolsize);
  memset(data, 0, poolsize);
  memset(stack, 0, poolsize);

  bp = sp = (INT *)((INT)stack + poolsize); // 将bp sp指向栈底
  ax = 0;

  i = 0;
  text[i++] = IMM;  // 1
  text[i++] = 10;   // 10
  text[i++] = PUSH; // 13
  text[i++] = IMM;  // 1
  text[i++] = 20;   // 20
  text[i++] = ADD;  //
  text[i++] = PUSH;
  text[i++] = EXIT;

  pc = text;

  return eval();
}