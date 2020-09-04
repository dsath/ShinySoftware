#ifndef TIMESTATE_HPP
#define TIMESTATE_HPP
#include "box.hpp"
enum Tstate { DAY_STATE, TWILIGHT_STATE, NIGHT_STATE };
class TimeState {
  private: 
    Tstate state;
    box Day[2];
    box Twilight[2];
    box Night[2];
    int hour;
    int min;

  public:
    TimeState();
    box CurState[2];
    void setState(); 
    void setState(Tstate s); 
    void printState();
    bool isTimeChange();
    inline Tstate getState() { return state; }
};
#endif
