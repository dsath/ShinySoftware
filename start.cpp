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

#include "lib/my.h"

using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
  const int pokeBGR[3] = {56, 97, 224};
  const int confP = 30;
  //const int hatColor[3] = {130, 194, 220};
  const int hatColor[3] = {175, 235, 245};
  const int confH = 10;

  int numResets = 0;

  //Open ttyUSB0 for writing
  int fd;
  int n;
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1) { 
    //Check if open port failed
    perror("open_port: Unable to open /dev/ttyUSB0");
  } else {
    fcntl(fd, F_SETFL, 0);
  }

  //Where in frame bgr value will be read
  box one = {135, 225, 10};
  box two = {410, 70, 10};

  const int fps = 20;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(2); 

  while (vid.read(frame)) {
    bgr1 = getBGR(&frame, one.col, one.row, one.height);
    addBlackBox(&frame, one.col, one.row, one.height);
    imshow("window", frame);

    //Check if yellow cap appears
    if( inRange(hatColor[0] - confH, hatColor[0] + confH, bgr1[0]) && 
        inRange(hatColor[1] - confH, hatColor[1] + confH, bgr1[1]) && 
        inRange(hatColor[2] - confH, hatColor[2] + confH, bgr1[2])) {

      //send stop pressing A signal
      n = write(fd, "A", 1);
      if ( n < 0) cout << "Write of A failed" << std::endl;

      for(int i = 0; i < 50; i++) {
        vid.read(frame);
        //bgr2 = getBGR(&frame, two.row, two.col, two.height);
        addBlackBox(&frame, two.col, two.row, two.height);
        imshow("window", frame);
        cvWaitKey(1000 / fps);
      }
        vid.read(frame);
        bgr2 = getBGR(&frame, two.col, two.row, two.height);
        addBlackBox(&frame, two.col, two.row, two.height);
        imshow("window", frame);
        //current encounter BGR value
        cout << "blue: " << bgr2[0] << std::endl;
        cout << "green: " << bgr2[1] << std::endl;
        cout << "red: " << bgr2[2] << std::endl;

      //check if pokemon is shiny

      if (   inRange(pokeBGR[0] - confP, pokeBGR[0] + confP, bgr2[0]) &&
             inRange(pokeBGR[1] - confP, pokeBGR[1] + confP, bgr2[1]) &&
             inRange(pokeBGR[2] - confP, pokeBGR[2] + confP, bgr2[2])) {
        
        //send soft reset signal
        n = write(fd, "B", 1);
        if ( n < 0 ) cout << "Write of B failed" << std::endl;
        //number soft resets
        numResets++;
        cout << "Soft Resets: " << numResets << std::endl;
        cout << std::endl;
        free(bgr1);
        free(bgr2);
       } else {
         cout << "Shiny has been found!!!" << std::endl; 
         break;
       }
    }

    cvWaitKey(1000 / fps);
  }
  return 1;
}

