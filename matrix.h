#ifndef MATRIX_H
	#define MATRIX_H
	#include <stdlib.h>
	#include <cmath>
	#include <iostream>
	#include "vector.h"
	
	class Matrix
	{
		int row_w;
		int col_w;
		double* numbers;
		
		public:
			Matrix();
			Matrix(int row_width, int col_width);
			Matrix(int n);
			int getColWidth() const;
			int getRowWidth() const;
			double getAt(int r, int w) const;
			void setAt(int r, int w, double x);
			void transpose(); // needs test
			void invert();
			void print() const;
			Matrix getTranspose() const;
			Matrix getInverse() const;
			Matrix& operator=(const Matrix &rhs);
			void rotate2d(double degrees);
			~Matrix();
			const Vector operator*(const Vector &other) const;
			const Matrix operator-(const Matrix &other) const; 
			const Matrix operator*(const double &other) const;
			const Matrix operator*(const Matrix &other) const; //needs test
	};
#endif