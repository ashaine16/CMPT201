// minishell.c
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char line[1024];

  printf("Enter programs to run.\n");
  while (1) {
    printf("> ");
    fflush(stdout);

    // EOF (Ctrl+D) or read error -> exit
    if (fgets(line, sizeof(line), stdin) == NULL) {
      printf("\n");
      break;
    }

    // strip trailing newline (if any)
    size_t n = strlen(line);
    if (n > 0 && line[n - 1] == '\n')
      line[n - 1] = '\0';

    // skip empty lines
    if (line[0] == '\0')
      continue;

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
      // child: execute the program at the given path, no arguments
      // argv[0] conventionally is the program name; we’ll pass the path itself
      execl(line, line, (char *)NULL);
      // if we get here, exec failed
      // print exactly what the example shows:
      printf("Exec failure\n");
      _exit(127);
    } else {
      // parent: wait for the child to finish
      int status;
      (void)waitpid(pid, &status, 0);
    }
  }
  return 0;
}
