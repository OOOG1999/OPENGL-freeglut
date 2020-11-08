# Functions

## Initialize the display function.

	void display_init() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 600, 0, 400);
	glClearColor(1, 1, 0.9, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	}


## Initialize the window.  OnTimer: **15 ms, timer 1.**

```
void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}

void OnTimer(int value) {
	// when_in_mainloop();
	glutTimerFunc(15, OnTimer, 1);
}

void keyboard_input(unsigned char key, int x, int y) {// keyboard interaction
	if (key == 'q' || key == 'Q') {
	//something
	}
}

void mouse_input(int button, int state, int x, int y) { // mouse interaction
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
	//the mouse release
	}
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
```

## Select font and draw string.

Using ```glRasterPos2f(180.0, 350.0);``` to set the position.

```
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
```

## Draw a circle (Bresenham algorithm)

```
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
```

## Draw a DDA line

```
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
```

## Draw a Bezier line

```
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
```

## Sin function

```
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
```

## Draw ellipse

```
glBegin(GL_TRIANGLE_FAN);
	for (i = 1; i <= CIRCLE_POINT_NUM; i++)
	{
		glVertex2f(a * cos(2 * P * (double)i / (double)CIRCLE_POINT_NUM) + balloon_position_x + x,
			b * sin(2 * P * (double)i / (double)CIRCLE_POINT_NUM) + balloon_position_y + y);
	}
	glEnd();
```

