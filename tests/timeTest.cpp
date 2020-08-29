#include <iostream>
#include <fstream>
#include <assert.h>
#include <ctime>

using namespace std;

int main () {
  time_t n = time(0);
  tm *now = localtime(&n);
  cout << now->tm_year << " -- " << now->tm_hour << " -- " <<  now->tm_min << endl;
  return EXIT_SUCCESS;
}


