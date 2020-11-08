#define FREEGLUT_STATIC
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include <windows.h>

//declare the necessary function
void create_circle(GLfloat x, GLfloat y, GLfloat r);
void create_curve();
void DDALine(int x0, int y0, int x1, int y1);
void drawBezier(double x1, double x2, double x3, double y1, double y2, double y3);

const double a = 25.0f, b = 35.0f;//Major axis and minor axis of ellipse
const double P = 3.1415926536f;//pi
const double CIRCLE_POINT_NUM = 200;//use to draw the circle and ellipse
//the screen size
const int width = 600;
const int height = 400;
//use to show the balloon animation and mouse control
GLfloat balloon_position_x = 500;
GLfloat balloon_position_y = 300;
int step = 2;//the step of balloon
GLfloat i;//counter
GLfloat rotate = 30;//the angle of pointer
GLfloat trans_x = 0;//the x-axis of viewing translation
GLfloat trans_y;//the y-axis of viewing translation
GLfloat scale = 1;//scale of the view
GLfloat line_control = 10;//control the line in circle
GLfloat control_step = 2;//the step of the lines

//initialize the viewing and background
void display_init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 600, 0, 400);

	glClearColor(1, 1, 0.9, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}

//x, y indicate the postion
void create_balloon(int x,int y) {
	//draw the ellipse
	glBegin(GL_TRIANGLE_FAN);
	for (i = 1; i <= CIRCLE_POINT_NUM; i++)
	{
		glVertex2f(a * cos(2 * P * (double)i / (double)CIRCLE_POINT_NUM) + balloon_position_x + x,
			b * sin(2 * P * (double)i / (double)CIRCLE_POINT_NUM) + balloon_position_y + y);
	}
	glEnd();

	glColor3f(0.6f, 0.5f, 0.8f);
	glBegin(GL_LINES);
	glVertex2f(balloon_position_x + x, balloon_position_y - b + y);
	glVertex2f(balloon_position_x + x, balloon_position_y - b - 70 + y);
	glEnd();

}

void create_candle(GLfloat x, GLfloat y) {
	glColor3f(1.0f, 1.0f, 0.8f);
	glBegin(GL_POLYGON);
	glVertex2f(x, y); 
	glVertex2f(x, y + 60);
	glVertex2f(x + 5, y + 60);         
	glVertex2f(x + 5, y); 
	glEnd();

	glLineWidth(1);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x, y + 60);
	glVertex2f(x, y + 60);
	glVertex2f(x + 5, y + 60);
	glVertex2f(x + 5, y + 60);
	glVertex2f(x + 5, y);
	glEnd();

	//draw the fire
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (i = 1; i <= CIRCLE_POINT_NUM; i++)
	{
		glVertex2f(2 * cos(2 * P * i / (double)CIRCLE_POINT_NUM) + x + 2,
			(5 * sin(2 * P * i / (double)CIRCLE_POINT_NUM)) + y + 60);
	}
	glEnd();
}

//draw a tree using translate and rotate
void create_tree(int x, int y) {
	glPushMatrix();//matrix1
	glLineWidth(6);
	glTranslatef(x, y, 0);
	int length = 200;
	int angle;
	//the root
	glColor3f(0.6f, 0.8f, 0.2f);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, length);
	glEnd();

	//draw the branches
	glPushMatrix();//matrix2
	for (angle = 25; angle <= 150; angle += 25) {
		length = length / 2;
		glTranslatef(0, length, 0);
		//the left side of left side
		glRotatef(angle, 0, 0, 1);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, length);
		glEnd();
		//the right side of left side
		glRotatef(-angle * 2, 0, 0, 1);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, length);
		glEnd();
		glRotatef(angle * 2, 0, 0, 1);
	}
	glPopMatrix();//matrix2
	//symetric operation
	length = 200;
	glPushMatrix();//matrix3
	for (angle = -25; angle >= -150; angle -= 25) {
		length = length / 2;
		glTranslatef(0, length, 0);
		//the left side of right side
		glRotatef(angle, 0, 0, 1);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, length);
		glEnd();
		//the right side of right side
		glRotatef(-angle*2, 0, 0, 1);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, length);
		glEnd();
		glRotatef(angle * 2, 0, 0, 1);
	}
	glPopMatrix();//matrix3
	glPopMatrix();//matrix1
}

