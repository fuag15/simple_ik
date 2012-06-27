 #include "bone.h"
 
 Bone::Bone()
 {
	 bLength = 10.0;
	 bOrientation = 0.0;
	 return;
 }
 
 Bone::Bone(float length)
 {
	 bLength = length;
	 bOrientation = 0.0;
	 return;
 }
 
 Bone::Bone(float length, double orientation)
 {
	 bLength = length;
	 bOrientation = orientation;
	 return;
 }
 
float Bone::getLength()
{
	return bLength;
}

double Bone::getOrientation()
{
	return bOrientation;
}

void Bone::changeOrientation(double orientation)
{
	double circles;
	bOrientation += orientation;
	circles = floor(fabs(bOrientation / 360.0));
	if(bOrientation >= 360.0)
	{
		bOrientation -= circles * 360.0;
	}
	else if(bOrientation <= -360.0)
	{
		bOrientation += circles * 360.0;
	}
	
	if(bOrientation > 180.0)
 	{
 		bOrientation = -180.0 + (bOrientation - 180.0);
 	}
 	else if(bOrientation < -180.0)
	{
		bOrientation = 180.0 + (bOrientation + 180.0);
	}
}

void Bone::setLength(float length)
{
	bLength = length;
}

void Bone::setOrientation(double orientation)
{
	bOrientation = orientation;
}