#include <chrono>
#include <thread>
#include <wiringPi.h>
#include "wp.hpp"

void pressA() {
  digitalWrite(buttonA, HIGH);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  digitalWrite(buttonA, LOW);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
}

void softReset() {
  digitalWrite(buttonStart, HIGH);
  digitalWrite(buttonSelect, HIGH);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  digitalWrite(buttonStart, LOW);
  digitalWrite(buttonSelect, LOW);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
}
void initButtons() {
  pinMode(buttonA, OUTPUT);
  pinMode(buttonSelect, OUTPUT);
  pinMode(buttonStart, OUTPUT);
  digitalWrite(buttonA, LOW);
  digitalWrite(buttonStart, LOW);
  digitalWrite(buttonSelect, LOW);
}