void create_curve() {
	//using sin function to draw the curve
	GLfloat curve_y = 0;
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0.9f);
	double temp = 0;
	for (i = 1; i <= 100; i++)
	{
		glVertex2f(i, curve_y/2);
		temp = temp + P / 50;
		curve_y = curve_y + sin((double)P + temp);
	}
	glEnd();
}

void drawBezier(double x1, double x2, double x3, double y1, double y2, double y3)
{
	//bezier algrithm
	//the parameters are 3 controll point
	GLfloat Bezier_x;
	GLfloat Bezier_y;
	double t = 0.0;
	float last_x = pow((1 - t), 2) * x1 + 2 * t * (1 - t) * x2 + pow(t, 2) * x3;;
	float last_y = pow((1 - t), 2) * y1 + 2 * t * (1 - t) * y2 + pow(t, 2) * y3;
	for (t = 0.0; t <= 1.0; t += 0.1) {
		Bezier_x = pow((1 - t), 2) * x1 + 2 * t * (1 - t) * x2 + pow(t, 2) * x3;
		Bezier_y = pow((1 - t), 2) * y1 + 2 * t * (1 - t) * y2 + pow(t, 2) * y3;
		glBegin(GL_LINES);
		glVertex2f(Bezier_x, Bezier_y);
		glVertex2f(last_x, last_y);
		glEnd();
		glFlush();
		last_x = Bezier_x;
		last_y = Bezier_y;
	}
}

//draw a simple line
void DDALine(int x0, int y0, int x1, int y1)
{
	int dx, dy, n, k;
	float xinc, yinc, x, y;
	dx = x1 - x0;
	dy = y1 - y0;
	if (abs(dx) > abs(dy))
		n = abs(dx);
	else
		n = abs(dy);

	xinc = (float)dx / n;
	yinc = (float)dy / n;
	x = (float)x0;
	y = (float)y0;
	for (k = 1; k <= n; k++)
	{ 
		glVertex2f(x, y);
		x += xinc; // Increment in x direction
		y += yinc; // Increment in y direction
	}
}

void create_circle(GLfloat x, GLfloat y, GLfloat r) {
	//using Bresenham algrithm
	glBegin(GL_TRIANGLES);
	for (i = 1; i <= (double)CIRCLE_POINT_NUM / 8; i += 2)
	{
		double x_i, y_i;
		x_i = r * cos(2 * P * i / (double)CIRCLE_POINT_NUM) + x;
		y_i = r * sin(2 * P * i / (double)CIRCLE_POINT_NUM) + y;
		glVertex2f(x_i, y_i);
		glVertex2f(x_i, -y_i);
		glVertex2f(-x_i, y_i);
		glVertex2f(-x_i, -y_i);
		glVertex2f(y_i, x_i);
		glVertex2f(y_i, -x_i);
		glVertex2f(-y_i, x_i);
		glVertex2f(-y_i, -x_i);
	}
	glEnd();
}

void create_compass() {
	glColor3f(0.2f, 0.5f, 0.8f);
	
	//using Bresenham algrithm
	create_circle(0, 0, 300);

	//draw 2 triangles to make the multi color pointer
	//set a matrix for rotation
	glPushMatrix();
	glRotatef(rotate, 0, 0, 1);
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);
	glVertex2f(200, 100);
	glVertex2f(223, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(286, 70);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex2f(200, 100);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex2f(223, 0);
	glEnd();
	glPopMatrix();
}

void create_cake() {
	glPushMatrix();
	glTranslatef(450, 150, 0);//translate to origin

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.9f, 0.8f, 0.4f);
	for (i = 1; i <= CIRCLE_POINT_NUM; i++)
	{
		glVertex2f(100 * cos(2 * P * i / (double)CIRCLE_POINT_NUM),
			(100 * sin(2 * P * i / (double)CIRCLE_POINT_NUM)) * 1 / 2);
	}
	glEnd();

	//draw and rotate the SIN curve
	for (int j = 0; j < 30; j++) {
		glRotatef(15, 0, 0, 1);
		create_curve();
	}
	glPopMatrix();//the curve go back

	glBegin(GL_POINTS);
	glColor3f(0.9f, 0.8f, 0.4f);
	DDALine(350, 150, 350, 50);
	glEnd();

	//bezier line
	drawBezier(350, 450, 550, 50, -20, 50);

	glBegin(GL_POINTS);
	glColor3f(0.9f, 0.8f, 0.4f);
	DDALine(550, 150, 550, 50);
	glEnd();
}

