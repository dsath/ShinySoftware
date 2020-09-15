#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "NDS.hpp"

bool NDS::pressingA = false;

NDS::NDS() {
  wiringPiSetup();
  initButtons();

  signal(SIGUSR1, processSignal);
  signal(SIGUSR2, processSignal);
  std::cout << "Starting proccess..." << std::endl;
  pid_t pid = fork();
  if(pid == -1) {
    std::cout << "Error when forking" << std::endl;
  }
  else if (pid == 0) {
    while(1) {
      //continuously press A until signal received
      while(!pressingA); 
      pressA();
    }
  }
  else {
    ps = pid;
  }
}

void NDS::pressA() {
  digitalWrite(buttonA, HIGH);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  digitalWrite(buttonA, LOW);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
}
void NDS::softReset() {
  digitalWrite(buttonStart, HIGH);
  digitalWrite(buttonSelect, HIGH);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  digitalWrite(buttonStart, LOW);
  digitalWrite(buttonSelect, LOW);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
}
void NDS::initButtons() {
  pinMode(buttonA, OUTPUT);
  pinMode(buttonSelect, OUTPUT);
  pinMode(buttonStart, OUTPUT);
  digitalWrite(buttonA, LOW);
  digitalWrite(buttonStart, LOW);
  digitalWrite(buttonSelect, LOW);
}

void NDS::processSignal(int sig) {
  if (sig == SIGUSR1) {
    pressingA = false;
  } 
  else if (sig == SIGUSR2) {
    pressingA = true;
  }
}

void NDS::stopPressA() {
  kill(ps, SIGUSR1);
}

void NDS::startPressA() {
  kill(ps, SIGUSR2);
}


