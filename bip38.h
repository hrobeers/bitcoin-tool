#include <termios.h>
#include <stdio.h>

size_t
passwd_prompt (char *buf, size_t n, const char *prompt)
{
  struct termios old, new;
  char* result = NULL;
  FILE* stream = stdin;

  fprintf(stderr, prompt);

  /* Turn echoing off and fail if we can't. */
  if (tcgetattr (fileno (stream), &old) != 0)
    return 0;
  new = old;
  new.c_lflag &= ~ECHO;
  if (tcsetattr (fileno (stream), TCSAFLUSH, &new) != 0)
    return 0;

  /* Read the password. */
  result = fgets(buf, n, stream);

  /* Restore terminal. */
  (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);
  fputc('\n', stderr);

  /* Replace newline by null */
  if (!result) return 0;
  size_t len = strlen(result)-1;
  if (len > n-1) return 0;
  result[len] = 0;

  return len;
}

void passwd_clear(char* buf, size_t size)
{
  int i=0;
  for (; i<size; i++)
    buf[i] = 0;
}
