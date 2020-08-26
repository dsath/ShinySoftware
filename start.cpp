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

#include "lib/my.h"

using namespace cv;
using namespace std;



int main(int argv, char** argc)
{
  ifstream file;
  file.open("setup/data.txt");
  
  //box coordinates and size
  int c1;
  int r1;
  int h1;
  int c2;
  int r2;
  int h2;

  //bgr of boxes
  int blue1;
  int green1;
  int red1;
  int blue2;
  int green2;
  int red2;

  //get box coordinates and size
  file >> c1;
  file >> r1;
  file >> h1;
  file >> c2;
  file >> r2;
  file >> h2;

  //get bgr of boxes
  file >> blue1;
  file >> green1;
  file >> red1;
  file >> blue2;
  file >> green2;
  file >> red2;

  //box location in frame, size of box, and bgr under that box
  const box one = {c1, r1, h1, blue1, green1, red1};
  const box two = {c2, r2, h2, blue2, green2, red2};

  //confidence for getting values
  const int conf1 = 10;
  const int conf2 = 50;

  int numResets = 0;

  //Open ttyUSB0 serial port for writing
  int fd;
  int n;
  
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1) { 
    //Check if open port failed
    perror("open_port: Unable to open /dev/ttyUSB0");
  } else {
    fcntl(fd, F_SETFL, 0);
  }


  const int fps = 20;

  int *temp;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(0); 
  make_480(&vid);

  while (vid.read(frame)) {
    bgr1 = getBGR(&frame, one.col, one.row, one.height);
    addBlackBox(&frame, one.col, one.row, one.height);
    imshow("window", frame);


    //Check if yellow cap appears
    if( inRange(one.blue - conf1, one.blue + conf1, bgr1[0]) && 
        inRange(one.green - conf1, one.green + conf1, bgr1[1]) && 
        inRange(one.red - conf1, one.red + conf1, bgr1[2])) {

      //Show current cap colors
      cout << "cap: " << bgr1[0] << "--" << bgr1[1] << "--" << bgr1[2] << endl;

      //send stop pressing A signal
      n = write(fd, "A", 1);
      if ( n < 0) cout << "Write of A failed" << std::endl;

      for(int i = 0; i < 35; i++) {
        vid.read(frame);
        //bgr2 = getBGR(&frame, two.row, two.col, two.height);
        //addBlackBox(&frame, two.col, two.row, two.height);
        imshow("window", frame);
        cvWaitKey(1000 / fps);
      }


      bgr2 = new int[3] {0};
      int acc = 10;
      for(int i = 0; i < acc; i++) {
        vid.read(frame);
        temp = getBGR(&frame, two.col, two.row, two.height);
        bgr2[0] += temp[0]; 
        bgr2[1] += temp[1]; 
        bgr2[2] += temp[2]; 
        addBlackBox(&frame, two.col, two.row, two.height);
        imshow("window", frame);
        free(temp);
        cvWaitKey(1000 / fps);
      }

      bgr2[0] = bgr2[0] / acc;
      bgr2[1] = bgr2[1] / acc;
      bgr2[2] = bgr2[2] / acc;

        //current encounter BGR value
      cout << "poke: " << bgr2[0] << "--" << bgr2[1] << "--" << bgr2[2] << endl;

      //check if pokemon is shiny

      if (   inRange(two.blue - conf2, two.blue + conf2, bgr2[0]) &&
             inRange(two.green - conf2, two.green + conf2, bgr2[1]) &&
             inRange(two.red - conf2, two.red + conf2, bgr2[2])) {
        
        //send soft reset signal
        n = write(fd, "B", 1);
        if ( n < 0 ) cout << "Write of B failed" << std::endl;
        //number soft resets
        numResets++;
        cout << "Soft Resets: " << numResets << std::endl;
        cout << std::endl;
        //free bgr
        free(bgr1);
        free(bgr2);
       } else {
         free(bgr1);
         free(bgr2);

         cout << "Shiny has been found!!!" << std::endl; 
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

