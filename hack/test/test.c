#include <stdio.h>
#include <stdbool.h>

bool isCPULittleEnd() {
    union {
        int iVal;
        char cVal;
    }Endian;
 
    Endian.iVal = 1;
 
    return (1 == Endian.cVal);
}

int main () {
  FILE* fp;
  fp = fopen("./temp", "wb");
  int n = 0b101;
  fwrite(&n, sizeof(int), 1, fp);
  fclose(fp);
  printf("%d:", isCPULittleEnd());
  return 0;
}

