#include <stdio.h>
#include <sys/time.h>
#include <time.h>

const char *REQ_FIFO_PATH = "/home/keni/Projects/fifo-experiment/fifo_req";
const char *RES_FIFO_PATH = "/home/keni/Projects/fifo-experiment/fifo_res";

void LOG(const char *s) {
}


char getTimeBuf[4096];
const char *GetTime() {
  struct timeval tv;
  struct tm *t;
  gettimeofday(&tv, NULL);
  t = localtime(&tv.tv_sec);
  
  sprintf(getTimeBuf, "%02d:%02d:%02d.%03d",
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            (int)tv.tv_usec / 1000);
  return getTimeBuf;;
}

int main(int argc, char *argv[]) {
  FILE *f_req = fopen(REQ_FIFO_PATH, "r");
  FILE *f_res = fopen(RES_FIFO_PATH, "w");

  const int kBufSize = 4096;
  char buf[kBufSize];
  while (fgets(buf, kBufSize, f_req)) {
    fprintf(f_res, "[%s] Received by callee: %s", GetTime(), buf);
    fflush(f_res);
  }

  return 0;
}
