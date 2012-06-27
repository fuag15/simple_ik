 #include "point.h"
 
 Point::Point()
 {
	 x = 0;
	 y = 0;
	 z = 0;
 }
 
 Point::Point(double x_in, double y_in, double z_in)
 {
	 x = x_in;
	 y = y_in;
	 z = z_in;
 }
 
 double Point::getX() const
 {
	 return x;
 }
 
 double Point::getY() const
 {
	 return y;
 }
 
 double Point::getZ() const
 {
	 return z;
 }
 
 void Point::setX(double x_in)
 {
	 x = x_in;
 }
 
 void Point::setY(double y_in)
 {
	 y = y_in;
 }
 
 void Point::setZ(double z_in)
 {
	 z = z_in;
 }
 
 void Point::print()
 {
	 std::cout << "( " << getX() << " " << getY() << " " << getZ() << " )" << std::endl;
 }
 
 Point& Point::operator=(const Point &rhs)
 {
	 x = rhs.getX();
	 y = rhs.getY();
	 z = rhs.getZ();
	 
	 return *this;
 }
 
 const Point Point::operator+(const Vector &rhs) const
 {
	 Point p(x + rhs.getX(), y + rhs.getY(), z + rhs.getZ());
	 return p;
 }
 
 const Vector Point::operator-(const Point &rhs) const
 {
	 Vector vec(x - rhs.getX(), y - rhs.getY(), z - rhs.getZ());
	 return vec;
 }