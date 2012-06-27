 #ifndef POINT_H
	#define POINT_H
	#include <iostream>
	#include "vector.h"
	
	class Point
	{
		double x;
		double y;
		double z;
		
		public:
			Point();
			Point(double x_in, double y_in, double z_in);
			double getX() const;
			double getY() const;
			double getZ() const;
			void setX(double x_in);
			void setY(double y_in);
			void setZ(double z_in);
			void print();
			Point& operator=(const Point &rhs);
			const Point operator+(const Vector &rhs) const;
			const Vector operator-(const Point &rhs) const;
	};
#endif