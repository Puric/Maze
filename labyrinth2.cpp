#include<iostream>
#include<ctime>
#include<stack>
#include<stdlib.h>
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

using namespace std;

//------------------------------------------------------------------------------------------------

#define WIDTH 600
#define HEIGHT 600
int keyflag = 0;
const int QUAD_DIMENTION = WIDTH / 37;

const int row = 37;
const int column = 37;



//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------



#define MOVE 1
#define QUIET 2
//-----------------------------------------------

void display();;
void keyboard(unsigned char c,int x,int y);
void idle();

//-----------------------------------------------

class Player {
  float x,y;   //-- Current position
  float vx,vy; //-- Velocity vector
  int state;

  long time_remaining;

public:

  Player();
  void set_position(int x,int y);
  void init_movement(int destination_x,int destination_y,int duration);
  void integrate(long t);
  void draw();

  float getPositionX(){return x;}
  float getPositionY(){return y;}
};


//-----------------------------------------------

Player::Player()
{
  state=QUIET;
}

//-----------------------------------------------

void Player::set_position(int x,int y)
{
  this->x = x;
  this->y = y;
}

//-----------------------------------------------

void Player::init_movement(int destination_x,int destination_y,int duration)
{
  vx = (destination_x - x)/duration;
  vy = (destination_y - y)/duration;

  state=MOVE;
  time_remaining=duration;  
}

//-----------------------------------------------

void Player::integrate(long t)
{
  if(state==MOVE && t<time_remaining)
    {
      x = x + vx*t;
      y = y + vy*t;
      time_remaining-=t;
    }
  else if(state==MOVE && t>=time_remaining)
    {
      x = x + vx*time_remaining;
      y = y + vy*time_remaining;
      state=QUIET;
    }
}

//-----------------------------------------------

void Player::draw()
{
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glVertex2i(x-6,y-6);
  glVertex2i(x+6,y-6);
  glVertex2i(x+6,y+6);
  glVertex2i(x-6,y+6);
  glEnd();
}



//-----------------------------------------------
// -- MAIN
//-----------------------------------------------

Player square;
long last_t=0;



//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

enum SquareType { Corridor, Wall, Start };
class Square {
	SquareType type;
	char display;		//what will square display 0 for wall, ' '(space) for corridor, S for start point 

						//variables of all possible neighbours of current square
	bool neighbour_top;
	bool neighbour_left;
	bool neighbour_bottom;
	bool neighbour_right;

public:
	bool visited;		//variable if square has been visited
	Square() {
		type = Wall;
		visited = false;
	}

	//change the state
	void changeType(SquareType t) { type = t; }
	void SquareVisited(bool v) { visited = v; }
	void changeDisplay(char d) { display = d; }
	void changeNeighbourTop(bool a) { neighbour_top = a; }
	void changeNeighbourLeft(bool a) { neighbour_left = a; }
	void changeNeighbourRight(bool a) { neighbour_right = a; }
	void changeNeighbourBottom(bool a) { neighbour_bottom = a; }

	//Getters
	SquareType getType() { return type; }
	char returnDisplay() { return display; }
	//bool isVisited() { return visited; }
	bool availableNeighbourTop() { return neighbour_top; }
	bool availableNeighbourBottom() { return neighbour_bottom; }
	bool availableNeighbourLeft() { return neighbour_left; }
	bool availableNeighbourRight() { return neighbour_right; }

};


void drawQuad(int i, int j, int width, int height);

class Maze {
	//Square **matrix;
	Square matrix[row][column];
	//int row;
	//int column;
public:
	Maze(int r, int c) {
		//setting all matrix to be a wall
		for (int i = 0; i<r; i++) {
			for (int j = 0; j<c; j++) {
				matrix[i][j].changeDisplay('0');
				matrix[i][j].SquareVisited(false);
				matrix[i][j].changeNeighbourTop(true);;
				matrix[i][j].changeNeighbourBottom(true);
				matrix[i][j].changeNeighbourLeft(true);
				matrix[i][j].changeNeighbourRight(true);
			}
		}


		for (int i = 1; i<r - 1; i++) {
			matrix[i][0].SquareVisited(true);
			matrix[i][c - 1].SquareVisited(true);
			matrix[i][1].SquareVisited(true);
			matrix[i][c - 2].SquareVisited(true);
			for (int j = 1; j<c - 1; j++) {
				// Border Cells have fewer accessible walls
				matrix[1][j].changeNeighbourTop(false);
				matrix[r - 2][j].changeNeighbourBottom(false);
				matrix[i][1].changeNeighbourLeft(false);
				matrix[i][c - 2].changeNeighbourRight(false);
				matrix[0][j].SquareVisited(true);
				matrix[r - 1][j].SquareVisited(true);
				matrix[1][j].SquareVisited(true);
				matrix[r - 2][j].SquareVisited(true);

			}
		}
	}


