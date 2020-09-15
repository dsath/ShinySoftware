#include <iostream>
#include <fstream>
#include <ctime>

#include "TimeState.hpp"
#include "box.hpp"

TimeState::TimeState() {

  //box coordinates and size
  int c1, r1, h1;
  int c2, r2, h2;
  //box color
  int dayBlue1, dayGreen1, dayRed1;
  int dayBlue2, dayGreen2, dayRed2;
  int twilightBlue1, twilightGreen1, twilightRed1;
  int twilightBlue2, twilightGreen2, twilightRed2;
  int nightBlue1, nightGreen1, nightRed1;
  int nightBlue2, nightGreen2, nightRed2;
  int caveBlue1, caveGreen1, caveRed1;
  int caveBlue2, caveGreen2, caveRed2;


  //get box coordinates and size
  {
    std::ifstream file;
    file.open("setup/coords.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << std::endl;
    }
    file >> c1; file >> r1; file >> h1;
    file >> c2; file >> r2; file >> h2;
    file.close();
  }

  //get bgr values for daytime
  {
    std::ifstream file;
    file.open("setup/day.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << std::endl;
    }
    file >> dayBlue1; file >> dayGreen1; file >> dayRed1;
    file >> dayBlue2; file >> dayGreen2; file >> dayRed2;
    file.close();
  }

  //get bgr values for twilight 
  {
    std::ifstream file;
    file.open("setup/twilight.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << std::endl;
    }
    file >> twilightBlue1; file >> twilightGreen1; file >> twilightRed1;
    file >> twilightBlue2; file >> twilightGreen2; file >> twilightRed2;
    file.close();
  }

  //get bgr values for nighttime
  {
    std::ifstream file;
    file.open("setup/night.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << std::endl;
    }
    file >> nightBlue1; file >> nightGreen1; file >> nightRed1;
    file >> nightBlue2; file >> nightGreen2; file >> nightRed2;
    file.close();
  }

  //get bgr values for cave
  {
    std::ifstream file;
    file.open("setup/cave.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << std::endl;
    }
    file >> caveBlue1; file >> caveGreen1; file >> caveRed1;
    file >> caveBlue2; file >> caveGreen2; file >> caveRed2;
    file.close();
  }
  Day[0] = {c1, r1, h1, dayBlue1, dayGreen1, dayRed1}; 
  Day[1] = {c2, r2, h2, dayBlue2, dayGreen2, dayRed2}; 
  Twilight[0] = {c1, r1, h1, twilightBlue1, twilightGreen1, twilightRed1}; 
  Twilight[1] = {c2, r2, h2, twilightBlue2, twilightGreen2, twilightRed2}; 
  Night[0] = {c1, r1, h1, nightBlue1, nightGreen1, nightRed1}; 
  Night[1] = {c2, r2, h2, nightBlue2, nightGreen2, nightRed2}; 
  Cave[0] = {c1, r1, h1, caveBlue1, caveGreen1, caveRed1}; 
  Cave[1] = {c2, r2, h2, caveBlue2, caveGreen2, caveRed2}; 

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
  } 
  else if (hour >= 17 && hour < 20) {
    state = TWILIGHT_STATE;
    CurState[0] = Twilight[0];
    CurState[1] = Twilight[1];
  } else {
    state = DAY_STATE;
    CurState[0] = Day[0];
    CurState[1] = Day[1];
  }
}

void TimeState::setState(Tstate s) {
  if (s == NIGHT_STATE) { 
    state = NIGHT_STATE;
    CurState[0] = Night[0];
    CurState[1] = Night[1];
  }
  else if (s == TWILIGHT_STATE) {
    state = TWILIGHT_STATE;
    CurState[0] = Twilight[0];
    CurState[1] = Twilight[1];
  } 
  else if (s == DAY_STATE) {
    state = DAY_STATE;
    CurState[0] = Day[0];
    CurState[1] = Day[1];
  }
  else if (s == CAVE_STATE) {
    state = CAVE_STATE;
    CurState[0] = Cave[0];
    CurState[1] = Cave[1];
  }
  else {
    std::cout << "Error: State Doesn't Exist" << std::endl;
  }
}

bool TimeState::isTimeChange() {
  bool changing = false;
  if(( hour == 19 && min >= 57) ||
     (hour == 16 && min >= 57)  ||
     (hour == 3 && min >= 57)) {   
    changing = true;
  } 
  return changing;
}

