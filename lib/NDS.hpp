#ifndef NDS_HPP
#define NDS_HPP
#include <sys/types.h>
class NDS {
  private:
    static bool pressingA;
    pid_t ps;
    static const int buttonA = 0;
    static const int buttonStart = 2;
    static const int buttonSelect = 3;
    static void processSignal(int sig); 
  public:
    NDS();
    void pressA();
    void softReset();
    void initButtons();
    void startPressA(); 
    void stopPressA(); 
};
#endif
