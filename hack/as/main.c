#include "as.h"

int main(int argc, char **argv) {

  if (argc != 2)
    printf("%s: invalid number of arguments", argv[0]);

  char* filename = argv[1];

  FILE* fp = fopen(filename, "r");
  if (!fp)
    printf("cannot open %s: %s", filename, strerror(errno));

  parser(fp);

  printf("%d:\n", '\n');

  return 0;
}
