#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *line = NULL;
  size_t cap = 0;

  printf("Enter text: ");
  fflush(stdout);

  ssize_t nread = getline(&line, &cap, stdin);
  if (nread == -1) {
    free(line);
    return 1;
  }

  printf("Tokens:\n");

  char *saveptr;
  char *tok = strtok_r(line, " \t\n", &saveptr);

  while (tok != NULL) {
    printf(" %s\n", tok);
    tok = strtok_r(NULL, " \t\n", &saveptr);
  }

  free(line);
  return 0;
}
