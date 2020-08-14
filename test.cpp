#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

#include <stdint.h>
#include <iostream>

using namespace cv;
using namespace std;

int* getBGR(Mat *f, int sr, int sc, int bh); 
void addBlackBox(Mat *f, int sr, int sc, int bh); 

int main(int argv, char** argc)
{
  const int startRow = 150;
  const int startCol = 150;
  const int boxHeight = 30;
  const int fps = 20;
  int *bgr;
 
  Mat frame;
  namedWindow("boxOne", CV_WINDOW_AUTOSIZE);
  VideoCapture vid(2); 

  while (vid.read(frame)) {
    bgr = getBGR(&frame, startRow, startCol, boxHeight);
    addBlackBox(&frame, startRow, startCol, boxHeight);

    cout << "blue: " << bgr[0] << std::endl;
    cout << "green: " << bgr[1] << std::endl;
    cout << "red: " << bgr[2] << std::endl;

    imshow("boxOne", frame);
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





