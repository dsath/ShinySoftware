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

#include "lib/my.hpp"
#include "lib/box.hpp"
#include "lib/TimeState.hpp"

#include <chrono>
#include <thread>

using namespace cv;
using namespace std;



int main(int argv, char** argc)
{
  //TimeState object used to keep track of day/night cycles
  TimeState state;

  //confidence for getting values
  const int conf1 = 10;
  const int conf2 = 20;

  int numResets = 0;

  //Open ttyUSB0 serial port for writing
  int fd;
  int n;
  
  //set frames per second
  const int fps = 20;

  int *temp;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) { 
    //Check if open port failed
    perror("open_port: Unable to open /dev/ttyUSB0");
  } else {
    fcntl(fd, F_SETFL, 0);
  }
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  //selects camera from command line
  VideoCapture vid(stoi(argc[1])); 
  //make resolution 480
  make_480(&vid);


  while (vid.read(frame)) {
    //set state of program based off current time
    state.setState();

    //wait till time has changed completely
    if(state.isTimeChange()) {
      //stop pressing A
      n = write(fd, "A", 1);
      if ( n < 0) cout << "Write of A failed" << std::endl;
      cout << "Waiting..." << endl;
      std::this_thread::sleep_for(std::chrono::minutes(6));
      //send soft reset signal
      n = write(fd, "B", 1);
      if ( n < 0 ) cout << "Write of B failed" << std::endl;
      continue;
    }

    bgr1 = getBGR(&frame, state.CurState[0].col, state.CurState[0].row, state.CurState[0].height);
    addBlackBox(&frame, state.CurState[0].col, state.CurState[0].row, state.CurState[0].height);
    imshow("window", frame);

    //Check if yellow cap appears
    if( inRange(state.CurState[0].blue - conf1, state.CurState[0].blue + conf1, bgr1[0]) && 
        inRange(state.CurState[0].green - conf1, state.CurState[0].green + conf1, bgr1[1]) && 
        inRange(state.CurState[0].red - conf1, state.CurState[0].red + conf1, bgr1[2])) {


      //Show CurStateent cap colors
      cout << "cap: " << bgr1[0] << "--" << bgr1[1] << "--" << bgr1[2] << endl;

      //send stop pressing A signal
      n = write(fd, "A", 1);
      if ( n < 0) cout << "Write of A failed" << std::endl;

      for(int i = 0; i < 30; i++) {
        vid.read(frame);
        imshow("window", frame);
        cvWaitKey(1000 / fps);
      }

      //get poke bgr value with 10 frame accuracry
      bgr2 = new int[3] {0};
      for(int i = 0; i < 10; i++) {
        vid.read(frame);
        temp = getBGR(&frame, state.CurState[1].col, state.CurState[1].row, state.CurState[1].height);
        bgr2[0] += temp[0]; 
        bgr2[1] += temp[1]; 
        bgr2[2] += temp[2]; 
        addBlackBox(&frame, state.CurState[1].col, state.CurState[1].row, state.CurState[1].height);
        imshow("window", frame);
        free(temp);
        cvWaitKey(1000 / fps);
      }
      bgr2[0] = bgr2[0] / 10;
      bgr2[1] = bgr2[1] / 10;
      bgr2[2] = bgr2[2] / 10;

        //CurStateent encounter BGR value
      cout << "poke: " << bgr2[0] << "--" << bgr2[1] << "--" << bgr2[2] << endl;

      //check if pokemon is shiny

      if (   inRange(state.CurState[1].blue - conf2, state.CurState[1].blue + conf2, bgr2[0]) &&
             inRange(state.CurState[1].green - conf2, state.CurState[1].green + conf2, bgr2[1]) &&
             inRange(state.CurState[1].red - conf2, state.CurState[1].red + conf2, bgr2[2])) {
        
        //send soft reset signal
        n = write(fd, "B", 1);
        if ( n < 0 ) cout << "Write of B failed" << std::endl;
        //number soft resets
        numResets++;
        cout << "Soft Resets: " << numResets << std::endl;
        cout << std::endl;

        //set new target colors for time of day based off new readings
        state.setNowTargetColor(bgr1, bgr2);

        //passes bright soft reset screen then resumes
        for(int i = 0; i < 50; i++) {
          vid.read(frame);
          imshow("window", frame);
          cvWaitKey(1000 / fps);
        }

        //free bgr
        free(bgr1);
        free(bgr2);
       } else {
         cout << "Shiny has been found!!!" << std::endl; 
         cout << "poke: " << bgr2[0] << "--" << bgr2[1] << "--" << bgr2[2] << endl;
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

