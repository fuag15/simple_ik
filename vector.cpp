#include "vector.h"

Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double x_in, double y_in, double z_in)
{
	x = x_in;
	y = y_in;
	z = z_in;
}

double Vector::getX() const
{
	return x;
}

double Vector::getY() const
{
	return y;
}

double Vector::getZ() const
{
	return z;
}

const Vector Vector::operator*(const double &rhs) const
{
	Vector ret(0,0,0);
	ret.setX(getX() * rhs);
	ret.setY(getY() * rhs);
	ret.setZ(getZ() * rhs);
	
	return ret;
}

const Vector Vector::operator+(const Vector &rhs) const
{
	Vector ret(0,0,0);
	ret.setX(getX() + rhs.getX());
	ret.setY(getY() + rhs.getY());
	ret.setZ(getZ() + rhs.getZ());
	
	return ret;
}

const Vector Vector::operator/(const double &rhs) const
{
	Vector ret(0,0,0);
	ret.setX(getX() / rhs);
	ret.setY(getY() / rhs);
	ret.setZ(getZ() / rhs);
	
	return ret;
}

double Vector::getLength() const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

void Vector::setX(double x_in)
{
	x = x_in;
}

void Vector::print() const
{
	std::cout << "< " << getX() << " " << getY() << " " << getZ() << " >" << std::endl;
}

void Vector::setY(double y_in)
{
	y = y_in;
}

void Vector::setZ(double z_in)
{
	z = z_in;
}

void Vector::normalize()
{
	double length = getLength();
	x /= length;
	y /= length;
	z /= length;
}

Vector& Vector::operator=(const Vector &rhs)
{
	x = rhs.getX();
	y = rhs.getY();
	z = rhs.getZ();
	return *this;
}