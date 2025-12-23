#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// PALS Interactive Test: Simulates desktop interactive workload
// Tests wakeup compensation by measuring wakeup-to-run latency

#define NUM_BURSTS 20
#define SLEEP_TIME 5 // ticks to sleep between bursts

int main(int argc, char *argv[]) {
  int pid_bg, pid_fg;
  int start_time, end_time;

  printf("=== PALS INTERACTIVE WORKLOAD TEST ===\n");
  printf("Testing wakeup compensation for bursty I/O processes\n\n");

  start_time = uptime();

  // Background CPU-intensive process (100 tickets)
  pid_bg = fork();
  if (pid_bg == 0) {
    if (settickets(100) < 0) {
      printf("FAILED: settickets(100) returned error\n");
      exit(1);
    }

    // CPU-intensive work (increased for better contention)
    long i;
    for (i = 0; i < 10000000000; i++) {
      ; // busy loop
    }

    printf("[BACKGROUND] CPU-intensive task completed\n");
    exit(0);
  }

  // Foreground bursty I/O process (10 tickets)
  pid_fg = fork();
  if (pid_fg == 0) {
    if (settickets(10) < 0) {
      printf("FAILED: settickets(10) returned error\n");
      exit(1);
    }

    // Simulate bursty I/O: sleep, wake, do work, repeat
    for (int burst = 0; burst < NUM_BURSTS; burst++) {
      int sleep_start = uptime();
      pause(SLEEP_TIME); // Simulate I/O wait
      int wake_time = uptime();

      // Do some work (increased for better measurement)
      long work;
      for (work = 0; work < 100000000; work++)
        ;

      int work_done_time = uptime();
      int latency = work_done_time - wake_time;

      printf("[INTERACTIVE] Burst %d: slept %d ticks, wakeup-to-work latency: "
             "%d ticks\n",
             burst, wake_time - sleep_start, latency);
    }

    exit(0);
  }

  // Parent waits for both children
  wait(0);
  wait(0);

  end_time = uptime();

  printf("\n=== TEST COMPLETE ===\n");
  printf("Total test duration: %d ticks\n", end_time - start_time);
  printf("\nWith PALS: Interactive process should show lower wakeup latency\n");
  printf("Without PALS: Higher variance in wakeup latency expected\n");

  exit(0);
}
