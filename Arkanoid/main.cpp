#include <GL/glut.h>
#include <math.h>
#include "Ball.h"
#include "Brick.h"
#include "vars.h"

float r_x = 50.0, r_y = 290.0, r_w = 80.0, r_h = 8.0;
int score = 0;
int lives = 3;
int level = 1;
int count = 60;

Ball ball(6);
Brick brick[10][15];

bool isCollision(Ball &ball, Brick &brick)
{
	float x = ball.x, y = ball.y, r = ball.r, col = brick.col, w = brick.width, row = brick.row, h = brick.height;
	if (brick.active)
	{
		if (fabs(x - row * w - w / 2 - 2) <= r + w / 2 && fabs(y - col * h - h / 2 - 2) <= r + h / 2)
		{
			if (sqrt((row*w + w / 2 - x)*(row*w + w / 2 - x) + (col*h + h / 2 - y)*(col*h + h / 2 - y))
				- (w / 2 - 1)*sqrt(2.0) - r > r*(sqrt(2.0) - 1))
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

void DrawText(const char *text, int length, int x, int y)
{
	glColor3f(0.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 400, 300, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}

void DrawBricks()
{
	glColor3f(0.1, 0.2, 0.8);
	glBegin(GL_QUADS);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			Brick &b = brick[i][j];
			if (b.active)
			{
				glVertex2f(b.row * b.width + 1, b.col * b.height + 1);
				glVertex2f(b.row * b.width + b.width - 1, b.col * b.height + 1);
				glVertex2f(b.row * b.width + b.width - 1, b.col * b.height + b.height - 1);
				glVertex2f(b.row * b.width + 1, b.col * b.height + b.height - 1);
			}
		}
	}
	glEnd();
}

void Reset()
{
	for (int i = 0; i < level + 3; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			brick[i][j].active = true;
			brick[i][j].col = i;
			brick[i][j].row = j;
			brick[i][j].hit = 1;
		}
	}
}

void DrawPlatform()
{
	glColor3f(0.2, 0.8, 0.1);
	glRectf(r_x, r_y, r_x + r_w, r_y + r_h);
}

void DrawBall()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (float i = 0; i < 2 * 3.14; i += 3.14 / 4)
	{
		glVertex2f(ball.x + ball.r * sin(i), ball.y + ball.r * cos(i));
	}
	glEnd();
}

void DrawLine()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(301, 0);
	glVertex2f(301, 300);
	glEnd();
}

void DrawResults()
{
	// Draw score
	char str[3];
	if (score >= 0 && score <= 9)
	{
		str[0] = score + '0';
	}
	if (score >= 10 && score <= 99)
	{
		str[0] = score / 10 + '0';
		str[1] = score % 10 + '0';
	}
	if (score >= 100 && score <= 999)
	{
		str[0] = score / 100 + '0';
		str[1] = score % 100 / 10 + '0';
		str[2] = score % 10 + '0';
	}
	DrawText("Score: ", 7, 305, 15);
	DrawText(str, 3, 362, 15);
	// Draw lives
	char live[1];
	live[0] = lives + '0';
	DrawText("Lives: ", 7, 305, 35);
	DrawText(live, 1, 362, 35);
	// Draw level
	char lvl[1];
	lvl[0] = level + '0';
	DrawText("Level: ", 7, 305, 55);
	DrawText(lvl, 1, 362, 55);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawPlatform();
	DrawBall();
	DrawBricks();
	DrawLine();
	DrawResults();
	glutSwapBuffers();
}

void MousePress(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && !ball.active)
	{
		ball.active = true;
		ball.dx = 4;
		ball.dy = -4;
	}
}

void Mouse(int ax, int ay)
{
	if(ax <= 260 && ax >= 40)
		r_x = ax - r_w/2;
	if (!ball.active)
	{
		ball.x = r_x + r_w / 2;
		ball.y = r_y - ball.r;
	}
}

void StartGame()
{
	for (int i = 0; i < level + 3; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			brick[i][j].active = true;
			brick[i][j].col = i;
			brick[i][j].row = j;
			brick[i][j].hit = 1;
		}
	}
}

void Won()
{
	DrawText("You won", 7, 305, 55);
}

void Timer(int)
{
	if (lives == 0)
	{
		lives = 3;
		Reset();
	}
	if (ball.active)
	{
		ball.Move();
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				Brick &b = brick[i][j];
				if (isCollision(ball, b))
				{
					if (fabs(ball.x - b.row*b.width - b.width / 2) < fabs(ball.y - b.col*b.height - b.height / 2))
					{
						ball.dy *= -1;
					}
					else if (fabs(ball.x - b.row*b.width - b.width / 2) > fabs(ball.y - b.col*b.height - b.height / 2))
					{
						ball.dx *= -1;
					}
					else
					{
						if (ball.dx > 0)
						{
							if (ball.x < b.row*b.width + 1) 
								ball.dx *= -1;
						}
						else if (ball.x > (b.row + 1)*b.width - 1) 
							ball.dx *= -1;
						if (ball.dy > 0)
						{
							if (ball.y < b.col*b.height + 1) 
								ball.dy *= -1;
						}
						else if (ball.y > (b.col + 1)*b.height - 1) 
							ball.dy *= -1;
					}
					if (--b.hit == 0)
					{
						b.active = false;
					}
					score++;
					if (score == count)
					{
						ball.active = false;
						score = 0;
						if (level == 7)
						{
							for (int i = 0; i < level + 3; i++)
							{
								for (int j = 0; j < 15; j++)
								{
									brick[i][j].active = true;
									brick[i][j].col = i;
									brick[i][j].row = j;
									brick[i][j].hit = 1;
								}
							}
							level = 0;
						}
						level++;
						lives = 3;
						count += 15;
						StartGame();
					}
					goto o;
				}
			}
		}
	}
o:	display();
	glutTimerFunc(33, Timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 200);
	glutCreateWindow("Arkanoid");
	glutDisplayFunc(display);
	glutTimerFunc(33, Timer, 0);
	glutPassiveMotionFunc(Mouse);
	glutMouseFunc(MousePress);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 400, 300, 0, 1, 0);
	StartGame();
	glutMainLoop();

	return 0;
}