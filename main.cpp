/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include "1605088_classes.h"
#include<vector>
#include <iostream>
#include <fstream>

#include <string>
#include <cstring>
#include <stack>
#include <math.h>
#include<iomanip>
#include<algorithm>
#include<bits/stdc++.h>
#include <sstream>






#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;

vector<Object*> objects;
vector<Light*> lights;




double translateY;

struct point position, u, r, l;



void drawAxes()
{
	if (drawaxes == 1)
	{

		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES); {
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, 100, 0);
			glVertex3f(0, -100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES); {
			for (i = -8;i <= 8;i++) {

				if (i == 0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}glEnd();
	}
}

void drawCircle(double radius, int segments)
{
	int i;
	struct point points[100];
	glColor3f(0.7, 0.7, 0.7);
	//generate points
	for (i = 0;i <= segments;i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * pi);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * pi);
	}
	//draw segments using generated points
	for (i = 0;i < segments;i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}


void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
	{
		double c = cos((10.0f * 3.1416) / 180);
		double s = sin((10.0f * 3.1416) / 180);
		struct point temp;
		temp.x = l.x;
		temp.y = l.y;
		temp.z = l.z;
		l.x = l.x * c - r.x * s;
		l.y = l.y * c - r.y * s;
		l.z = l.z * c - r.z * s;

		r.x = r.x * c + temp.x * s;
		r.y = r.y * c + temp.y * s;
		r.z = r.z * c + temp.z * s;


		break;
	}

	case '2':
	{
		double c = cos((10.0f * 3.1416) / 180);
		double s = sin((10.0f * 3.1416) / 180);
		struct point temp;
		temp.x = l.x;
		temp.y = l.y;
		temp.z = l.z;
		l.x = l.x * c + r.x * s;
		l.y = l.y * c + r.y * s;
		l.z = l.z * c + r.z * s;

		r.x = r.x * c - temp.x * s;
		r.y = r.y * c - temp.y * s;
		r.z = r.z * c - temp.z * s;
		break;
	}


	case '3':
	{
		double c = cos((10.0f * 3.1416) / 180);
		double s = sin((10.0f * 3.1416) / 180);


		//rotating l vector
		l.x = l.x * c + u.x * s;
		l.y = l.y * c + u.y * s;
		l.z = l.z * c + u.z * s;
		//rotating u vector
		u.x = u.x * c - l.x * s;
		u.y = u.y * c - l.y * s;
		u.z = u.z * c - l.z * s;


		break;
	}
	case '4':
	{
		double c = cos((10.0f * 3.1416) / 180);
		double s = sin((10.0f * 3.1416) / 180);
		l.x = l.x * c - u.x * s;
		l.y = l.y * c - u.y * s;
		l.z = l.z * c - u.z * s;

		u.x = u.x * c - u.x * s;
		u.y = u.y * c - u.y * s;
		u.z = u.z * c - u.z * s;
		break;
	}

	case '6':
	{
		double c = cos((10.0f * 3.1416) / 180);
		double s = sin((10.0f * 3.1416) / 180);
		struct point temp;
		temp.x = u.x;
		temp.y = u.y;
		temp.z = u.z;
		u.x = u.x * c + r.x * s;
		u.y = u.y * c + r.y * s;
		u.z = u.z * c + r.z * s;

		r.x = r.x * c - temp.x * s;
		r.y = r.y * c - temp.y * s;
		r.z = r.z * c - temp.z * s;
		break;

	}

	case '5':
	{
		double c = cos((10.0f * 3.1416) / 180);
		double s = sin((10.0f * 3.1416) / 180);
		struct point temp;
		temp.x = u.x;
		temp.y = u.y;
		temp.z = u.z;
		u.x = u.x * c - r.x * s;
		u.y = u.y * c - r.y * s;
		u.z = u.z * c - r.z * s;

		r.x = r.x * c + temp.x * s;
		r.y = r.y * c + temp.y * s;
		r.z = r.z * c + temp.z * s;
		break;

	}

	default:
		break;
	}
}


