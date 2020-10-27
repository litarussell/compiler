#include "parser.h"

int main(int argc, char **argv) {

  if (argc != 2)
    printf("%s: invalid number of arguments", argv[0]);

  char* filename = argv[1];

  fin = fopen(filename, "r");
  if (!fin)
    printf("cannot open %s: %s", filename, strerror(errno));

  parser();

  return 0;
}
