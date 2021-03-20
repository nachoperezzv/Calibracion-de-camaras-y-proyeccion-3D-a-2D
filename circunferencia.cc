#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string> 
#include <sstream> 
#include <fstream>
#include <math.h>

using namespace std;

#define pi 3.14159265359

int main(){
	int puntos = 40;
	int radio = 5;
	int x = 45 , y = 30;
	
	float vecx[puntos];
	float vecy[puntos];
	
	float puntox = x + radio;
	float puntoy = y;
	
	float ang = (2*pi)/puntos;
	
	for(int i = 0; i < puntos; i ++){
		puntox = x + radio*cos(i * ang);
		puntoy = y + radio*sin(i * ang);
		vecx[i] = puntox;
		vecy[i] = puntoy; 
		cout << "60 " << puntox << " " << -1*puntoy << endl;
	}
	
	cout << endl << "PATRAS" << endl;
	
	for(int i = puntos-1; i >=0 ; i--){
		cout << "60 " << vecx[i] << " " << -1*vecy[i] << endl;	
	}
}
