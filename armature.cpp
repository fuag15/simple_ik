 #include "armature.h"
 
 Armature::Armature()
 {
	 solutionFound = true;
	 maxLength = 0.0;
	 speed = 5.0;
	 animToDest = false;
	 boneCursor = 0;
	 numBones = 0;
 }
 
 
 Armature::Armature(double x, double y)
 {
	 solutionFound = true;
	 maxLength = 0.0;
	 speed = 5.0;
	 animToDest = false;
	 boneCursor = 0;
	 numBones = 0;
	 worldOffset.setX(x);
	 worldOffset.setY(y);
 }
 
 void Armature::setX(double x)
 {
	 //only call these before adding bones
	 worldOffset.setX(x);
 }
 
 void Armature::setY(double y)
 {
	 //only call these before adding bones
	 worldOffset.setY(y);
 }
 
 double Armature::getX()
 {
	 return root.getX() + worldOffset.getX();
 }
 
 double Armature::getY()
 {
	 return root.getY() + worldOffset.getY();
 }
 
 void Armature::addBone(float length, double orientation)
 {
	 if(numBones < 10)
	 {
		 bones[numBones] = new Bone(length, orientation);
		 testBones[numBones] = new Bone(length, orientation);
		 numBones++;
		 
		 maxLength += length;
		 
		 Matrix temp(2, numBones);
		 jacobian = temp;
		 
		 computeTip();
	 }
 }
 
 void Armature::resetCursor()
 {
	 boneCursor = 0;
 }
 
 bool Armature::getNext(Bone* &nextBone)
 {
	 if(boneCursor >= numBones)
	 {
		 return false;
	 }
	 
	 nextBone = bones[boneCursor];
	 boneCursor++;
	 
	 return true;
 }
 
 void Armature::computeJacobian(double samplesize)
 {
	//only for 2d
	
	Point localOff(0,0,0);
	Vector temp(0,0,0);
	Vector currentOff(0,0,0);
	
	for(int j = 0; j < numBones; j++)
	{
		currentOff = computeOffset(j, samplesize, localOff);
		
		jacobian.setAt(0, j, samplesize / currentOff.getX());
		jacobian.setAt(1, j, samplesize / currentOff.getY());
		
		temp.setX(0);
		temp.setY(0);
		temp.setZ(0);
		rotation.rotate2d(testBones[j] -> getOrientation());
		temp.setX(testBones[j] -> getLength());
		temp = rotation * temp;
		localOff = localOff + temp;
	}
 }
 
 void Armature::animateStep(double seconds, double samplesize)
 {
	if(animToDest)
	{
		if(!solutionFound)
		{
			computeAngles(samplesize);
			setUpMotionTween();
		}
		syncToActual(seconds);
		isSolved();
	}
 }
 
 void Armature::isSolved()
 {
	 computeTip();
	 Vector dX(0,0,0);
	 dX = destination - tip;
	 if(dX.getLength() <= 2.0)
	 {
		 animToDest = false;
	 }
 }
 
 void Armature::syncBones()
 {
	 for(int i = 0; i < numBones; i++)
	 {
		 testBones[i] -> setOrientation(bones[i] -> getOrientation());
	 }
 }
 
 void Armature::setUpMotionTween()
 {
	 double destAngle;
	 double sourceAngle;
	 double offset;
	 tweenAngles = angles;
	 for(int i = 0; i < numBones; i++)
	 {
		 destAngle = testBones[i] -> getOrientation();
		 sourceAngle = bones[i] -> getOrientation();
		 offset = destAngle - sourceAngle;
		 offset = sanatizeAngle(offset);
		 offset /= speed;
		 tweenAngles.setAt(0, i, offset);
	 }
 }
 
 void Armature::syncToActual(double seconds)
 {
	 double totalAngle;
	 double thisRound;
	 for(int i = 0; i < numBones; i++)
	 {
		 
		bones[i] -> changeOrientation(seconds * tweenAngles.getAt(0,i));
	 }
 }
 
 void Armature::computeAngles(double samplesize)
 {
	syncBones();
	computeTestTip();
	Matrix errorResult(2);
	Matrix jacobianTranspose(3);
	Matrix jacobianInverse(3);
	Matrix pseudoInverse(3);
	Matrix identity2d(2);
	Matrix shouldBeId(2);
	Matrix errorM(2);
	Vector error(0,0,0);
	Matrix dXmatrix(2, 1);
	Matrix resultAngles(2);
	Vector dX(0,0,0);
	dX = destination - testTip;
	Point temp;
	while(dX.getLength() > 2.0)
	{
		computeJacobian(samplesize);
		jacobianTranspose = jacobian.getTranspose();
		pseudoInverse = jacobian * jacobianTranspose;
		pseudoInverse.invert();
		jacobianInverse = jacobianTranspose * pseudoInverse;
		dXmatrix.setAt(0,0, dX.getX());
		dXmatrix.setAt(1,0, dX.getY());
		angles = jacobianInverse * dXmatrix;
		fixAngles();
		updateAngles();
		computeTestTip();
		dX = destination - testTip;
	}
	solutionFound = true;
 }
 
 double Armature::sanatizeAngle(double angle)
 {
	double circleCount;
	
	circleCount = floor(fabs(angle / 360.0));
	if(angle > 360.0)
	{
		angle -= circleCount * 360.0;
	}
	else if(angle < -360.0)
	{
		angle += circleCount * 360.0;
	}
	
	if(angle > 180.0)
	{
		angle = -180.0 + (angle - 180.0);
	}
	else if(angle < -180.0)
	{
		angle = 180.0  + (angle + 180.0);
	}
	
	return angle;
 }
 
 void Armature::fixAngles()
 {
	 for(int i = 0; i < numBones; i++)
	 {
		angles.setAt(0, i, sanatizeAngle(angles.getAt(0, i)));
	 }
 }
 
 void Armature::updateAngles()
 {
	 for(int i = 0; i < numBones; i++)
	 {
		 testBones[i] -> changeOrientation(angles.getAt(0, i));
	 }
	 jitterTestBones();
 }
 
 void Armature::jitterTestBones()
 {
	for(int i = 0; i < numBones; i++)
	{
		if(testBones[i] -> getOrientation() == 0.0)
		{
			testBones[i] -> changeOrientation(0.01);
		}
	}
 }
 
 void Armature::setDestination(Point newDest)
 {
	 animToDest = true;
	 solutionFound = false;
	 destination.setX(newDest.getX() - worldOffset.getX());
	 destination.setY(newDest.getY() - worldOffset.getY());
	 destination.setZ(newDest.getZ() - worldOffset.getZ());
	 Vector temp;
	 temp = destination - root;
	 if(temp.getLength() > maxLength)
	 {
		 temp.normalize();
		 temp = temp * (maxLength * 0.93);
		 destination = root + temp;
	 }
 }
 
 void Armature::defaultPose()
 {
	 animToDest = false;
	 for(int i = 0; i < numBones; i++)
	 {
		 bones[i] -> setOrientation(0.0);
		 testBones[i] -> setOrientation(0.0);
	 }
	 jitterBones();
	 computeTip();
 }
 
 void Armature::jitterBones()
 {
	 for(int i = 0; i < numBones; i++)
	 {
		 if(bones[i] -> getOrientation() == 0.0)
		 {
			 bones[i] -> changeOrientation(0.01);
		 }
		 
		 if(testBones[i] -> getOrientation() == 0.0)
		 {
			 testBones[i] -> changeOrientation(0.01);
		 }
	 }
 }
 
 Vector Armature::computeOffset(int root, double samplesize, Point localOffset)
 {
	 Vector final(0,0,0);
	 Vector temp(0,0,0);
	 Point offTemp(0,0,0);
	 
	 for(int i = root; i < numBones; i++)
	 {
		 temp.setX(0);
		 temp.setY(0);
		 temp.setZ(0);
		 if(i == root)
		 {
			rotation.rotate2d((testBones[i] -> getOrientation()) + samplesize);
		 }
		 else
		 {
			 rotation.rotate2d(testBones[i] -> getOrientation());
		 }
		 temp.setX(testBones[i] -> getLength());
		 temp = rotation * temp;
		 final = final + temp;
	 }
	 
	 offTemp = localOffset + final;
	 final = offTemp - tip;
	 
	 return final;
 }
 
 void Armature::setSpeed(double s)
 {
	 speed = s;
 }
 
 void Armature::computeTestTip()
 {
	 Vector final(0,0,0);
	 Vector temp(0,0,0);
	 double accumulatedAngle = 0.0;
	 double circle = 0;
	 for(int i = 0; i < numBones; i++)
	 {
		 temp.setX(0);
		 temp.setY(0);
		 temp.setZ(0);
		 accumulatedAngle += testBones[i]->getOrientation();
		 accumulatedAngle = sanatizeAngle(accumulatedAngle);
		 rotation.rotate2d(accumulatedAngle);
		 temp.setX(testBones[i] -> getLength());
		 temp = rotation * temp;
		 final = final + temp;
	 }
	 
	 testTip.setX(final.getX());
	 testTip.setY(final.getY());
	 testTip.setZ(final.getZ());
 }
 
 void Armature::computeTip()
 {
	 Vector final(0,0,0);
	 Vector temp(0,0,0);
	 double accumulatedAngle = 0.0;
	 double circle = 0;
	 for(int i = 0; i < numBones; i++)
	 {
		 temp.setX(0);
		 temp.setY(0);
		 temp.setZ(0);
		 accumulatedAngle += bones[i]->getOrientation();
		 accumulatedAngle = sanatizeAngle(accumulatedAngle);
		 rotation.rotate2d(accumulatedAngle);
		 temp.setX(bones[i] -> getLength());
		 temp = rotation * temp;
		 final = final + temp;
	 }
	 
	 tip.setX(final.getX());
	 tip.setY(final.getY());
	 tip.setZ(final.getZ());
 }
 
 Armature::~Armature()
 {
	 for(int i = 0; i < numBones; i++)
	 {
		 free(bones[i]);
		 free(testBones[i]);
	 }
 }