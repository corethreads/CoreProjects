/******************************************************************************
 *                                                                            *
 *  ##  Project      : First Implementation of Threads                        *
 *  ##  File         : threadingI.c                                           *
 *  ##  Description  : First implementation of how multithreading was built   *
 *  ##  Author       : Leonne Kamau <leonnekamau28@gmail.com>                 *
 *  ##  Created      : 2026-09-25                                             *
 *  ##  Modified     : NULL                                                   *
 *  ##  Version      : 0.0.1                                                  *
 *  ##  License      : NULL                                                   *
 *                                                                            *
 ******************************************************************************/

#include <setjmp.h>
#include <stdio.h>
#define MAX_TASKS 2
#define u4 int

jmp_buf task_buffer[MAX_TASKS];
u4 current_task = 0;

// The manual schedular:manually swaps execution
void swapper() {
  int old_task = current_task;
  current_task = (current_task + 1) % MAX_TASKS;

  if (setjmp(task_buffer[old_task]) == 0) {
    longjmp(task_buffer[current_task], 1);
  }
}

void task_one() {
  for (u4 i = 0; i < 5; i++) {
    printf("Thread 1:working on step: %d\n", i + 1);
    swapper(); // Hand over control to thread 1
  }
}

void task_two() {
  for (u4 i = 0; i < 5; i++) {
    printf("Thread 2: working on step: %d\n", i + 1);
    swapper();
  }
}

int main() {

  if (setjmp(task_buffer[1]) == 0) {
    task_one();
  } else {
    task_two();
  }

  return 0;
}
