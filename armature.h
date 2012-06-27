#ifndef ARMATURE_H
	#define ARMATURE_H
	#include <stdlib.h>
	#include <cmath>
	#include <iostream>
	#include "matrix.h"
	#include "vector.h"
	#include "bone.h"
	#include "point.h"
	
	class Armature
	{
		int boneCursor;
		int numBones;
		Bone* bones[10];
		Bone* testBones[10];
		bool animToDest;
		bool solutionFound;
		double speed;
		Matrix jacobian;
		Matrix rotation;
		Matrix angles;
		Matrix tweenAngles;
		Point root;
		Point destination;
		Point tip;
		Point testTip;
		Vector worldOffset;
		double maxLength;
		
		public:
			Armature();
			Armature(double x, double y);
			void addBone(float length, double orientation);
			void resetCursor();
			bool getNext(Bone* &nextBone);
			void setX(double x);
			void setSpeed(double s);
			void setDestination(Point newDest);
			void syncBones();
			void jitterTestBones();
			void setUpMotionTween();
			void setY(double y);
			void isSolved();
			void fixAngles();
			void jitterBones();
			double sanatizeAngle(double angle);
			void computeAngles(double samplesize);
			double getX();
			double getY();
			void syncToActual(double seconds);
			void defaultPose();
			void updateAngles();
			void computeJacobian(double samplesize);
			Vector computeOffset(int root, double samplesize, Point localOffset);
			void animateStep(double seconds, double samplesize);
			void computeTip();
			void computeTestTip();
			~Armature();
	};
#endif