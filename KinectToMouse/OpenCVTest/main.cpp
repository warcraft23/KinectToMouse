#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	const char* imagename = "lena.jpg";

	Mat img = imread(imagename);

	if(img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", imagename);
		return -1;
	}

	imshow("image", img);
	waitKey();
	return 0;
}
