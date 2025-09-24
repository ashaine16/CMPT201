// lab3.c — keeps the last 5 inputs; typing "print" shows them (including that
// "print")
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { HISTORY_SIZE = 5 };

typedef struct {
  char *buf[HISTORY_SIZE];
  int head; // index of oldest
  int len;  // number of stored lines
} History;

static char *dupstr(const char *s) {
  size_t n = strlen(s) + 1;
  char *p = malloc(n);
  if (!p) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  memcpy(p, s, n);
  return p;
}

static void history_init(History *h) {
  h->head = 0;
  h->len = 0;
  for (int i = 0; i < HISTORY_SIZE; ++i)
    h->buf[i] = NULL;
}

static void history_free(History *h) {
  for (int i = 0; i < h->len; ++i) {
    int idx = (h->head + i) % HISTORY_SIZE;
    free(h->buf[idx]);
    h->buf[idx] = NULL;
  }
  h->head = h->len = 0;
}

static void history_push(History *h, const char *line) {
  char *copy = dupstr(line);
  if (h->len < HISTORY_SIZE) {
    int idx = (h->head + h->len) % HISTORY_SIZE;
    h->buf[idx] = copy;
    h->len++;
  } else {
    free(h->buf[h->head]);
    h->buf[h->head] = copy;
    h->head = (h->head + 1) % HISTORY_SIZE;
  }
}

static void history_print(const History *h) {
  for (int i = 0; i < h->len; ++i) {
    int idx = (h->head + i) % HISTORY_SIZE;
    printf("%s\n", h->buf[idx]);
  }
}

int main(void) {
  History hist;
  history_init(&hist);

  char *line = NULL;
  size_t cap = 0;

  for (;;) {
    printf("Enter input: ");
    fflush(stdout);

    ssize_t n = getline(&line, &cap, stdin);
    if (n == -1) {
      putchar('\n');
      break;
    }

    if (n > 0 && line[n - 1] == '\n')
      line[n - 1] = '\0';

    history_push(&hist, line);

    if (strcmp(line, "print") == 0) {
      history_print(&hist);
    }
  }

  free(line);
  history_free(&hist);
  return 0;
}
