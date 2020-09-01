#include <iostream>
#include <fstream>
#include <assert.h>
#include <ctime>
#include "../lib/TimeState.hpp"

using namespace std;

int main() {
  //test 1
  {
    time_t n = time(0);
    tm *now = localtime(&n);
    ifstream coords("../setup/coords.txt");
    ifstream day("../setup/day.txt");
    ifstream night("../setup/night.txt");
    TimeState state;
    state.setState();
    int data;
    
    //check coords.txt matches 
    coords >> data;
    assert(state.CurState[0].col == data);
    coords >> data;
    assert(state.CurState[0].row == data);
    coords >> data;
    assert(state.CurState[0].height == data);
    coords >> data;
    assert(state.CurState[1].col == data);
    coords >> data;
    assert(state.CurState[1].row == data);
    coords >> data;
    assert(state.CurState[1].height == data);

    //state changes based off current hour
    if(now->tm_hour >= 20 || now->tm_hour < 4) {
      //check night.txt matches
      night >> data;
      assert(state.CurState[0].blue == data);
      night >> data;
      assert(state.CurState[0].green == data);
      night >> data;
      assert(state.CurState[0].red == data);
      night >> data;
      assert(state.CurState[1].blue == data);
      night >> data;
      assert(state.CurState[1].green == data);
      night >> data;
      assert(state.CurState[1].red == data);

    } else {
      //check day.txt matches
      day >> data;
      assert(state.CurState[0].blue == data);
      day >> data;
      assert(state.CurState[0].green == data);
      day >> data;
      assert(state.CurState[0].red == data);
      day >> data;
      assert(state.CurState[1].blue == data);
      day >> data;
      assert(state.CurState[1].green == data);
      day >> data;
      assert(state.CurState[1].red == data);
    }
    coords.close();
    day.close();
    night.close();
  }
  //test2
  {
    TimeState state;
    state.setState();
    int bgr1[3] {1, 2, 3};
    int bgr2[3] {4, 5, 6};
    box before[2];
    box after[2];

    before[0] = state.CurState[0];
    before[1] = state.CurState[1];
    state.setNowTargetColor(bgr1, bgr2);
    state.setState();
    after[0] = state.CurState[0];
    after[1] = state.CurState[1];

    assert(before[0].blue != after[0].blue);
    assert(before[0].green != after[0].green);
    assert(before[0].red != after[0].red);
    assert(before[1].blue != after[1].blue);
    assert(before[1].green != after[1].green);
    assert(before[1].red != after[1].red);
    assert(after[0].blue  == 1);
    assert(after[0].green == 2);
    assert(after[0].red   == 3);
    assert(after[1].blue  == 4);
    assert(after[1].green == 5);
    assert(after[1].red   == 6);
  }
  cout << "All tests passed!" << endl;
}


