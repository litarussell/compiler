#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define maxsym 10  // 符号的最大长度

FILE* fin;

void parser();

int add_entry(char*, int);

int get_address(char*);
