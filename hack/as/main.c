#include "as.h"

int main(int argc, char **argv) {

  if (argc != 2)
    error("%s: invalid number of arguments", argv[0]);

  char* filename = argv[1];

  FILE* fp = fopen(filename, "r");
  if (!fp)
    error("cannot open %s: %s", filename, strerror(errno));

  parser(fp);

  printf("%d:\n", '\n');

  return 0;
}