void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:		//down arrow key
		position.x = position.x - l.x * 2;
		position.y = position.y - l.y * 2;
		position.z = position.z - l.z * 2;

		//cameraHeight -= 3.0;
		break;
	case GLUT_KEY_UP:		// up arrow key
		position.x = position.x + l.x * 2;
		position.y = position.y + l.y * 2;
		position.z = position.z + l.z * 2;
		//cameraHeight += 3.0;
		break;

	case GLUT_KEY_RIGHT:
		//r.x+=2;
		position.x = position.x + r.x * 2;
		position.y = position.y + r.y * 2;
		position.z = position.z + r.z * 2;
		//cameraAngle += 0.03;
		break;
	case GLUT_KEY_LEFT:
		position.x = position.x - r.x * 2;
		position.y = position.y - r.y * 2;
		position.z = position.z - r.z * 2;
		break;

	case GLUT_KEY_PAGE_UP:
		position.x = position.x + u.x * 2;
		position.y = position.y + u.y * 2;
		position.z = position.z + u.z * 2;

		break;
	case GLUT_KEY_PAGE_DOWN:
		position.x = position.x - u.x * 2;
		position.y = position.y - u.y * 2;
		position.z = position.z - u.z * 2;
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		break;
	case GLUT_KEY_END:
		break;

	default:
		break;
	}
}


void mouseListener(int button, int state, int x, int y) {	//x, y is the x-y of the screen (2D)
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//........
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}




void display() {
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(position.x, position.y, position.z, position.x + l.x, position.y + l.y, position.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
	drawAxes();

    vector<Object*>::iterator iter, end;
    vector<Light*>::iterator iter1,end1;
	 for(iter = objects.begin(), end = objects.end() ; iter != end; ++iter)
     {
            (*iter)->draw();
     }
     for(iter1 = lights.begin(), end1 = lights.end() ; iter1 != end1; ++iter1)
     {
         (*iter1)->draw();
     }






	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate() {
	//angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}



void init() {
	//codes for initialization
	position.x = 100;
	position.y = 90;
	position.z = 20;

	cameraHeight = 150.0;
	cameraAngle = 1.0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -1 / sqrt(2);
	r.y = 1 / sqrt(2);
	r.z = 0;

	l.x = -1 / sqrt(2);
	l.y = -1 / sqrt(2);
	l.z = 0;
	translateY = 0;

    drawaxes = 1;



	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}



void loadData()
{
	ifstream inputfile;
	inputfile.open("F:\\Study\\L-4-T-1\\Computer Graphics Sessional\\Offline 3\\1605088\\1605088_main\\scene.txt");
	int recursionLevel=0,pixel=0,numberOfObjects=0 ;
	double a=0.0, b=0.0, c=0.0, d=0.0;
	if (inputfile.is_open())
	{
	    inputfile>>recursionLevel;
		inputfile>>pixel;
		inputfile>>numberOfObjects;
		string tp;
		while (inputfile>>tp)
		{

			int e;
			if (tp == "sphere")
			{
			    inputfile>>a >> b >> c;
                Vector3D point(a,b,c);
                inputfile>>d;
                Object *temp;
                temp = new Sphere(point,d);
              //  Sphere sphere(point,d);

				 inputfile >> a >> b >> c;
				 double col[3];
				 col[0] = a;
				 col[1] = b;
				 col[2] = c;
				 temp->setColor(col);


				 inputfile>> a >> b >> c >> d;
				 double coeff[4];
				 coeff[0] = a;
				 coeff[1] = b;
				 coeff[2] = c;
				 coeff[3] = d;
				 temp->setCoEfficients(coeff);
				 inputfile>> e;
				 temp->setShine(e);
				 objects.push_back(temp);

			}
			else if (tp == "triangle")
			{

				inputfile>> a >> b >> c;

				Vector3D x(a, b, c);
				inputfile>> a >> b >> c;
				Vector3D y(a, b, c);
				inputfile>> a >> b >> c;
				Vector3D z(a, b, c);
				Object *temp;
				temp = new Triangle(x,y,z);
				//Triangle triangle(x, y, z);
				inputfile>> a >> b >> c;
				double col[] = { a,b,c};
				temp->setColor(col);
				inputfile>> a >> b >> c >> d;
				double coeff[4] = { a,b,c,d };
				temp->setCoEfficients(coeff);
				inputfile>> e;
				cout<<"shine"<<e<<endl;
				temp->setShine(e);
				objects.push_back(temp);
			}

			else if(tp == "general")
            {

            }
            else if(tp == "floor")
            {
                Object* temp;
                inputfile>>a>>b;
                temp = new Floor(a,b);
                objects.push_back(temp);
            }
            else{
                int noOfLight=0;
                stringstream g(tp);
                g>>noOfLight;
                double d,f,h;
                for(int i=0;i<noOfLight;i++)
                {
                    inputfile>>a>>b>>c;
                    inputfile>>d>>f>>h;
                    Vector3D point(a,b,c);
                    double c[3] = {d,f,h};
                    Light *temp;
                    temp = new Light(point,c);
                    //Light light(point,c);
                    lights.push_back(temp);
                }

            }
		}
		}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

	init();


	loadData();




	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
