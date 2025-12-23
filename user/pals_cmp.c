#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// PALS Comparison Test: Comprehensive test for baseline vs PALS comparison
// Mixed workload with CPU-bound and I/O-bound processes

#define NUM_CPU_PROCS 3
#define NUM_IO_PROCS 3
#define CPU_WORK 1000000000 // Increased from 100M to 1B
#define IO_BURSTS 15
#define IO_SLEEP 3

int main(int argc, char *argv[]) {
  int start_time, end_time;

  printf("=== PALS COMPREHENSIVE COMPARISON TEST ===\n");
  printf("Mixed workload: %d CPU-bound + %d I/O-bound processes\n\n",
         NUM_CPU_PROCS, NUM_IO_PROCS);

  start_time = uptime();

  // Create CPU-bound processes with varying tickets
  int cpu_tickets[] = {10, 20, 30};
  for (int i = 0; i < NUM_CPU_PROCS; i++) {
    int pid = fork();
    if (pid == 0) {
      if (settickets(cpu_tickets[i]) < 0) {
        printf("FAILED: settickets(%d) returned error\n", cpu_tickets[i]);
        exit(1);
      }

      int proc_start = uptime();
      long work;
      for (work = 0; work < CPU_WORK; work++)
        ;
      int proc_end = uptime();

      printf(
          "[CPU-BOUND %d, %d tickets] Start: %d, End: %d, Duration: %d ticks\n",
          i, cpu_tickets[i], proc_start, proc_end, proc_end - proc_start);
      exit(0);
    }
  }

  // Create I/O-bound processes with varying tickets
  int io_tickets[] = {5, 10, 15};
  for (int i = 0; i < NUM_IO_PROCS; i++) {
    int pid = fork();
    if (pid == 0) {
      if (settickets(io_tickets[i]) < 0) {
        printf("FAILED: settickets(%d) returned error\n", io_tickets[i]);
        exit(1);
      }

      int proc_start = uptime();
      int total_latency = 0;

      for (int burst = 0; burst < IO_BURSTS; burst++) {
        pause(IO_SLEEP);
        int after_sleep = uptime();

        // Work burst (increased for better measurement)
        long work;
        for (work = 0; work < 50000000; work++)
          ;

        int after_work = uptime();
        total_latency += (after_work - after_sleep);
      }

      int proc_end = uptime();
      int avg_latency = total_latency / IO_BURSTS;

      printf("[I/O-BOUND %d, %d tickets] Start: %d, End: %d, Duration: %d, Avg "
             "wakeup latency: %d ticks\n",
             i, io_tickets[i], proc_start, proc_end, proc_end - proc_start,
             avg_latency);
      exit(0);
    }
  }

  // Wait for all children
  for (int i = 0; i < NUM_CPU_PROCS + NUM_IO_PROCS; i++) {
    wait(0);
  }

  end_time = uptime();

  printf("\n=== TEST COMPLETE ===\n");
  printf("Total duration: %d ticks\n", end_time - start_time);
  printf("\nKey metrics to compare:\n");
  printf("- CPU-bound process completion times (should be proportional to "
         "tickets)\n");
  printf(
      "- I/O-bound average wakeup latency (PALS should show lower latency)\n");
  printf("- Overall fairness and responsiveness\n");

  exit(0);
}
