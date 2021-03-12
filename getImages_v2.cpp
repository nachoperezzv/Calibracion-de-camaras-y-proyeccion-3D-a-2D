/*

	g++ getImages_v2.cpp -o getImages_v2 `pkg-config opencv --cflags --libs`
	./getImages_v2

	Raúl Calatayud Ferre
	Ignacio Pérez Vilaplana

*/

//OpenCV library:
#include <opencv2/opencv.hpp>

//C Libraries:
#include <stdio.h>
#include <stdlib.h>

//C++ Libraries
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

//Namespaces used:
using namespace std;
using namespace cv;

//Global Variables  
string 			imgName;
int 			numImg 	= 0;

//Constant
const int 		numOfImg= 14;

//main function:
int main(){

	Mat frame;				//Mat object to save frames
	VideoCapture camera(0);	//Function from /opencv.hpp

	auto start 	= clock();	//Initialazing variables with cpu clock
	auto end 	= clock();

							//Checking if the webcam is on
	if(!camera.isOpened())	//Returns true if video capturing has been initialized already
	{
		cerr<< "ERROR: couldn't open webcam" << endl;
	 	return -1;
	}

	namedWindow("Capture", WINDOW_AUTOSIZE); //Creates a window for display

	start = clock();	end = clock();				//Gives a time-lapse of 2 secs
	while(1000*(end-start)/(CLOCKS_PER_SEC*30) > 2)
		end = clock();

	start = clock();	end = clock();				
	
	while(true)				//Infinite loop
	{
		camera >> frame;	//Saving frame from the webcam. 
							//Also could be --> camera.read(frame)
		end = clock();

		if(frame.empty())	//Checking empty frame
			break;
		
		imshow("Capture", frame);	//if it's not empty, shows it. 
		
		if(waitKey(10))		//Waits (at least) 10ms until it passes to next frame
		{
			if(numImg >= numOfImg)	//Checking number of pics taken
				break;				//if it's over max number of pics == break

			if(1000*(end-start)/(CLOCKS_PER_SEC*30) > 4)
							//Waits 4secs between pics. if time is below 4secs goes back to 
							//the beginning of the loop
			{
				imgName = "/myFrames/cap" + to_string(numImg) + ".jpg";	numImg++;
							//Name and number with whick the frame is saved

				if(imwrite(imgName, frame))
					cout << imgName << " saved" << endl;
							//Frame successfully saved message
				else
					cout << "ERROR: couldn't save image" << endl;
							//failure saving frame message

				start = end;

			}
		}
	}

	destroyWindow("VideoCapture");	
							//Closes the window
							//The camera closes automatically 
	
	return 0;
}