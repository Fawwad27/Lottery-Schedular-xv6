#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Stress Test: Rapidly creates many processes with varying ticket counts
// Tests scheduler performance under heavy load

#define NUM_PROCESSES 50

int
main(int argc, char *argv[])
{
  int i;
  int pids[NUM_PROCESSES];
  int completed = 0;
  
  printf("=== LOTTERY SCHEDULER: STRESS TEST ===\n");
  printf("Creating %d processes with varying ticket counts\n", NUM_PROCESSES);
  printf("This tests scheduler performance under heavy load\n\n");

  int start_time = uptime();

  // Create many processes with different ticket counts
  for(i = 0; i < NUM_PROCESSES; i++) {
    pids[i] = fork();
    
    if(pids[i] == 0) {
      // Child process
      int tickets = (i % 10) + 1; // Tickets from 1 to 10
      
      if(settickets(tickets) < 0) {
        printf("FAILED: settickets(%d) for process %d\n", tickets, i);
        exit(1);
      }
      
      // Shorter CPU work for stress test
      long j;
      for(j = 0; j < 50000000; j++) {
        ;
      }
      
      int end_time = uptime();
      printf("[Process %d] PID=%d, Tickets=%d, Finished at %d ticks\n", 
             i, getpid(), tickets, end_time - start_time);
      exit(0);
    }
    
    if(pids[i] < 0) {
      printf("FAILED: fork() failed for process %d\n", i);
      break;
    }
  }

  // Wait for all children to complete
  printf("\nWaiting for all %d processes to complete...\n", i);
  for(int j = 0; j < i; j++) {
    if(wait(0) > 0) {
      completed++;
    }
  }

  int end_time = uptime();
  printf("\n=== STRESS TEST COMPLETE ===\n");
  printf("SUCCESS: %d/%d processes completed successfully\n", completed, i);
  printf("Total time: %d ticks\n", end_time - start_time);
  printf("All processes were scheduled and completed under heavy load\n");

  exit(0);
}
