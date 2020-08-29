#include <iostream>
#include <fstream>
#include <ctime>

#include "TimeState.hpp"
#include "box.hpp"

using namespace std;

TimeState::TimeState() {

  //box coordinates and size
  int c1, r1, h1;
  int c2, r2, h2;
  //box color
  int dayBlue1, dayGreen1, dayRed1;
  int dayBlue2, dayGreen2, dayRed2;
  int nightBlue1, nightGreen1, nightRed1;
  int nightBlue2, nightGreen2, nightRed2;

  ifstream file;

  //get box coordinates and size
  {
    ifstream file;
    file.open("setup/coords.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << endl;
    }
    file >> c1; file >> r1; file >> h1;
    file >> c2; file >> r2; file >> h2;
    file.close();
  }

  //get bgr values for daytime
  {
    ifstream file;
    file.open("setup/day.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << endl;
    }
    file >> dayBlue1; file >> dayGreen1; file >> dayRed1;
    file >> dayBlue2; file >> dayGreen2; file >> dayRed2;
    file.close();
  }

  //get bgr values for nighttime
  {
    file.open("setup/night.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << endl;
    }
    file >> nightBlue1; file >> nightGreen1; file >> nightRed1;
    file >> nightBlue2; file >> nightGreen2; file >> nightRed2;
    file.close();
  }

  Day[0] = {c1, r1, h1, dayBlue1, dayGreen1, dayRed1}; 
  Day[1] = {c2, r2, h2, dayBlue2, dayGreen2, dayRed2}; 
  Night[0] = {c1, r1, h1, nightBlue1, nightGreen1, nightRed1}; 
  Night[1] = {c2, r2, h2, nightBlue2, nightGreen2, nightRed2}; 

}

void TimeState::setState() {
  time_t n = time(0);
  tm *now = localtime(&n);
  hour = now->tm_hour;
  min = now->tm_min;
  if(hour >= 20 || hour < 4) {
    state = NIGHT_STATE;
    CurState[0] = Night[0];
    CurState[1] = Night[1];
  } else {
    state = DAY_STATE;
    CurState[0] = Day[0];
    CurState[1] = Day[1];
  }
}

void TimeState::printState() {
  if (state == NIGHT_STATE) {
    cout << "It's night and the time is " << hour << ":" << min << endl;
  } else {
    cout << "It's day and the time is " << hour << ":" << min << endl;
  }
}

