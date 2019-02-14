#include <stdio.h>

int main()
{
  int a = 0, b = 0;
  for (int i = 0; i < 10; i++) {
    b += i;
  }

  printf("%d", b);
}