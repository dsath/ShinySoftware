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
};
int* getBGR(Mat *f, int sc, int sr, int bh);
void addBlackBox(Mat *f, int sc, int sr, int bh);
bool inRange(int low, int high, int val);
void setBox(VideoCapture *v, box *b);
#endif
