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
#include "lib/NDS.hpp"


int main(int argv, char** argc)
{
  //TimeState object used to keep track of target BGR through day/night cycles
  TimeState state;
  //Controls Nintendo DS by using RPI4 GPIO pins. Starts a process for pressing A.
  NDS nds;

  //for keeping track of time
  time_t n;
  char *now;

  //confidence for getting values
  int conf1 = std::stoi(argc[2]);
  int conf2 = std::stoi(argc[3]);

  int numResets = 0;

  //set frames per second
  const int fps = 20;

  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;


  cv::Mat frame;
  cv::namedWindow("window", CV_WINDOW_AUTOSIZE);
  //selects camera from command line
  cv::VideoCapture vid(std::stoi(argc[1])); 
  //make resolution 480
  make_480(&vid);

  n = time(0);
  now = ctime(&n);

  std::cout << "The date and time is: " << now;
  std::cout << "Starting hunt... ";
  std::cout << std::endl << std::endl;

  nds.softReset();
  nds.startPressA();
  while (vid.read(frame)) {

#ifdef TIMECHANGE
    //set state of program based off current time
    state.setState();
    //wait till time has changed completely
    if(state.isTimeChange()) {
      //stop pressing A
      kill(pid, SIGUSR1);
      std::cout << "Waiting..." << std::endl;
      std::this_thread::sleep_for(std::chrono::minutes(6));
      //Soft reset and passes bright soft reset screen then resumes
      softReset();
      showFrames(50, &vid, &frame, fps);
      //start pressing A again
      kill(pid, SIGUSR1);
      continue;
    }

    //skip twilight state
    if(state.getState() == TWILIGHT_STATE) {
      kill(pid, SIGUSR1);
      std::cout << "Waiting..." << std::endl;
      std::this_thread::sleep_for(std::chrono::hours(3));
      std::this_thread::sleep_for(std::chrono::minutes(5));
      softReset();
      showFrames(50, &vid, &frame, fps);
      kill(pid, SIGUSR1);
      continue;
    }
#endif

#ifndef TIMECHANGE
    state.setState(CAVE_STATE);
#endif


    bgr1 = getBGR(&frame, state.CurState[0].col, state.CurState[0].row, state.CurState[0].height);
    addBlackBox(&frame, state.CurState[0].col, state.CurState[0].row, state.CurState[0].height);
    cv::imshow("window", frame);

    //Check if yellow cap appears
    if( inRange(state.CurState[0].blue - conf1, state.CurState[0].blue + conf1, bgr1[0]) && 
        inRange(state.CurState[0].green - conf1, state.CurState[0].green + conf1, bgr1[1]) && 
        inRange(state.CurState[0].red - conf1, state.CurState[0].red + conf1, bgr1[2])) {


      //send stop pressing A signal
      nds.stopPressA();

      showFrames(30, &vid, &frame, fps);

      //get poke bgr value with 10 frame accuracry
      bgr2 = getAverageColorFrames(10, &vid, &frame, &state, fps);

        //CurStateent encounter BGR value
      std::cout << "poke: " << bgr2[0] << "--" << bgr2[1] << "--" << bgr2[2] << std::endl;

      //check if pokemon is shiny

      if (   inRange(state.CurState[1].blue - conf2, state.CurState[1].blue + conf2, bgr2[0]) &&
             inRange(state.CurState[1].green - conf2, state.CurState[1].green + conf2, bgr2[1]) &&
             inRange(state.CurState[1].red - conf2, state.CurState[1].red + conf2, bgr2[2])) {
        
        //not shiny, soft reset
        nds.softReset();
        //number soft resets
        numResets++;
        std::cout << "Soft Resets: " << numResets << std::endl << std::endl;

        //passes bright soft reset screen then resumes
        showFrames(50, &vid, &frame, fps);

        //start pressing A again
        nds.startPressA();

        //free bgr
        free(bgr1);
        free(bgr2);
       } else {
         n = time(0);
         now = ctime(&n);
         std::cout << "Shiny has been found!!!" << std::endl; 
         std::cout << "The current data and time is: " << now << std::endl;
         free(bgr1);
         free(bgr2);
         //endless loop of reading frames
         while(vid.read(frame)) {
           cv::imshow("window", frame);
           cv::waitKey(1000 / fps);
         }
         break;
       }
    } else {
      //free cap bgr
      free(bgr1);
    }
    cv::waitKey(1000 / fps);
  }
  return EXIT_SUCCESS;
}

