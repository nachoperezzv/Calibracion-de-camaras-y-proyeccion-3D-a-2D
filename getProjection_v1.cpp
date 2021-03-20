/*

	g++ getProjection_v1.cpp -o getProjection_v1 `pkg-config opencv --cflags --libs`
	./getProjection_v1

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
#include <fstream>
#include <string>

//Namespaces used:
using namespace std;
using namespace cv;

//Functions used: 
static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners);


int main(){
		//It is declared the variables that will be used. Firstly it is indicated the size of the
		//chessboard. Secondly, a variable type Mat (matrix that can save RGB/grayscale/.. image), 
		//that will get the intrinsic parameters of the camera. And last another Mat variable 
		//where the extrinsic parameters will be saved. 
	Size patternSize(9,6);    
    Mat cameraMatrix;	
    Mat distCoeffs;

    	//The previous variables are going to get the data from a YML file. These kind of
    	//files are declared as a 'FileStorage' object. This is a class that encapsulates
    	//all the information necessary for writing or reading data from these files. 
    	//As it is onlt necesarry to read the file, the only flag activated is 'READ'.
    	//The parameters have been previously calculated in another program and save in
    	//a .YML file called 'DataCam'.
    FileStorage fs("DataCam.yml", FileStorage::READ);

    	//In order to read the values easily it is possible to put them directly into a 
    	//container vector. In .YML files, data is stored hierarchically. That means that
    	//is possible to call the parameters that are needed by its name. 
    	//So since the intrinsic parameters have been saved with the name Camera_Matrix, 
    	//it is possible to request the information by typing the name.
	fs["Camera_Matrix"] >> cameraMatrix;
	fs["Distortion_Coefficients"] >> distCoeffs;
	
	Mat rvecs;	//Matrix of rotation vectors. Indicates the rotation of the camera
				//based on the chessboard.

	Mat tvecs;	//Matrix of traslation vectors. Indicates the traslation of the camera
				//based on the chessboard.
	
	vector<Point3f> corners3D;
	
	calcChessboardCorners(Size(9,6),24,corners3D);
		//As has been done in the calibration program it is created a vector chessboard
	
	ifstream f("projections/monigote.txt");	
		//Access to the folder where the figure to be projected is located
	
	float x,y,z;
	vector<Point3f> puntos;
	if(f.is_open()){
		//The positions of the .txt file that indicate the position of the figures are read
		//and saved in a type 'Point3f' vector (indicates position in a 3D reference system)

		while(f >> y && f >> x && f >> z){
		
		Point3f p = {x,y,z};
		puntos.push_back(p);
		}
	}
	
	
	vector<Point2f> corners;
	vector<Point2f> puntosProyectados;
	
	namedWindow("imagen",WINDOW_AUTOSIZE);
		//It is created the window where the video is going to be show
	
	VideoCapture capture = VideoCapture(0);
		//Acces to webcam

	Mat image, image2;
	Mat gray;
	
	
	while(1){
		
		capture >> image;
		undistort(image, image2, cameraMatrix, distCoeffs);//corrijo distorsion radial
		cvtColor(image2,gray, CV_RGB2GRAY);
			//Correction of the webcam thanks to the parameters calculated previously

		bool found = findChessboardCorners(gray, patternSize,corners);
			//Finding the corners of the frame that is being capture by the camera in 
			//real time to position the figure correctly
		if(found){
			TermCriteria criteria(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
				//The criteria of the function are established. It can be done both 
				//outside and inside the 'cornerSubPix()' function.

			cornerSubPix(gray, corners, Size(11,11), Size(-1,-1),criteria);
				//It has already been defined in 'getCalibrated_v1()'. Nevertheless
				//This function is used to improve the precision of the corner detector. 
				//It further refines detected corners with sub-pixel precision. In order to
				//use this one, it is need to have calculated the corners with Harris or some
				//other algorithm (like findChessboardCorners). 
			
			solvePnP(corners3D, corners, cameraMatrix, distCoeffs, rvecs, tvecs);
				//Finds an object pose from 3D-2D point correspondences. This function returns 
				//the rotation and the translation vectors that transform a 3D point expressed 
				//in the object coordinate frame to the camera coordinate frame. 
				//
				//The function estimates the object pose given a set of object points, their 
				//corresponding image projections, as well as the camera intrinsic matrix and the 
				//distortion coefficients, see the figure below (more precisely, the X-axis 
				//of the camera frame is pointing to the right, the Y-axis downward and the 
				//Z-axis forward).

			projectPoints(puntos, rvecs, tvecs, cameraMatrix, distCoeffs, puntosProyectados);
				//Projects 3D points to an image plane. The function computes the 2D 
				//projections of 3D points to the image plane, given intrinsic and extrinsic 
				//camera parameters

			for(int i = 0 ; i < (int)puntosProyectados.size() - 1 ; i++){
				if(i != 10){
					line(image2,puntosProyectados[i], puntosProyectados[i+1],Scalar(0,255,0), 3);
				}	

				//Once the points of the figure are drawn in the 2D projection, they are
				//lined by the function 'line()'. 
			}
		}
		
		imshow("imagen", image2);
		waitKey(1);
	}
	
	return 0;
}

static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners){
	corners.clear();
    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back(Point3f(float(j*squareSize),
                                      float(i*squareSize), 0)); 
}