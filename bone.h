#ifndef BONE_H
	#define BONE_H
	#include <cmath>
	#include <iostream>
	class Bone
	{
		float bLength;
		double bOrientation;
		
		public:
			Bone();
			Bone(float length);
			Bone(float length, double orientation);
			float getLength();
			double getOrientation();
			void setLength(float length);
			void setOrientation(double orientation);
			void changeOrientation(double orientation);
	};
#endif
