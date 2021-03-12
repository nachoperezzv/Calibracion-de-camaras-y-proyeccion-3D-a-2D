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

static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners){
	corners.clear();
    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back(Point3f(float(j*squareSize),
                                      float(i*squareSize), 0)); 
}

int main(){
	Size patternSize(9,6);
    
    Mat cameraMatrix;
	
    Mat distCoeffs;

    FileStorage fs("DataCam.yml", FileStorage::READ);
    
	fs["Camera_Matrix"] >> cameraMatrix;
	fs["Distortion_Coefficients"] >> distCoeffs;
	
	Mat rvecs;
	Mat tvecs;
	
	vector<Point3f> corners3D;
	
	calcChessboardCorners(Size(9,6),24,corners3D);
	
	ifstream f("projections/monigote.txt");	
	
	float x,y,z;
	vector<Point3f> puntos;
	if(f.is_open()){
		while(f >> y && f >> x && f >> z){
		
		Point3f p = {x,y,z};
		puntos.push_back(p);
		}
	}
	
	
	vector<Point2f> corners;
	vector<Point2f> puntosProyectados;
	
	VideoCapture capture = VideoCapture(0);
	namedWindow("imagen",WINDOW_AUTOSIZE);
	Mat image, image2;
	Mat gray;
	
	
	while(1){
		
		capture >> image;
		undistort(image, image2, cameraMatrix, distCoeffs);//corrijo distorsion radial
		cvtColor(image2,gray, CV_RGB2GRAY);

		bool found = findChessboardCorners(gray, patternSize,corners);
		if(found){
			TermCriteria criteria(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
			cornerSubPix(gray, corners, Size(11,11), Size(-1,-1),criteria);
			
			solvePnP(corners3D, corners, cameraMatrix, distCoeffs, rvecs, tvecs);

			projectPoints(puntos, rvecs, tvecs, cameraMatrix, distCoeffs, puntosProyectados);

			for(int i = 0 ; i < (int)puntosProyectados.size() - 1 ; i++){
				if(i != 10){
					line(image2,puntosProyectados[i], puntosProyectados[i+1],Scalar(0,255,0), 3);
				}	
			}
		}
		
		imshow("imagen", image2);
		waitKey(1);
	}
	
	return 0;
}
