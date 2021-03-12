/*
	g++ getCalibrated_v1.cpp -o getCalibrated_v1 `pkg-config opencv --cflags --libs`
	./getCalibrated_v1

	Raúl Calatayud Ferre
	Ignacio Pérez Vilaplana

*/

//OpenCV Libraries:
#include <opencv2/opencv.hpp>

//C Libraries
#include <stdio.h>
#include <stdlib.h> 

//C++ Libraries
#include <iostream>
#include <string> 

//Namespaces used:
using namespace std;
using namespace cv;

//Functions used:
static 	void 	getCorners	(Size , float , vector<Point3f>&);
static 	void 	saveParams	(const string&, const Mat&, const Mat&, const vector<Mat>&, 
							 const vector<Mat>&, const double&);

//Main Function:
int main(){

	Size patternsize = Size(9,6);	//Number of squares of the chessboard in terms
											//height and weidth

	vector<Point3f> corners3D;		//calcChessboardCorners saves the simulation of
											//the board points here

	vector<Point2f> corners2D;		//findChessboardCorners saves the board points,
											//found on the image, here.

	vector<vector<Point2f>> coord2D;
											//Location of the corners in the image

	vector<vector<Point3f>> coord3D;
											//Real location of the 3D points in the image


	getCorners(patternsize,25,corners3D);
					//Parameters send to this function are: number of squares vertically
					//and horizontally (patternsize), size of each box(square).
	
	Mat 	img;
	Mat 	imgGray;
	bool 	found;
	string 	imgs;

	for(int i=0; i<14; i++)
				//The loop is due to the fact that each of the images has to be calibrated.
				//It will have as many iterations as there are images.
	{
			imgs = "myFrames/cap" + to_string(i) + ".jpg";
			img = imread(imgs);
			if(!img.data or img.empty())
				//Checking if the image exists
				continue;
			

			cvtColor(img, imgGray, COLOR_BGR2GRAY);
				//The change to grey scale it's due to the function findChessboardCorners 
				//needs the parameter to be in 8-bit black-&-white format


			found = findChessboardCorners(imgGray,patternsize,corners2D,
											CALIB_CB_ADAPTIVE_THRESH + 
			                                CALIB_CB_NORMALIZE_IMAGE + 
											CALIB_CB_FAST_CHECK);
				//This function from OpenCV find the position of internal corners of the 
				//chessboard. It finds in the grey scale image 'imgGray' the corners and save
				//this positions in the vector 'corners2D'. The order the corners are saved
				//is row by row, starting with the one closest to the TOP_LEFT corner of 
				//the image

				//The flags have a function.
				//	->CALIB_CB_ADAPTIVE_THRESH: Use adaptive thresholding to convert the image
				//								to black and white.
				//	->CALIB_CB_NORMALIZA_IMAGE:	Normalize the image gamma with equalizeHist()
				//	->CALIB_CB_FAST_CHECK:		Run a fast check on the image that looks for
				//								chessboard corners, and shortcut the call if
				//								none is found. 

				//The function returns a non-zero value if all of the corners are found and 
				//they are placed in a certain order (row by row, left to right in every row).
				//That means that if the image has more or less corners than what is indicated,
				//the pic/frame won't be used to do the calibration.

			if(found) //If all the corners arefound
			{
				
			cornerSubPix(imgGray, corners2D, Size(11, 11), Size(-1, -1),
						 TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1 ));
				//This function is used to improve the precision of the corner detector. 
				//It further refines detected corners with sub-pixel precision. In order to
				//use this one, it is need to have calculated the corners with Harris or some
				//other algorithm (like findChessboardCorners). 

				//The parameters are the input image (imgGray) in grey-scale; the initial
				//coordinates of the input corners (corners2D) (This variable will be modified 
				//with the new, more precise coordinates); half of the size length of the 
				//search window; half of the size of the dead region in the middle of the
				//searchxone over which the summation in the formula below is not done;
				//and last parameter, but not least, the criteria parameter. Criteria for
				//termination of the iterative process of corner refinement. In this case
				//it is called to 'TermCriteria' class. The class definiting termination
				//criteria for iterative algorithms. The constructor calls the flags
				//COUNT (the maximum number of iterations or elements to compute -> 30) and 
				//EPS (the desired accuracy or change in parameters at which the iterative
				//algorithm stops -> 0.1). 

				//This let us now that the cornerSubPix is going to run a loop until the 
				//accuracy precision is achieved. 

			drawChessboardCorners(img,patternsize,Mat(corners2D),found);
				//This renders the detected chessboard corners. In this case, the last 
				//parameter of the function won't be needed for us, because it has been
				//already checked that the pattern has been found. Anyways, it is putted 
				//because of the constructor. 


			coord2D.push_back(corners2D);
			coord3D.push_back(corners3D);
				//It is saved in a vector the square coordinates of all the images. Not 
				//only 2D coordinates, but also 3D coordinates. 
		} 

		namedWindow("image",WINDOW_AUTOSIZE);
		imshow("image", img);
		waitKey(1500);
	}

	cvDestroyWindow("image");

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
	vector<Mat> rvecs;
	vector<Mat> tvecs;
   
	double  rms = 	calibrateCamera(coord3D, coord2D, img.size(), cameraMatrix,
                  	distCoeffs, rvecs, tvecs, 
				  		CALIB_FIX_PRINCIPAL_POINT + 
				  		CALIB_FIX_ASPECT_RATIO + 
				  		CALIB_ZERO_TANGENT_DIST,
					TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 2.22e-16));
		//It is an OpenCV function that finds the camera intrinsic and extrinsic parameters
		//from several views of a calibration. 

		//The most important thing of this function is that it provides both the values of the
		//distorsion coefficients, which will save in 'distCoeffs', and it's a vector,
		//of distorsion coefficients; and the 3x3 matrix of floating-point with the 
		//coefficients fx, fy (focal length width and height), cx and cy 
		//(image center coordinates). 		[fx 0  cx]
		//								A =	[0  fy cy]
		//									[0  0   1]
		//
		//Also it will be provided  an output vector of rotation vectors estimated for each
		//pattern view. That is, each k-th rotation vector together with the corresponding
		//k-th translation vector brings the calibration pattern from the model coordinate 
		//space to the world coordinate space, that is a real position of the calibration
		//pattern in the k-th pattern view. This will be saved in 'rvecs' vector. Besides
		//there's another vector of Mat(s) called 'tvecs' with the output vector of 
		//translation vectors estimated for each pattern view. 

		//Basically what this function does is to give us two matrix of transformation
		//(translation and rotation) for each pattern gived, that let us know the position 
		//that the camera is occupying in a reference system where the board is (0,0,0). 
		//And so the focal length thanks to matrix A. 

		//It must be said that the algorithm is based on the Zhang method, which is a very
		//simple and efficient algorithm (at least compared to others). The algorithm 
		//performs the following steps:	
		//	
		// 	1- Compute the initial intrinsic parameters or read them from the input 
		//     parameters. The distorsion coefficients are all set to zeros initially
		//	   unless some of the flags are specified
		//	2- Estimate the initial camera pose as if the intrinsic parameters have been 
		//	   already known. This is done using 'solvePnP()'. 
		//  3- Run the gloabl Levenberg-Marquadt optimization algorithm to minimize the 
		//     reprojection error, that is, the total sum os squared distances between the
		//     observed feature points 'imagePoints' and the projected (using the current
		//     estimates for camera parameters and the poses) object points. 

		//The function returns the final re-projection error

	//It can be showed by the terminal the
	cout << "RMS: " << rms << endl;
	cout << "Camera_Matrix: " << endl << cameraMatrix << endl;
	cout << "Distortion _coefficients: " << endl << distCoeffs << endl;

	saveParams("DataCam.yml",cameraMatrix,distCoeffs,rvecs,tvecs,rms);

		//Now the params has been saved and also the matrix and distorsion coefficients had
		//been calculated, it is possible to show in the screen the correct calibration
		//of the camera. 

		//So here it is made a comparission with the distorted image and the undistorted one
		//on real time. 


	/*This part can be showed to check if everything is allright, but once it's checked
	  it is not necessary to print every time the programm is run*/

	Mat imageUndistorted,image;
	VideoCapture capture = VideoCapture(0);
	namedWindow("imgOriginal",WINDOW_AUTOSIZE);
	namedWindow("imgCalibrada",WINDOW_AUTOSIZE);
	while(1){

	    capture >> image;
	    undistort(image, imageUndistorted, cameraMatrix, distCoeffs);
	    	//This one transforms an image to compensate for lens distorsion. The function
	    	//transforms an image to compensate radial and tangential lens distorsion. 

	    	//In conclusion, all the params that had been calculated since this moment are
	    	//used in order to correct the distorsion. As it was said before the A matrix
	    	//and the distorsion coefficients are needed to correct this problem. That's 
	    	//what this function really does; to correct the image. 

	    imshow("imgOriginal", image);
	    imshow("imgCalibrada", imageUndistorted);
	    waitKey(1);
	}
	/**/

	waitKey(0);
	getchar();

	return 0;
}




