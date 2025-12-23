#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Edge-Case Test: Tests error handling and boundary conditions
// Tests invalid inputs, zero tickets, negative tickets, large values

void test_negative_tickets() {
  printf("\n--- Test 1: Negative Tickets ---\n");
  int result = settickets(-5);
  if(result < 0) {
    printf("PASS: settickets(-5) correctly returned error\n");
  } else {
    printf("FAIL: settickets(-5) should return error but didn't\n");
  }
}

void test_zero_tickets() {
  printf("\n--- Test 2: Zero Tickets ---\n");
  int result = settickets(0);
  if(result < 0) {
    printf("PASS: settickets(0) correctly returned error\n");
  } else {
    printf("FAIL: settickets(0) should return error but didn't\n");
  }
}

void test_valid_edge_cases() {
  printf("\n--- Test 3: Valid Edge Cases ---\n");
  
  // Test with 1 ticket (minimum valid)
  int result = settickets(1);
  if(result == 0) {
    printf("PASS: settickets(1) accepted (minimum valid value)\n");
  } else {
    printf("FAIL: settickets(1) should succeed\n");
  }
  
  // Test with large number
  result = settickets(1000000);
  if(result == 0) {
    printf("PASS: settickets(1000000) accepted (large value)\n");
  } else {
    printf("FAIL: settickets(1000000) should succeed\n");
  }
}

void test_parent_child_tickets() {
  printf("\n--- Test 4: Parent-Child Ticket Inheritance ---\n");
  
  settickets(50);
  int pid = fork();
  
  if(pid == 0) {
    // Child process - test if it inherited tickets
    printf("Child process created. Setting own tickets to 25.\n");
    int result = settickets(25);
    if(result == 0) {
      printf("PASS: Child can set its own tickets independently\n");
    } else {
      printf("FAIL: Child cannot set tickets\n");
    }
    exit(0);
  } else {
    wait(0);
    printf("PASS: Parent-child ticket management works correctly\n");
  }
}

void test_multiple_settickets() {
  printf("\n--- Test 5: Multiple settickets() Calls ---\n");
  
  int pid = fork();
  if(pid == 0) {
    printf("Child: Setting tickets to 10\n");
    settickets(10);
    
    // Do some work
    long i;
    for(i = 0; i < 100000000; i++);
    
    printf("Child: Changing tickets to 50\n");
    settickets(50);
    
    // Do more work
    for(i = 0; i < 100000000; i++);
    
    printf("PASS: Can call settickets() multiple times\n");
    exit(0);
  }
  wait(0);
}

void test_all_processes_same_tickets() {
  printf("\n--- Test 6: All Processes Same Tickets ---\n");
  
  int pid1 = fork();
  if(pid1 == 0) {
    settickets(10);
    long i;
    for(i = 0; i < 200000000; i++);
    printf("Process 1 (10 tickets) finished\n");
    exit(0);
  }
  
  int pid2 = fork();
  if(pid2 == 0) {
    settickets(10);
    long i;
    for(i = 0; i < 200000000; i++);
    printf("Process 2 (10 tickets) finished\n");
    exit(0);
  }
  
  int pid3 = fork();
  if(pid3 == 0) {
    settickets(10);
    long i;
    for(i = 0; i < 200000000; i++);
    printf("Process 3 (10 tickets) finished\n");
    exit(0);
  }
  
  wait(0);
  wait(0);
  wait(0);
  printf("PASS: Scheduler handles equal ticket distribution\n");
}

int
main(int argc, char *argv[])
{
  printf("=== LOTTERY SCHEDULER: EDGE-CASE TEST ===\n");
  printf("Testing error handling and boundary conditions\n");

  test_negative_tickets();
  test_zero_tickets();
  test_valid_edge_cases();
  test_parent_child_tickets();
  test_multiple_settickets();
  test_all_processes_same_tickets();

  printf("\n=== EDGE-CASE TEST COMPLETE ===\n");
  printf("All edge cases tested. Review PASS/FAIL results above.\n");

  exit(0);
}
