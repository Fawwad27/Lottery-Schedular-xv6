#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Basic Functionality Test: Demonstrates lottery scheduling under normal conditions
// Tests 3 processes with different ticket counts (10, 20, 30)

int
main(int argc, char *argv[])
{
  int pid1, pid2, pid3;
  int start_time, end_time;
  
  printf("=== LOTTERY SCHEDULER: BASIC FUNCTIONALITY TEST ===\n");
  printf("Testing 3 processes with tickets: 10, 20, 30\n");
  printf("Expected: Process with 30 tickets should finish first (on average)\n\n");

  start_time = uptime();

  // Process 1: 10 tickets
  pid1 = fork();
  if(pid1 == 0) {
    if(settickets(10) < 0) {
      printf("FAILED: settickets(10) returned error\n");
      exit(1);
    }
    
    // CPU-intensive work
    long i;
    for(i = 0; i < 300000000; i++) {
      ; // busy loop
    }
    
    end_time = uptime();
    printf("[PID %d] Process with 10 tickets finished at time %d (duration: %d ticks)\n", 
           getpid(), end_time, end_time - start_time);
    exit(0);
  }

  // Process 2: 20 tickets
  pid2 = fork();
  if(pid2 == 0) {
    if(settickets(20) < 0) {
      printf("FAILED: settickets(20) returned error\n");
      exit(1);
    }
    
    long i;
    for(i = 0; i < 300000000; i++) {
      ;
    }
    
    end_time = uptime();
    printf("[PID %d] Process with 20 tickets finished at time %d (duration: %d ticks)\n", 
           getpid(), end_time, end_time - start_time);
    exit(0);
  }

  // Process 3: 30 tickets
  pid3 = fork();
  if(pid3 == 0) {
    if(settickets(30) < 0) {
      printf("FAILED: settickets(30) returned error\n");
      exit(1);
    }
    
    long i;
    for(i = 0; i < 300000000; i++) {
      ;
    }
    
    end_time = uptime();
    printf("[PID %d] Process with 30 tickets finished at time %d (duration: %d ticks)\n", 
           getpid(), end_time, end_time - start_time);
    exit(0);
  }

  // Parent waits for all children
  wait(0);
  wait(0);
  wait(0);

  printf("\n=== TEST COMPLETE ===\n");
  printf("SUCCESS: All processes completed. Check finish times above.\n");
  printf("Process with more tickets should generally finish earlier.\n");

  exit(0);
}
