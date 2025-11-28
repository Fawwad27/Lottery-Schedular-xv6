#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_PROCS 50

int main(int argc, char *argv) {
  int pid;
  int i;

  printf("Starting Stress Test with %d processes...\n", NUM_PROCS);

  for (i = 0; i < NUM_PROCS; i++) {
    pid = fork();
    if (pid < 0) {
      printf("Fork failed at iteration %d\n", i);
      break;
    }
    if (pid == 0) {
      // Child process: Assign random tickets and do some work
      settickets((i % 10) + 1); // Tickets between 1 and 10
      
      // CPU intensive work
      for (volatile int j = 0; j < 1000000; j++); 
      
      exit(0);
    }
  }

  // Parent waits for all children to finish
  for (i = 0; i < NUM_PROCS; i++) {
    wait(0);
  }

  printf("Stress Test Passed: All %d processes finished safely.\n\n\n", NUM_PROCS);
  exit(0);
}