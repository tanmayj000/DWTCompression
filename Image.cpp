//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>
#include <algorithm>
#include <math.h> 

#include <cstring>

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	filter_level = -2;
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

void MyImage::compressRows(double* transformed_array, int length) {

	double* compressedRow = new double[length];
	double* currRow = new double[length];

	for (int channel = 0; channel < 3; channel++) {

		for (int iRow = 0; iRow < length; iRow++) {

			for (int iCol = 0; iCol < length; iCol++) {
				currRow[iCol] = transformed_array[3 * (iRow * Width + iCol) + channel];
			}

			for (int iCol = 0; iCol < length; iCol += 2) {
				compressedRow[iCol / 2] = (currRow[iCol] + currRow[iCol + 1]) / 2;
				compressedRow[iCol / 2 + length / 2] = (currRow[iCol] - currRow[iCol + 1]) / 2;
			}

			for (int iCol = 0; iCol < length; iCol++) {
				transformed_array[3 * (iRow * Width + iCol) + channel] = compressedRow[iCol];
			}

		}
	}

	delete[] compressedRow;
	delete[] currRow;

}

void MyImage::compressColumns(double* transformed_array, int length) {

	double* compressedCol = new double[length];
	double* currCol = new double[length];

	for (int channel = 0; channel < 3; channel++) {

		for (int iCol = 0; iCol < length; iCol++) {

			for (int iRow = 0; iRow < length; iRow++) {
				currCol[iRow] = transformed_array[3 * (iRow * Width + iCol) + channel];
			}

			for (int iRow = 0; iRow < length; iRow += 2) {
				compressedCol[iRow / 2] = (currCol[iRow] + currCol[iRow + 1]) / 2;
				compressedCol[iRow / 2 + length / 2] = (currCol[iRow] - currCol[iRow + 1]) / 2;
			}

			for (int iRow = 0; iRow < length; iRow++) {
				transformed_array[3 * (iRow * Width + iCol) + channel] = compressedCol[iRow];
			}

		}
	}

	delete[] compressedCol;
	delete[] currCol;

}

void MyImage::decompressColumns(double* transformed_array, int length) {
	double* decompressedCol = new double[length];
	double* currCol = new double[length];

	for (int channel = 0; channel < 3; channel++) {

		for (int iCol = 0; iCol < length; iCol++) {

			for (int iRow = 0; iRow < length; iRow++) {
				currCol[iRow] = transformed_array[3 * (iRow * Width + iCol) + channel];
			}

			for (int iRow = 0; iRow < length / 2; iRow++) {
				decompressedCol[2 * iRow] = currCol[iRow] + currCol[iRow + length / 2];
				decompressedCol[2 * iRow + 1] = currCol[iRow] - currCol[iRow + length / 2];
			}

			for (int iRow = 0; iRow < length; iRow++) {
				transformed_array[3 * (iRow * Width + iCol) + channel] = decompressedCol[iRow];
			}

		}
	}

	delete[] decompressedCol;
	delete[] currCol;

}

void MyImage::decompressRows(double* transformed_array, int length) {
	double* decompressedRow = new double[length];
	double* currRow = new double[length];

	for (int channel = 0; channel < 3; channel++) {

		for (int iRow = 0; iRow < length; iRow++) {

			for (int iCol = 0; iCol < length; iCol++) {
				currRow[iCol] = transformed_array[3 * (iRow * Width + iCol) + channel];
			}

			for (int iCol = 0; iCol < length / 2; iCol++) {
				decompressedRow[2 * iCol] = currRow[iCol] + currRow[iCol + length / 2];
				decompressedRow[2 * iCol + 1] = currRow[iCol] - currRow[iCol + length / 2];
			}

			for (int iCol = 0; iCol < length; iCol++) {
				transformed_array[3 * (iRow * Width + iCol) + channel] = decompressedRow[iCol];
			}
		}
	}

	delete[] decompressedRow;
	delete[] currRow;

}

void MyImage::setDWTCoeffs(double* transformed_array, int filter_level) {
	long long length = pow(2, filter_level);
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			if (i >= length || j >= length) {
				transformed_array[3 * (i * Width + j)] = 0.0;
				transformed_array[3 * (i * Width + j) + 1] = 0.0;
				transformed_array[3 * (i * Width + j) + 2] = 0.0;
			}
		}
	}
}


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(int curr_level)
{
	int startWidth = Width;

	// Get original data in doubles
	double* transformed_array = new double[Height * Width * 3];

	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			transformed_array[3 * (i * Width + j)] = (double)(unsigned char)Data[3 * (i * Width + j)];
			transformed_array[3 * (i * Width + j) + 1] = (double)(unsigned char)Data[3 * (i * Width + j) + 1];
			transformed_array[3 * (i * Width + j) + 2] = (double)(unsigned char)Data[3 * (i * Width + j) + 2];
		}
	}

	// Compress rows and columns
	for (int i = 512; i != 1; i /= 2) {
		compressRows(transformed_array, i);
		compressColumns(transformed_array, i);
	}

	setDWTCoeffs(transformed_array, curr_level);


	for (int i = 2; i != 1024; i *= 2) {
		decompressColumns(transformed_array, i);
		decompressRows(transformed_array, i);
	}
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			Data[3 * (i * Width + j)] = (char)(int)transformed_array[3 * (i * Width + j)];
			Data[3 * (i * Width + j) + 1] = (char)(int)transformed_array[3 * (i * Width + j) + 1];
			Data[3 * (i * Width + j) + 2] = (char)(int)transformed_array[3 * (i * Width + j) + 2];
		}
	}
	delete[] transformed_array;


	return false;
}