#include <iostream>
#include <fstream>
#include <assert.h>
#include <ctime>
#include "../lib/TimeState.hpp"

using namespace std;

int main() {
  time_t n = time(0);
  tm *now = localtime(&n);

  ifstream coords("../setup/coords.txt");
  ifstream day("../setup/day.txt");
  ifstream night("../setup/night.txt");
  TimeState time;
  time.setState();
  int data;
  
  //check coords.txt matches 
  coords >> data;
  assert(time.CurState[0].col == data);
  coords >> data;
  assert(time.CurState[0].row == data);
  coords >> data;
  assert(time.CurState[0].height == data);
  coords >> data;
  assert(time.CurState[1].col == data);
  coords >> data;
  assert(time.CurState[1].row == data);
  coords >> data;
  assert(time.CurState[1].height == data);

  //state changes based off current hour
  if(now->tm_hour >= 20 || now->tm_hour < 4) {
    //check night.txt matches
    night >> data;
    assert(time.CurState[0].blue == data);
    night >> data;
    assert(time.CurState[0].green == data);
    night >> data;
    assert(time.CurState[0].red == data);
    night >> data;
    assert(time.CurState[1].blue == data);
    night >> data;
    assert(time.CurState[1].green == data);
    night >> data;
    assert(time.CurState[1].red == data);

  } else {
    //check day.txt matches
    day >> data;
    assert(time.CurState[0].blue == data);
    day >> data;
    assert(time.CurState[0].green == data);
    day >> data;
    assert(time.CurState[0].red == data);
    day >> data;
    assert(time.CurState[1].blue == data);
    day >> data;
    assert(time.CurState[1].green == data);
    day >> data;
    assert(time.CurState[1].red == data);
  }
  cout << "All tests passed!" << endl;
}


