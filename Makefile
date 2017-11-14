labyrinth: labyrinth.cpp
	gcc -lglut -lGLU -lGL -lm labyrinth.cpp -o labyrinth
clean:
	rm labyrinth
