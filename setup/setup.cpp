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

#include "../lib/my.h"


using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
  struct box one = {135, 225, 10};
  struct box two = {410, 70, 10};

  const int fps = 20;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(2);

  setBox(&vid, &one);
  setBox(&vid, &two);

  while (vid.read(frame)) {
    bgr1 = getBGR(&frame, one.col, one.row, one.height);
    addBlackBox(&frame, one.col, one.row, one.height);
    bgr2 = getBGR(&frame, two.col, two.row, two.height);
    addBlackBox(&frame, two.col, two.row, two.height);
    imshow("window", frame);
    cout << bgr1[0] << std::endl;
    cout << bgr1[1] << std::endl;
    cout << bgr1[2] << std::endl;
    cvWaitKey( 1000 / fps); 
  }
  return 1;
}
