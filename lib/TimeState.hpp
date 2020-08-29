#ifndef TIMESTATE_HPP
#define TIMESTATE_HPP
#include "box.hpp"
enum Tstate { DAY_STATE, NIGHT_STATE };
class TimeState {
  private: 
    Tstate state;
    box Day[2];
    box Night[2];
    int hour;
    int min;

  public:
    TimeState();
    box CurState[2];
    void setState(); 
    void printState();
};
#endif
