
#include <iostream>

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>


void rec(int sig) {
  if(sig == SIGUSR1) std::cout << "Received signal" << std::endl;
}

int main() {
  signal(SIGUSR1, rec);

  pid_t pid = fork();

  if(pid < 0) {
    std::cout << "error" << std::endl;
  }
  else if(pid == 0) {
    while(1) {

    }
  }
  else {
    while(1) {
      usleep(100000);
      kill(pid, SIGUSR1);
    }
  }
}
