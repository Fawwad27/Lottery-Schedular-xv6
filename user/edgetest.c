#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv) {
  printf("Starting Edge Case Test...\n");

  // Test 1: Negative Tickets (Should fail)
  if (settickets(-10) < 0) {
    printf(" settickets(-10) rejected correctly.\n");
  } else {
    printf("[FAIL] settickets(-10) was accepted.\n");
  }

  // Test 2: Zero Tickets (Should fail, min is usually 1)
  if (settickets(0) < 0) {
    printf(" settickets(0) rejected correctly.\n");
  } else {
    printf("[FAIL] settickets(0) was accepted.\n");
  }

  // Test 3: Large Number (Should succeed)
  if (settickets(10000) == 0) {
    printf(" settickets(10000) accepted.\n");
  } else {
    printf("[FAIL] settickets(10000) failed unexpectedly.\n");
  }

  printf("Edge Case Test Finished.\n\n\n");
  exit(0);
}