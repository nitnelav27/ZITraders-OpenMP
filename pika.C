#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void inittime() {
srand(time(NULL));
};

int main() {

  inittime();

  
  printf("\n Using Percentage\n");

  int i, num, index;

  for (i = 0; i < 5; i++)
    {
      index = rand();
      num = rand() % 100;
      printf("%d and %d \n", index, num);
    };

  printf("\n Using multiplication\n");

  for (i = 0; i < 5; i++)
    {
      index = rand();
      num = rand() * 100;
      printf("%d and %d\n", index, num);
    };

  return(0);

};
