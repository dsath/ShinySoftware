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
  int twilightBlue1, twilightGreen1, twilightRed1;
  int twilightBlue2, twilightGreen2, twilightRed2;
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

  //get bgr values for twilight 
  {
    ifstream file;
    file.open("setup/twilight.txt");
    if(!(file.is_open())) {
      std::cout << "error opening file" << endl;
    }
    file >> twilightBlue1; file >> twilightGreen1; file >> twilightRed1;
    file >> twilightBlue2; file >> twilightGreen2; file >> twilightRed2;
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
  Twilight[0] = {c1, r1, h1, twilightBlue1, twilightGreen1, twilightRed1}; 
  Twilight[1] = {c2, r2, h2, twilightBlue2, twilightGreen2, twilightRed2}; 
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

void TimeState::printState() {
  if (state == NIGHT_STATE) {
    cout << "It's night and the time is " << hour << ":" << min << endl;
  } 
  else if( state == TWILIGHT_STATE) {
    cout << "It's twilight and the time is " << hour << ":" << min << endl;
  } 
  else {
    cout << "It's day and the time is " << hour << ":" << min << endl;
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

int TimeState::getHalfDiff(int x, int y) {
  return (x - y) / 2;
}

void TimeState::setNowTargetColor(int *bgr1, int *bgr2) {
  if( state == DAY_STATE)  {
    Day[0].blue  += getHalfDiff(bgr1[0], Day[0].blue);
    Day[0].green += getHalfDiff(bgr1[1], Day[0].green);
    Day[0].red   += getHalfDiff(bgr1[2], Day[0].red);

    Day[1].blue  += getHalfDiff(bgr2[0], Day[1].blue);
    Day[1].green += getHalfDiff(bgr2[1], Day[1].green);
    Day[1].red   += getHalfDiff(bgr2[2], Day[1].red);
  } 
  else if ( state == TWILIGHT_STATE) {
    Twilight[0].blue  += getHalfDiff(bgr1[0], Twilight[0].blue);
    Twilight[0].green += getHalfDiff(bgr1[1], Twilight[0].green);
    Twilight[0].red   += getHalfDiff(bgr1[2], Twilight[0].red);

    Twilight[1].blue  += getHalfDiff(bgr2[0], Twilight[1].blue);
    Twilight[1].green += getHalfDiff(bgr2[1], Twilight[1].green);
    Twilight[1].red   += getHalfDiff(bgr2[2], Twilight[1].red);
  }
  else {
    Night[0].blue  += getHalfDiff(bgr1[0], Night[0].blue);
    Night[0].green += getHalfDiff(bgr1[1], Night[0].green);
    Night[0].red   += getHalfDiff(bgr1[2], Night[0].red);

    Night[1].blue  += getHalfDiff(bgr2[0], Night[1].blue);
    Night[1].green += getHalfDiff(bgr2[1], Night[1].green);
    Night[1].red   += getHalfDiff(bgr2[2], Night[1].red);
  }
}

