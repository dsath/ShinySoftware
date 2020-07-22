#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

#include <stdint.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
  const int startRow = 150;
  const int startCol = 150;
  const int boxHeight = 30;

  const int fps = 20;
 


  //Mat original = imread("assets/pic.jpg", IMREAD_COLOR);
  //Mat modified = imread("assets/pic.jpg", IMREAD_COLOR);
  Mat frame;
  VideoCapture vid(2);


  while (vid.read(frame)) {

    int boxSize = boxHeight * boxHeight; 
    int red, green, blue;

    for (int r = startRow; r < startRow + boxHeight; ++r) {
      for(int c = startCol; c < startCol + boxHeight; ++c) {
        //store rgb values of current frame
        blue += frame.at<Vec3b>(r, c)[0]; 
        green += frame.at<Vec3b>(r, c)[1]; 
        red += frame.at<Vec3b>(r, c)[2];

        //make box black
        frame.at<Vec3b>(r, c)[0] = 0; 
        frame.at<Vec3b>(r, c)[1] = 0; 
        frame.at<Vec3b>(r, c)[2] = 0; 
      }
    }
      

    blue = blue/(boxSize);
    red = red/(boxSize);
    green = green/(boxSize);

    cout << "blue: " << blue << std::endl;
    cout << "green: " << green << std::endl;
    cout << "red: " << red << std::endl;
    imshow("frame", frame);
    if(cvWaitKey(1000 / fps) >= 0)
      break;
  }
  return 1;
}

