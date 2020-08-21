#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>

#include <stdint.h>
#include <iostream>

using namespace cv;
using namespace std;

bool inRange(int low, int high, int val); 
int* getBGR(Mat *f, int sr, int sc, int bh); 
void addBlackBox(Mat *f, int sr, int sc, int bh); 

struct box {
  int col;
  int row;
  int height;
};


int main(int argv, char** argc)
{
  const int pokeBGR[3] = {40, 80, 240};
  const int conf = 20;
  //Where in frame bgr value will be read
  box one = {125, 225, 15};
  box two = {410, 70, 15};

  const int fps = 20;
  //blue, green, red value for box one
  int *bgr1;
  //blue, green, red value for box two 
  int *bgr2;
 
  Mat frame;
  namedWindow("window", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(2); 

  while (vid.read(frame)) {
    bgr1 = getBGR(&frame, one.row, one.col, one.height);
    addBlackBox(&frame, one.row, one.col, one.height);
    imshow("window", frame);



    //Check if yellow cap appears
    if( inRange(0, 125, bgr1[0]) && inRange(200, 255, bgr1[1]) && inRange(200,255, bgr1[2])) {

      cout << "Found yellow cap" << std::endl;
      //send stop pressing A signal

      for(int i = 0; i < 100; i++) {
        vid.read(frame);
        //bgr2 = getBGR(&frame, two.row, two.col, two.height);
        addBlackBox(&frame, two.row, two.col, two.height);
        imshow("window", frame);
      }
        bgr2 = getBGR(&frame, two.row, two.col, two.height);
        addBlackBox(&frame, two.row, two.col, two.height);
        imshow("window", frame);
        cout << "blue: " << bgr2[0] << std::endl;
        cout << "green: " << bgr2[1] << std::endl;
        cout << "red: " << bgr2[2] << std::endl;

      //check if pokemon is shiny

      if (   inRange(pokeBGR[0] - conf, pokeBGR[0] + conf, bgr2[0]) &&
             inRange(pokeBGR[1] - conf, pokeBGR[1] + conf, bgr2[1]) &&
             inRange(pokeBGR[2] - conf, pokeBGR[2] + conf, bgr2[2])) {
        
        //send soft reset signal
        //send resume pressing A signal
       } else {
         cout << "Shiny has been found!!!" << std::endl; 
         break;
       }
    }

    if(cvWaitKey(1000 / fps) >= 0)
      break;
  }
  return 1;
}

int* getBGR(Mat *f, int sr, int sc, int bh) {
    int *vals = new int[3] {0};

    for (int r = sr; r < sr + bh; ++r) {
      for(int c = sc; c < sc + bh; ++c) {
        //store rgb values of current frame
        vals[0] += f->at<Vec3b>(r, c)[0]; 
        vals[1] += f->at<Vec3b>(r, c)[1]; 
        vals[2] += f->at<Vec3b>(r, c)[2];
      }
    }

    vals[0] /= (bh * bh);
    vals[1] /= (bh * bh);
    vals[2] /= (bh * bh);
    return vals;
}

void addBlackBox(Mat *f, int sr, int sc, int bh) {
    for (int r = sr; r < sr + bh; ++r) {
      for(int c = sc; c < sc + bh; ++c) {
        f->at<Vec3b>(r, c)[0] = 0; 
        f->at<Vec3b>(r, c)[1] = 0; 
        f->at<Vec3b>(r, c)[2] = 0;
      }
    }
}

bool inRange(int low, int high, int val) {
  if (low <= val && val <= high) {
    return true;
  } else {
    return false;
  }
}





