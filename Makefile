all:
	echo "make {generator,solver}"

generator:
	g++ generateMaze.cc -O3 -o generator

solver:
	g++ solveMaze.cc -O3 -lncurses -o solver

clean:
	rm generator solver