	int getRow() { return row; }
	int getColumn() { return column; }

	void Redraw() {
		for (int i = 0; i<row; i++) {
			cout << endl;
			matrix[i][0].changeDisplay(' ');
			matrix[i][column - 1].changeDisplay(' ');
			for (int j = 0; j < column; j++) {
				cout << " " << matrix[i][j].returnDisplay();
				matrix[0][j].changeDisplay(' ');
				matrix[row - 1][j].changeDisplay(' ');
			}
		}
		cout << endl;
		int counterR;
		int counterC;
		for (size_t i = 0; i < row; i++)
		{

			for (size_t i = 0; i < column; i++)
			{

			}
		}
	}
	void GraficDraw() {
		for (size_t i = 0; i < row; i++)
		{
			for (size_t j = 0; j < column; j++)
			{
				if (matrix[i][j].returnDisplay() == '0') {
					glColor3f(0, 0, 1);
					drawQuad(j, i, QUAD_DIMENTION, QUAD_DIMENTION);
				}
				else if (matrix[i][j].returnDisplay() == ' ') {
					glColor3f(0, 1, 0);
					drawQuad(j, i, QUAD_DIMENTION, QUAD_DIMENTION);
				}
				else if (matrix[i][j].returnDisplay() == 'S') {
					glColor3f(1, 0, 0);
					drawQuad(j, i, QUAD_DIMENTION, QUAD_DIMENTION);

				}
			}
		}
		glutPostRedisplay();
	}

	void makeStartPositions() {
		for (int i = row - 1; i > 0; i--)
		{
			for (int j = 0; j < column; j++)
			{
				if (matrix[i][j].returnDisplay() == ' ') {
					matrix[i][j].changeType(Start);
					matrix[i][j].changeDisplay('S');
					return;
				}
			}
		}
	}

	void makeEndPositions() {
		for (int i = 0; i < row; i++)
		{
			for (int j = column; j > 0; j--)
			{
				if (matrix[i][j].returnDisplay() == ' ') {
					matrix[i][j].changeType(Start);
					matrix[i][j].changeDisplay('S');
					return;
				}
			}
		}
	}


