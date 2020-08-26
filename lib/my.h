#ifndef MY_H
#define MY_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

struct box {
  int col;
  int row;
  int height;
  int blue;
  int green;
  int red;
};

void make_1080(VideoCapture *v);
void make_720(VideoCapture *v);
void make_480(VideoCapture *v);
int* getBGR(Mat *f, int sc, int sr, int bh);
void addBlackBox(Mat *f, int sc, int sr, int bh);
bool inRange(int low, int high, int val);
void setBox(VideoCapture *v, box *b);
void setBoxI (Mat f, box *b); 
#endif
