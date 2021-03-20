/*

	g++ getImages_v1.cpp -o getImages_v1 `pkg-config opencv --cflags --libs`
	./getImages_v1

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

//Namespaces used:
using namespace std;
using namespace cv;

//Global Variables  
string 			imgName;
int 			numImg 	= 0;

//Constant
const int 		numOfImg = 32;

//main function:
int main(){

	Mat frame;				//Mat object to save frames
	VideoCapture camera(0);	//Function from /opencv.hpp

							//Checking if the webcam is on
	if(!camera.isOpened())	//Returns true if video capturing has been initialized already
	{
		cerr<< "ERROR: couldn't open webcam" << endl;
	 	return -1;
	}

	namedWindow("Capture", WINDOW_AUTOSIZE); //Creates a window for display		
	
	while(true)				//Infinite loop
	{
		camera >> frame;	//Saving frame from the webcam. 
							//Also could be --> camera.read(frame)
		
		if(frame.empty())	//Checking empty frame
			break;
		
		imshow("Capture", frame);	//if it's not empty, shows it. 
		
		if(waitKey(10)==32)		//Waits (at least) 10ms until it passes to next frame
		{
			if(numImg >= numOfImg)	//Checking number of pics taken
				break;				//if it's over max number of pics == break
			
			imgName = "myFrames/cap" + to_string(numImg) + ".jpg";	numImg++;
							//Name and number with whick the frame is saved

			if(imwrite(imgName, frame))
				cout << imgName << " saved" << endl;
						//Frame successfully saved message
			else
				cout << "ERROR: couldn't save image" << endl;
						//failure saving frame message
		}
	}

	destroyWindow("VideoCapture");	
							//Closes the window
							//The camera closes automatically 
	
	return 0;
}
