#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

void error(char *fmt, ...) __attribute__((noreturn));


// int add_entry(char*, int);

// int get_address(char*);
