#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Usage: lotterytest <tickets>\n");
    exit(1);
  }

  int n = atoi(argv[1]);

  if(settickets(n) < 0){
    printf("settickets failed\n");
    exit(1);
  }

  // burn CPU
  long i;
  for(i = 0; i < 500000000; i++){
    ; // no op
  }

  printf("Process with %d tickets finished\n", n);
  exit(0);
}