static void getCorners(Size boardSize, float squareSize, vector<Point3f>& corners)
{
	corners.clear();		//Removes all elements from the vector(which are destroyed),
							//leaving the container with a size of 0. This just to make 
							//sure it's empty. 

							//boardSize is a variable that gives us the size of each chess
							//square. As is of type Size we had height and width. This one
							//contains the number os squares. 
    for( int i = 0; i < boardSize.height; i++ )			
        for( int j = 0; j < boardSize.width; j++ )		
            corners.push_back(Point3f(float(j*squareSize),float(i*squareSize), 0)); 
        					//This loop adds new element to vector type 'Point3f' in each
        					//iteration. A representation of the points of the plane that 
        					//correspond to the corners of the board is made and saved. The 
        					//'z' position is always 0. Is an attermpt to represent the board
        					//in a vector. 

}

static void saveParams(const string& f_name, const Mat& camera_mat, const Mat& dist_coeficients, 
					  const vector<Mat>& rvecs, const vector<Mat>& tvecs, const double& RMS)
{
	//In this function the goal is to save on a specific type of file (.yml) all the data
	//that has been calculated until now

	FileStorage fs( f_name, FileStorage::WRITE );
			//FileStorage is a class from opencv. XML/YAML/JSON file storage class that
			//encapsulates all the information necessary for writting or reading data
			//to/from a file. This is a a constructor of a fileStorage object named 'fs'.
			//It is provided with the name of file where is it going to be saved the data
			//(name must contain the extension of the type of file -- XML, YML, ...) and
			//the second it's a flag that indicates that the file is opened for writting

	fs << "Calibrate_Accuracy" << RMS;
	fs << "Camera_Matrix" << camera_mat;
	fs << "Distortion_Coefficients" << dist_coeficients;
	fs << "Rotation_Vector" << rvecs;
	fs << "Translation_vector" << tvecs;
			//The operators are overloaded so this way all the information is printed in
			//the document object 'fs'

	if( !rvecs.empty() && !tvecs.empty() ){

		CV_Assert(rvecs[0].type() == tvecs[0].type());
		Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
		for( int i = 0; i < (int)rvecs.size(); i++ ){
			Mat r = bigmat(Range(i, i+1), Range(0,3));
			Mat t = bigmat(Range(i, i+1), Range(3,6));

			CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
			CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
  
			r = rvecs[i].t();
			t = tvecs[i].t();
   		}
   		cvWriteComment( *fs, "Rotation vector + Translation vector", 0 );
    	fs << "extrinsic_parameters" << bigmat;
	}
	fs.release();
	
}