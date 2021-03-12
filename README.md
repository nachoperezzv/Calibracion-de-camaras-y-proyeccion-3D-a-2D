# Calibracion-de-camaras-y-proyeccion-3D-a-2D

This project has been donde by Raúl Calatayud Ferre and Ignacio Perez Vilaplana, for a robotics project at the Alicante University. 
In this repositorie are files that allow to take pics from a PC webcam and save them automatically in a file called 'myFrames'. The programm has been done
taking into account that the pics must contain an asimetric chessboard. Thanks to this pics another programcalled 'getCalibrated_vX.cpp' will calibrate the camera
parameters, not only intrinsec parameters but also extrinsec ones. This action will be possible thanks to OpenCV. For the main version it has been necessary
at least version 3.0.0 or higher, but some versions of the calibrated system requires OpenCV version 4.0.0. 
Once the camera is calibrated the distorsion coefficients will be saved in a YAML file. Program 'getProjection_vX.cpp' will use this file to create and undistort
image and will use the projection specified (from the file projections) to make the 3D representation. 
