#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string> 
#include <sstream> 
#include <fstream>

using namespace cv;
using namespace std;

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
	
	vector<Point2f> corners;
	vector<Point2f> puntosProyectados;
	
	VideoCapture capture = VideoCapture(0);
	namedWindow("imagen",WINDOW_AUTOSIZE);
	Mat image, image2;
	Mat gray;
	
	int num = 1;
	
	while(1){
		
		string frame = "bailarin/frame"; 
		
		frame = frame + to_string(num) + ".txt"; //Creation of the name of the .txt file that contains the point cloud
		
		ifstream f(frame);// Open the file
	
		float x,y,z;
		vector<Point3f> puntos;
		if(f.is_open()){ // Read the points inside
			while(f >> y && f >> x && f >> z){
			Point3f p = {x,y,z};
			puntos.push_back(p);
			}
		}
		
		if(num == 8){
			num = 1;
		}
		else{
			num++;
		}
		
		capture >> image;
		undistort(image, image2, cameraMatrix, distCoeffs);//corrijo distorsion radial
		cvtColor(image2,gray, CV_RGB2GRAY);

		bool found = findChessboardCorners(gray, patternSize,corners);
		if(found){
			TermCriteria criteria(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
			cornerSubPix(gray, corners, Size(11,11), Size(-1,-1),criteria);
			
			solvePnP(corners3D, corners, cameraMatrix, distCoeffs, rvecs, tvecs);

			projectPoints(puntos, rvecs, tvecs, cameraMatrix, distCoeffs, puntosProyectados);

			for(int i = 0 ; i < puntosProyectados.size() - 1 ; i++){
				if(i != 64){ // We dont draw a line with this point beacuse it is a jump to draw the head
					if(i >= 17 && i < 59){
						line(image2,puntosProyectados[i], puntosProyectados[i+1],Scalar(0,0,0), 3); // Drawing line in black for the stick
					}
					else{
						line(image2,puntosProyectados[i], puntosProyectados[i+1],Scalar(0,255,0), 3); // Drawing line in green for the body
					}
				}		
			}
		}
		
		imshow("imagen", image2);
		waitKey(10);
	}
	
	return 0;
}