	void make() {
		srand(time(NULL));

		int randX;
		int randY;
		do
		{
			randX = ((2 * rand()) + 1) % (column - 1);
		} while (!(randX > 3 && randX < column - 4));

		do
		{
			randY = ((2 * rand()) + 1) % (row - 1);

		} while (!(randY > 3 && randY < row - 4));
		int visitedX = randX;
		int visitedY = randY;
		int visitedCells = 1;
		int totalCells = 0;
		int brojac = 0;
		/*	if(visitedX % 2 == 0 && visitedY % 2 == 0)
		totalCells = ((row - 3) / 2)*((column - 3) / 2);
		else if (visitedX % 2 == 1 && visitedY % 2 == 0)
		totalCells = ((row - 4) / 2)*((column - 3) / 2);
		else if (visitedX % 2 == 0 && visitedY % 2 == 1)
		totalCells = ((row - 3) / 2)*((column - 4) / 2);
		else if (visitedX % 2 == 1 && visitedY % 2 == 1)
		totalCells = ((row - 4) / 2)*((column - 4) / 2);*/
		stack<int> back_trackX, back_trackY;
		matrix[randY][randX].changeDisplay(' ');
		matrix[randY][randX].SquareVisited(true);

		do {
			if (((matrix[randY - 2][randX].visited == false) && (matrix[randY][randX].availableNeighbourTop() == true) && (matrix[randY - 2][randX].availableNeighbourBottom() == true)) ||
				((matrix[randY + 2][randX].visited == false) && (matrix[randY + 2][randX].availableNeighbourTop() == true) && (matrix[randY][randX].availableNeighbourBottom() == true)) ||
				((matrix[randY][randX - 2].visited == false) && (matrix[randY][randX].availableNeighbourLeft() == true) && (matrix[randY][randX - 2].availableNeighbourRight() == true)) ||
				((matrix[randY][randX + 2].visited == false) && (matrix[randY][randX + 2].availableNeighbourLeft() == true) && (matrix[randY][randX].availableNeighbourRight() == true))) {
				int random = rand() % 4 + 1;
				if ((random == 1) && (randY > 1)) {
					if (matrix[randY - 2][randX].visited == false) {

						matrix[randY - 1][randX].changeDisplay(' ');
						matrix[randY - 1][randX].SquareVisited(true);
						matrix[randY][randX].changeNeighbourTop(false);
						back_trackX.push(randX);
						back_trackY.push(randY);
						randY -= 2;
						matrix[randY][randX].changeDisplay(' ');
						matrix[randY][randX].SquareVisited(true);
						matrix[randY][randX].changeNeighbourBottom(false);
						visitedCells++;
					}
					else
						continue;
				}
				if ((random == 2) && (randY < row - 2)) {
					if (matrix[randY + 2][randX].visited == false) {

						matrix[randY + 1][randX].changeDisplay(' ');
						matrix[randY + 1][randX].SquareVisited(true);
						matrix[randY][randX].changeNeighbourBottom(false);
						back_trackX.push(randX);
						back_trackY.push(randY);
						randY += 2;
						matrix[randY][randX].changeDisplay(' ');
						matrix[randY][randX].SquareVisited(true);
						matrix[randY][randX].changeNeighbourTop(false);
						visitedCells++;
					}
					else
						continue;
				}
				if ((random == 3) && (randX > 1)) {
					if (matrix[randY][randX - 2].visited == false) {

						matrix[randY][randX - 1].changeDisplay(' ');
						matrix[randY][randX - 1].SquareVisited(true);
						matrix[randY][randX].changeNeighbourLeft(false);
						back_trackX.push(randX);
						back_trackY.push(randY);
						randX -= 2;
						matrix[randY][randX].changeDisplay(' ');
						matrix[randY][randX].SquareVisited(true);
						matrix[randY][randX].changeNeighbourRight(false);
						visitedCells++;
					}
					else
						continue;
				}
				if ((random == 4) && (randX < column - 2)) {
					if (matrix[randY][randX + 2].visited == false) {

						matrix[randY][randX + 1].changeDisplay(' ');
						matrix[randY][randX + 1].SquareVisited(true);
						matrix[randY][randX].changeNeighbourRight(false);
						back_trackX.push(randX);
						back_trackY.push(randY);
						randX += 2;
						matrix[randY][randX].changeDisplay(' ');
						matrix[randY][randX].SquareVisited(true);
						matrix[randY][randX].changeNeighbourLeft(false);
						visitedCells++;
					}
					else
						continue;
				}
			}
			else {
				randX = back_trackX.top();
				back_trackX.pop();
				randY = back_trackY.top();
				back_trackY.pop();
				brojac++;
			}
		} while (!back_trackX.empty());
	}
};

Maze m(37, 37);

void display()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	
	//m.make();
//	m.makeStartPositions();
//	m.makeEndPositions();
	m.GraficDraw();

	square.draw();

	glutSwapBuffers();
}




float xx = 55;
float yy = 536;
  
void keyboard(unsigned char c, int x, int y)
{
	
 switch (c){
        case 'w':
            square.init_movement(xx,yy+10,200);
            square.set_position(xx,yy);
            yy +=10;
          break;

        case 'a':
            square.init_movement(xx-10,yy,200);
            square.set_position(xx,yy);
            xx -=10;
          break;

        case 's':
            square.init_movement(xx,yy-10,200);
            square.set_position(xx,yy);
            yy -=10;
          break;
        case 'd':
            square.init_movement(xx+10,yy,200);
            square.set_position(xx,yy);
			xx +=10;  			
          break;
		case 'r':
			m = Maze(m.getRow(), m.getColumn());
			m.make();
			m.makeStartPositions();
			m.makeEndPositions();		

			break;
        default:
            break;
    }
	
	glutPostRedisplay();
};









void drawQuad(int i, int j, int width, int height) {
	glBegin(GL_QUADS);

	glVertex2i(i*width, j*height);
	glVertex2i((i + 1)*width, j*height);
	glVertex2i((i + 1)*width, (j + 1)*height);
	glVertex2i(i*width, (j + 1)*height);

	glEnd();
}


int main(int argc, char *argv[]) {
	srand(time(NULL));
	square.set_position(55,536);

	m.make();
	m.makeStartPositions();
	m.makeEndPositions();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 500);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Maze-game");

	  
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	glutDisplayFunc(display);


	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, WIDTH - 1, 0, HEIGHT - 1);

	glutMainLoop();

	//system("pause");
	return 0;
}



void idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME); 

  if(last_t==0)
    last_t=t;
  else
    {
      square.integrate(t-last_t);
      last_t=t;
    }


  glutPostRedisplay();
}
