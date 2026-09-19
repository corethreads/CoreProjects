/******************************************************************************
 *                                                                            *
 *  ##  Project      : ArenaAllocator                                         *
 *  ##  File         : main.c                                                 *
 *  ##  Description  : Entry point and core logic for the application.        *
 *  ##  Author       : Leonne Kamau <leonnekamau28@gmail.com>                 *
 *  ##  Created      : 2026-09-18                                             *
 *  ##  Modified     : 2026-09- 19                                            *
 *  ##  Version      : 0.0.1                                                  *
 *  ##  License      : NULL                                                   *
 *                                                                            *
 ******************************************************************************/
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned char *buffer;
  size_t capacity;
  size_t used;
} Arena;

Arena *Init_arena(size_t capacity) {

  Arena *create_arena = malloc(sizeof(Arena));

  if (!create_arena)
    return NULL;

  create_arena->buffer = malloc(capacity);

  if (!create_arena->buffer) {
    perror("Error allocating capacity in buffer\n");
    return NULL;
  };
  create_arena->capacity = capacity;

  create_arena->used = 0;

  return create_arena;
};

void *arena_Allocation(Arena *arena, size_t object) {

  if (arena->used + object > arena->capacity) {
    perror("cant allocate, we have less capacity in buffer\n");
    return NULL;
  };

  void *ptr = arena->buffer + arena->used;

  arena->used += object;

  return ptr;
}

void arena_DeAllocate(Arena *arena) {
  free(arena->buffer);
  free(arena);
}

int main(void) {

  Arena *myArena = Init_arena(40);
  if (!myArena)
    return 1;
  char *name = arena_Allocation(myArena, 10);
  strcpy(name, "Leonne");

  printf("name %s\n ", name);
  arena_DeAllocate(myArena);
  return 0;
}