void dynamic_line(GLfloat x, GLfloat y) {
	glColor3f(1.0f, 0.5f, 0.0f);
	create_circle(x, y, 20);
}

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	//create new object insteat last one
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // if it is the first call, initial
						// Avoid out of memory(OOM) error
		isFirstCall = 0;
		// generate 128 serial number for ASCII
		lists = glGenLists(128);
		// The drawing command of each character is loaded into the corresponding display list
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	// display the string
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}
void OnTimer(int value) {
	if (line_control >= 180 || line_control<= -180) {
		control_step = -control_step;
	}
	if (balloon_position_y - b - 70 >= 400) {
		balloon_position_y = 0;
	}
	balloon_position_y += step;
	line_control += control_step;
	// when_in_mainloop();
	glutTimerFunc(15, OnTimer, 1);
}

void keyboard_input(unsigned char key, int x, int y) {// keyboard interaction
	if (key == 'q' || key == 'Q') {
		if (rotate >= 0 && rotate < 63) {
			rotate = rotate + 3;
		}
	}
	else if (key == 'e' || key == 'E') {
		if (rotate > 0 && rotate <= 63) {
			rotate = rotate - 3;
		}
	}
	else if (key == 'w' || key == 'W') {
		if (trans_y >= -200*scale) {
			trans_y = trans_y - 8;
		}
	}
	else if (key == 's' || key == 'S') {
		if (trans_y <= 100*scale) {
			trans_y = trans_y + 8;
		}
	}
	else if (key == 'a' || key == 'A') {
		if (trans_x <= 100*scale) {
			trans_x = trans_x + 8;
		}
	}
	else if (key == 'd' || key == 'D') {
		if (trans_x >= -150*scale) {
			trans_x = trans_x - 8;
		}
	}
	else if (key == 'r' || key == 'R') {
		if (scale <= 1.5) {
			scale = scale + 0.1;
		}
	}
	else if (key == 'f' || key == 'F') {
		if(scale >= 1.1)
			scale = scale - 0.1;
	}		
	else if (key == 'x' || key == 'X') {
		scale = 1;
		trans_y = 0;
		trans_x = 0;
		rotate = 30;
		glPolygonMode(GL_BACK, GL_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);
	}
}

void mouse_input(int button, int state, int x, int y) { // mouse interaction
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		step = 0;
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		step = 1;
		glPolygonMode(GL_BACK, GL_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);
	}
}
		

void display(void) {
	display_init();

	glPushMatrix();//The matrix
	glTranslatef(trans_x, trans_y, 0);
	glScalef(scale, scale, 0);

	//draw the tree
	create_tree(280, 100);

	//change the line width
	glLineWidth(2);

	//draw a dynamic line in the bottom
	dynamic_line(line_control, 200);

	//draw the compass
	create_compass();
	
	//draw the cake and candle
	create_cake();
	create_candle(390, 120);
	create_candle(420, 150);
	create_candle(470, 150);
	create_candle(500, 120);

	//draw the 2 balloons
	glColor3f(1.0f, 0.5f, 0.0f);
	create_balloon(0, 0);
	glColor3f(0.6f, 0.8f, 0.2f);
	create_balloon(-400, -100);

	//draw the string
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");//can only draw english letter
	glColor3f(1.0f, 0.5f, 0.0f);
	glRasterPos2f(180.0, 350.0);//set the start position of string
	drawString("Happy Birthday!");
	glRasterPos2f(410.0f, 50.0);
	drawString("Cake!");
	glPopMatrix();//The matrix
	glutSwapBuffers();
}
int main(int argc, char** argv) {
	//initial setting
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//double cache to diplay the animation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Birthday Card");

	//create the content
	glutDisplayFunc(display);
	glutIdleFunc(when_in_mainloop);
	glutTimerFunc(15, OnTimer, 1);

	glutKeyboardFunc(keyboard_input); // keyboard callback function
	glutMouseFunc(mouse_input);//mouse callback function
	glutMainLoop();
}