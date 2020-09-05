#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>

#include <stdint.h>
#include <iostream>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <ctime>

#include <wiringPi.h>
#include <sys/types.h>
#include <signal.h>

#include <chrono>
#include <thread>


#include "lib/my.hpp"
#include "lib/box.hpp"
#include "lib/TimeState.hpp"


#define buttonA 0
#define buttonStart 2
#define buttonSelect 3

using namespace cv;
using namespace std;

//used for stopping A presses
static bool stopPressA = false;

void _stop(int sig) {
  if (sig == SIGUSR1) {
    stopPressA = !(stopPressA);
  }
}

int main(int argv, char** argc)
{
  //TimeState object used to keep track of day/night cycles
  TimeState state;

  wiringPiSetup();
  initButtons();

  //for keeping track of time
  time_t n;
  char *now;

  //confidence for getting values
  const int conf1 = 5;
  const int conf2 = 10;

  int numResets = 0;

  //set frames per second
  const int fps = 20;

  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;


  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  //selects camera from command line
  VideoCapture vid(stoi(argc[1])); 
  //make resolution 480
  make_480(&vid);

  signal(SIGUSR1, _stop);

  //start A pressing process
  pid_t pid = fork();
  if(pid == -1) {
    printf("Error when forking");
  }
  else if (pid == 0) {
    while(1) {
      //continuously press A until signal received
      while(stopPressA); 
      pressA();
    }
  }

  n = time(0);
  now = ctime($n);

  std::cout << "The date and time is: " << now << std::endl;
  std::cout << "Starting hunt... "  << std::endl;

  softReset();
  while (vid.read(frame)) {
    //signal to stop or continue pressing A to proccess
    //set state of program based off current time
#ifdef TIMECHANGE
    state.setState();
#endif

#ifndef TIMECHANGE
    state.setState(CAVE_STATE);
#endif

#ifdef TIMECHANGE
    //wait till time has changed completely
    if(state.isTimeChange()) {
      //stop pressing A
      kill(pid, SIGUSR1);
      cout << "Waiting..." << endl;
      std::this_thread::sleep_for(std::chrono::minutes(6));
      //Soft reset and passes bright soft reset screen then resumes
      softReset();
      showFrames(50, &vid, &frame);
      //start pressing A again
      kill(pid, SIGUSR1);
      continue;
    }

    //skip twilight state
    if(state.getState() == TWILIGHT_STATE) {
      kill(pid, SIGUSR1);
      cout << "Waiting..." << endl;
      std::this_thread::sleep_for(std::chrono::hours(3));
      std::this_thread::sleep_for(std::chrono::minutes(5));
      softReset();
      showFrames(50, &vid, &frame);
      kill(pid, SIGUSR1);
      continue;
    }
#endif


    bgr1 = getBGR(&frame, state.CurState[0].col, state.CurState[0].row, state.CurState[0].height);
    addBlackBox(&frame, state.CurState[0].col, state.CurState[0].row, state.CurState[0].height);
    imshow("window", frame);

    //Check if yellow cap appears
    if( inRange(state.CurState[0].blue - conf1, state.CurState[0].blue + conf1, bgr1[0]) && 
        inRange(state.CurState[0].green - conf1, state.CurState[0].green + conf1, bgr1[1]) && 
        inRange(state.CurState[0].red - conf1, state.CurState[0].red + conf1, bgr1[2])) {


      //send stop pressing A signal
      kill(pid, SIGUSR1);

      showFrames(30, &vid, &frame);

      //get poke bgr value with 10 frame accuracry
      bgr2 = getAverageColorFrames(10, &vid, &frame);

        //CurStateent encounter BGR value
      cout << "poke: " << bgr2[0] << "--" << bgr2[1] << "--" << bgr2[2] << endl;

      //check if pokemon is shiny

      if (   inRange(state.CurState[1].blue - conf2, state.CurState[1].blue + conf2, bgr2[0]) &&
             inRange(state.CurState[1].green - conf2, state.CurState[1].green + conf2, bgr2[1]) &&
             inRange(state.CurState[1].red - conf2, state.CurState[1].red + conf2, bgr2[2])) {
        
        //number soft resets
        numResets++;
        cout << "Soft Resets: " << numResets << std::endl << std::endl;

        //passes bright soft reset screen then resumes
        softReset();
        showFrames(50, &vid, &frame);

        kill(pid, SIGUSR1);

        //free bgr
        free(bgr1);
        free(bgr2);
       } else {
         n = time(0);
         now = ctime($n);
         std::cout << "Shiny has been found!!!" << std::endl; 
         std::cout << "The current data and time is: " << now << std::endl;
         free(bgr1);
         free(bgr2);
         //endless loop of reading frames
         while(vid.read(frame)) {
           imshow("window", frame);
           cvWaitKey(1000 / fps);
         }
         break;
       }
    } else {
      //free cap bgr
      free(bgr1);
    }
    cvWaitKey(1000 / fps);
  }
  return EXIT_SUCCESS;
}

