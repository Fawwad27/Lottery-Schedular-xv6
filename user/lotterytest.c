#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv) {
  int pid1, pid2;
  
  printf("Starting Lottery Basic Test...\n");
  printf("Process with 80 tickets should finish much earlier than process with 10 tickets.\n\n");

  // Parent sets low tickets to stay out of the way
  if(settickets(1) < 0) printf("Syscall failed\n");

  pid1 = fork();
  if (pid1 == 0) {
    settickets(10); // Process A: 10 Tickets
    int start = uptime();
    int local_count = 0;
    for (int iterations = 0; iterations < 1000; iterations++) {
       // Do CPU work
       for (volatile int i = 0; i < 500000; i++) {
         local_count++;
       }
    }
    int end = uptime();
    printf("Process 1 (10 tickets): finished in %d ticks\n", end - start);
    exit(0);
  }

  pid2 = fork();
  if (pid2 == 0) {
    settickets(80); // Process B: 80 Tickets (Should run ~8x more often)
    int start = uptime();
    int local_count = 0;
    for (int iterations = 0; iterations < 1000; iterations++) {
       // Same amount of work
       for (volatile int i = 0; i < 500000; i++) {
         local_count++;
       }
    }
    int end = uptime();
    printf("Process 2 (80 tickets): finished in %d ticks\n", end - start);
    exit(0);
  }

  // Wait for both children to finish
  wait(0);
  wait(0);
  
  printf("\nTest Done.\n\n");
  exit(0);
}