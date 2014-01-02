#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace cv;
using namespace std;
#define TEMPERATURE_REACHED 2000

int main(void)
{
	int i, j;
	ofstream myfile;

	long int red_quant = 0;
	Mat cameraFrame, matrix2;

	// Video capture part
	VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
	 
	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
		exit(1);
	}

	
	for(i = 0; i < 30; i++)
	{
		stream1.read(cameraFrame);
	}

	matrix2 = cameraFrame(Range(127,169), Range(427, 460));
	int nrows, ncols;
	nrows = matrix2.rows;
	ncols = matrix2.cols;
	// segmentation for red, and count
	for(i = 0; i < ncols + 1; i++)
	{
		for(j = 0; j < ncols + 1; j++)
		{
			if((matrix2.at<cv::Vec3b>(i, j)[0] < 200) && (matrix2.at<cv::Vec3b>(i, j)[1] < 200)) //if colour is not close to white
			{
				if (matrix2.at<cv::Vec3b>(i, j)[2] > 200)
					red_quant += 255;
			}
			
		}
	}
	cout << "red quantity is " << red_quant << "\n";

	myfile.open ("/dev/shm/water_temperature");
	if(red_quant >= TEMPERATURE_REACHED)
 		myfile << "1";
	else
		myfile << "0";
	myfile.close();

	//namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
	//imshow( "Display window", matrix2);                   // Show our image inside it.
	//waitKey(0);  
}
