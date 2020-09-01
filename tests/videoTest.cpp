#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "../lib/my.hpp"

using namespace cv;
using namespace std;

const int fps = 20;


int coord[3] {45, 280, 10};

int main(int argv, char** argc)
{
  Mat frame;
  VideoCapture vid(stoi(argc[1]));
  int *bgr;

  if(!vid.isOpened())
  {
    cout << "Failed opening webcam" << std::endl;
    return -1;
  }

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
