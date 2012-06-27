COMP_OPTS = -lSDL -lGL -lGLU `sdl-config --cflags --libs`

ik_test.out : main.o bone.o armature.o vector.o point.o matrix.o
	g++ -o ik_test.out main.o bone.o armature.o vector.o point.o matrix.o $(COMP_OPTS)
	
main.o: main.cpp main.h bone.h point.h armature.h
	g++ -c main.cpp $(COMP_OPTS)
	
armature.o: armature.cpp armature.h bone.h point.h vector.h matrix.h
	g++ -c armature.cpp
	
bone.o: bone.cpp bone.h
	g++ -c bone.cpp
	
vector.o: vector.cpp vector.h matrix.h
	g++ -c vector.cpp

point.o: point.cpp point.h vector.h
	g++ -c point.cpp
	
matrix.o: matrix.cpp matrix.h vector.h
	g++ -c matrix.cpp
	
clean:
	rm -f main.o bone.o armature.o vector.o point.o matrix.o