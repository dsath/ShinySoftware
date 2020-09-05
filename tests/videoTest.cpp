#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include "../lib/my.hpp"

using namespace cv;
using namespace std;

const int fps = 20;

//used for stopping A presses
static bool stopPressA = false;

void _stop(int sig) {
  if (sig == SIGUSR1) {
    stopPressA = !(stopPressA);
  }
}

int coord[3] {90, 310, 10};
/*
90
310
10
465
110
10*/

int main(int argv, char** argc)
{
  wiringPiSetup();
  initButtons();

  Mat frame;
  VideoCapture vid(stoi(argc[1]));
  int *bgr;


  if(!vid.isOpened())
  {
    cout << "Failed opening webcam" << std::endl;
    return -1;
  }

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

  softReset();
  while (vid.read(frame))
  {
    bgr = getBGR(&frame, coord[0], coord[1], coord[2]);
    addBlackBox(&frame, coord[0], coord[1], coord[2]);
    imshow("Webcam", frame);
    cout << "values: " << bgr[0] << " -- " << bgr[1] << " -- " << bgr[2] << endl;
    free(bgr);
    if(cvWaitKey( 1000 / fps) >= 0) {
      break;
    }
  }
  return 1;
}
