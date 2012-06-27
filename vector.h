 #ifndef VECTOR_H
	#define VECTOR_H
	#include <cmath>
	#include <iostream>
	
	class Vector
	{
		double x;
		double y;
		double z;
		
		public:
			Vector();
			Vector(double x_in, double y_in, double z_in);
			double getX() const;
			double getY() const;
			double getZ() const;
			double getLength() const;
			void setX(double x_in);
			void setY(double y_in);
			void print() const;
			void setZ(double z_in);
			void normalize();
			Vector& operator=(const Vector &rhs);
			const Vector operator+(const Vector &rhs) const;
			const Vector operator*(const double &rhs) const;
			const Vector operator/(const double &rhs) const;
	};
#endif