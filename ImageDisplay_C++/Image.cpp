
//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <math.h>
#define PI 3.1415927
// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}

void getOld(int& old_x, int& old_y, int new_x, int new_y, double S, double A, int tx, int ty) {
	old_x = (cos(A * PI / 180)* (new_x - tx) + sin(A * PI / 180) * (new_y - ty))/S + tx;
	old_y = (-sin(A * PI / 180) * (new_x - tx) + cos(A * PI / 180) * (new_y - ty)) / S + ty;
}


void scale_rot(MyImage *that, char *Data, double S, double A, int W, int H) {
	int shift_x = W / 2, shift_y = H / 2, old_x, old_y;
	char* Tempo = new char[W * H * 3]();
	for (int new_x = 0; new_x < W; new_x++) {
		for(int new_y = 0; new_y < H; new_y++) {
			getOld(old_x, old_y, new_x, new_y, S, A, shift_x, shift_y);
			if (old_x < 0 || old_y < 0 || old_x >= W || old_y >= H) {
				Tempo[3 * (new_x * W + new_y)] = '0';
				Tempo[3 * (new_x * W + new_y)+1] = '0';
				Tempo[3 * (new_x * W + new_y)+2] = '0';
				continue;
			}
			Tempo[3 * (new_x * W + new_y)] = Data[3 * (old_x * W + old_y)];
			Tempo[3 * (new_x * W + new_y) + 1] = Data[3 * (old_x * W + old_y) + 1];
			Tempo[3 * (new_x * W + new_y) + 2]  = Data[3 * (old_x * W + old_y) + 2];
		}
	}
	that->setImageData(Tempo);
}

unsigned int ui(char x) {
	unsigned char y = x;
	return (unsigned int)(y);
}

void MyImage::anti_alias(char* Data) {
	int W = Width, H = Height;
	char* Tempo = new char[W * H * 3]();
	int neighbourhood[9][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,0},{0,1},{1,-1},{1,0},{1,1} };
	for (int j = 0; j < 3; j++) {
		for (int x = 0; x < H; x++) {
			for (int y = 0; y < W; y++) {
				if (x == 0 || y == 0 || x == H - 1 || y == W - 1) {
					Tempo[3 * (x * W + y) + j] = Data[3 * (x * W + y) + j];
					continue;
				}
				double val = 0;
				for (int i = 0; i < 9; i++)  
					val += ui(Data[3 * ((x + neighbourhood[i][0]) * W + y + neighbourhood[i][1]) + j]);
				val /= 9;
				Tempo[3 * (x * W + y) + j] = int(val);
			}
		}
	}
	setImageData(Tempo);
}


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(double S, double R, int A)
{
	scale_rot(this, Data, S, R, Width, Height);
	return true;
}