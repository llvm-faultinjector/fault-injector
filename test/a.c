#include <stdio.h>

int main(int argc, char *argv[])
{
  int a = 0, b = 0;
  for (int i = 0; i < argc; i++) {
    b += i;
  }

  printf("%d", b);
}