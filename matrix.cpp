#include "matrix.h"

Matrix::Matrix()
{
	row_w = 3;
	col_w = 3;
	numbers = (double*)malloc((3 * 3) * sizeof(double));
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			if(i == j)
			{
				numbers[(col_w*i) + j] = 1;
			}
			else
			{
				numbers[(col_w*i) + j] = 0;
			}
		}
	}
}

Matrix::Matrix(int row_width, int col_width)
{
	row_w = row_width;
	col_w = col_width;
	numbers = (double*)malloc((row_w*col_w) * sizeof(double));
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			numbers[(col_w*i) + j] = 0;
		}
	}
}

Matrix::Matrix(int n)
{
	row_w = n;
	col_w = n;
	numbers = (double*)malloc((n * n) * sizeof(double));
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			if(i == j)
			{
				numbers[(col_w*i) + j] = 1;
			}
			else
			{
				numbers[(col_w*i) + j] = 0;
			}
		}
	}
}

int Matrix::getColWidth() const
{
	return col_w;
}

int Matrix::getRowWidth() const
{
	return row_w;
}

double Matrix::getAt(int r, int c) const
{
	return numbers[(col_w*r) + c];
}

void Matrix::setAt(int r, int c, double x)
{
	numbers[(col_w*r) + c] = x;
}

Matrix& Matrix::operator=(const Matrix &rhs)
{
	row_w = rhs.getRowWidth();
	col_w = rhs.getColWidth();
	free(numbers);
	numbers = (double*)malloc((row_w * col_w) * sizeof(double));
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			numbers[(col_w*i) + j] = rhs.getAt(i, j);
		}
	}
	
	return *this;
}

void Matrix::transpose()
{
	int old_rw = row_w;
	int old_cw = col_w;
	Matrix temp(row_w);
	temp = *(this);
	row_w = old_cw;
	col_w = old_rw;
 	for(int i = 0; i < old_rw; i++)
	{
		for(int j = 0; j < old_cw; j++)
		{
			setAt(j, i, temp.getAt(i, j));
		}
	}
}

Matrix Matrix::getTranspose() const
{
	Matrix trans(col_w, row_w);
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			trans.setAt(j, i, numbers[(col_w * i) + j]);
		}
	}
	
	return trans;
}

void Matrix::invert()
{
	if((col_w == 2)&&(row_w == 2))
	{
		double a;
		double b;
		double c;
		double d;
		double o;
		
		a = getAt(0, 0);
		b = getAt(0, 1);
		c = getAt(1, 0);
		d = getAt(1, 1);
		
		o = 1.0/((a*d) - (b*c));
		
		setAt(0,0, (o*d));
		setAt(0,1, (o*b*-1.0));
		setAt(1,0, (o*c*-1.0));
		setAt(1,1, (o*a));
	}
	else if((col_w == 3)&&(row_w == 3))
	{
		//not needed yet
	}
	return;
}

Matrix Matrix::getInverse() const
{
	Matrix inv(2, 2);
	if((col_w == 2)&&(row_w == 2))
	{
		double a;
		double b;
		double c;
		double d;
		double o;
		
		a = getAt(0, 0);
		b = getAt(0, 1);
		c = getAt(1, 0);
		d = getAt(1, 1);
		
		o = 1.0/((a*d) - (b*c));
		
		inv.setAt(0,0, (o*d));
		inv.setAt(0,1, (o*b*-1.0));
		inv.setAt(1,0, (o*c*-1.0));
		inv.setAt(1,1, (o*a));
	}
	else if((col_w == 3)&&(row_w == 3))
	{
		//not needed yet
	}
	return inv;
}

const Vector Matrix::operator*(const Vector &other) const
{
	//only made to work by 2X2 matricies and vec2's
	Vector vec(0, 0, 0);
	vec.setX( (getAt(0,0) * other.getX()) + (getAt(0,1) * other.getY()));
	vec.setY( (getAt(1,0) * other.getX()) + (getAt(1,1) * other.getY()));
	return vec;
}

const Matrix Matrix::operator-(const Matrix &other) const
{
	Matrix result(row_w, col_w);
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			result.setAt(i, j, (getAt(i, j) - other.getAt(i, j)));
		}
	}
	
	return result;
}

void Matrix::rotate2d(double degrees)
{
	// assumes a 2x2 matrix
	double radians = degrees * (M_PI / 180.0);
	setAt(0,0, cos(radians));
	setAt(0,1, (sin(radians)*-1.0f));
	setAt(1,0, sin(radians));
	setAt(1,1, cos(radians));
}

const Matrix Matrix::operator*(const double &other) const
{
	Matrix result(row_w, col_w);
	for(int i = 0; i < row_w; i++)
	{
		for(int j = 0; j < col_w; j++)
		{
			result.setAt(i, j, (getAt(i, j) * other));
		}
	}
	
	return result;
}

void Matrix::print() const
{
	for(int i = 0; i < row_w; i++)
	{
		std::cout << "[";
		for(int j = 0; j < col_w; j++)
		{
			std::cout << " " << getAt(i,j) << " ";
		}
		std::cout << "]" << std::endl;
	}
}

const Matrix Matrix::operator*(const Matrix &other) const
{
	// this assumes row and colums make sense for multiplication
	Matrix result(row_w, other.getColWidth());
	double accumulator;
	for(int i = 0; i < row_w; i++)
	{
		for(int k = 0; k < other.getColWidth(); k++)
		{
			accumulator = 0.0;
			for(int j = 0; j< col_w; j++)
			{
				accumulator += getAt(i, j) * other.getAt(j, k);
			}
			result.setAt(i, k, accumulator);
		}
	}
	
	return result;
}

Matrix::~Matrix()
{
	free(numbers);
}
