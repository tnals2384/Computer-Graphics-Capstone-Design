#pragma once

#include "bmpfuncs.h"
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <gl/glut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable : 4996)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265


volatile double theta = 45.0;
volatile double phi = 45.0;
volatile int radius = 18;
volatile double upVector = cos(phi*PI / 180);

volatile double *angle = new double[7]();
volatile double rotateFactor = 1.0;
bool spinEnable = true;
bool display_axis = false;
bool set_materialProperty = true;
int current_width, current_height;

/* 광원 설정 모드 */
volatile unsigned char lightMode = 0;

/* 시야 설정 */
volatile double eyex, eyey, eyez;

/* texture mapping set variable */
GLuint texName[6];	// make cube
GLuint texName2[3];	// make cylinder
GLuint *texName3 = new GLuint();
GLuint tmp;

/* quadric object 생성 */
GLUquadricObj* qobj = gluNewQuadric();

/* object selector */
volatile unsigned char selector = 4;

void init(void);
void resize(int, int);

void draw(void);
void draw_axis(void);
void draw_textureCube(void);
void draw_textureCube3(void);

void draw_cylinder(void);
void draw_sphere(void);
void draw_string(void*, const char*, float, float, float, float, float);
void draw_text(void);

void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);

void idle(void);

void popup_menu(void);
void sub_menu_function(int option);
void main_menu_function(int option);

void light_default(void);
void printInstruction(void);
void setDefault(void);

void cubeTextureMapping(void);
void cylinderTextureMapping(void);
void sphereTextureMapping(void);

void light_sel(void);

typedef unsigned char uchar;

int main(int argc, char **argv) {
	/* Initialize window */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// GLUT_DOUBLE , GLUT_RGBA로 권장

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(400, 300);
	glutCreateWindow("12201869 김수민 lab11");
	init();

	/* CPU 유후 시간 처리 */
	glutIdleFunc(idle);

	/* quadric object 속성 설정 */
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	/* Define Draw Callback */
	glutDisplayFunc(draw);

	/* 키보드 인터럽트 처리 */
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);

	/* Popup Menu */
	popup_menu();

	/* Main Loop Start! */
	glutMainLoop();

	return 0;
}

void init(void) {
	/* 화면의 기본색으로 Black 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	glutReshapeFunc(resize); // resize 콜백 함수
	light_default(); // light setting
	printInstruction(); // 조작법 출력 

	/* TEXTURE MAPPING SET */
	gluQuadricTexture(qobj, GL_TRUE);

	cubeTextureMapping();
	cylinderTextureMapping();
	sphereTextureMapping();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

}

void draw(void) {

	/* 화면을 깨끗하게 지우기 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 물체의 앞/뒤 관계를 위한 DEPTH 추가
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);	// 뒤에 있는 object가 앞으로 보이는 현상 제거

	eyex = radius * sin(theta*PI / 180)*cos(phi*PI / 180);
	eyey = radius * sin(phi*PI / 180);
	eyez = radius * cos(theta*PI / 180)*cos(phi*PI / 180);
	upVector = cos(phi*PI / 180);

	//차례대로 x에서 보는 위치, y에서 보는 위치, z에서 보는 위치
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);
	draw_text();
	glDisable(GL_LIGHT1);
	light_sel();

	/*  큐브 */
	glPushMatrix();
	if (selector == 4)
		glTranslatef(2, 0, 2);
	if (selector == 1 || selector == 4)
		draw_textureCube();
		//draw_textureCube3();
	glPopMatrix();

	/* 사이다 */
	glPushMatrix();
	if (selector == 4)
		glTranslatef(2, 0, -2);
	if (selector == 2 | selector == 4)
		draw_cylinder();
	glPopMatrix();

	/* 지구 */
	glPushMatrix();
	if (selector == 4)
		glTranslatef(-2.82, 0, 0);
	if (selector >= 3)
		draw_sphere();
	glPopMatrix();

	/* 실제로 출력하는 함수 */
	glutSwapBuffers();
}

void resize(int width, int height) {

	glViewport(0, 0, width, height);
	current_width = width;
	current_height = height;
	printf("WIDTH : %d  HEIGHT : %d\n", width, height);
	glMatrixMode(GL_PROJECTION);	/* 3D 투영을 위한 프로젝션 선언 */
	glLoadIdentity();

	gluPerspective(45, (float)width / (float)height, 1, 500);	
	glMatrixMode(GL_MODELVIEW);
}

void draw_axis(void) {
	if (!display_axis) goto out1;
	glLineWidth(1.5);	// 좌표 축의 두께 설정

	glBegin(GL_LINES);
	glColor3f(0.5, 0, 0);	// x axis color : red
	glVertex3f(0, 0, 0);
	glVertex3f(3, 0, 0);

	glColor3f(0, 0.5, 0);	// y axis color : green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 3, 0);

	glColor3f(0, 0, 0.5);	// z axis : blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 3);
	glEnd();

	glLineWidth(1);		// 좌표 축의 두께 : 1(default)
out1:
	glutPostRedisplay();	// call draw() callback func
}

void idle(void) {
	double rf = rotateFactor;

	/* spin변수가 변하는 곳! */
	if (!spinEnable) goto out3;
	angle[0] += 0.5*rf;		// 
	angle[1] += 0.05*rf;		// 
	angle[2] += 0.3*rf;		// 
	angle[3] += 0.1*rf;		// 
	angle[4] += 0.5*rf;		// 
	angle[5] += 0.2*rf;		//
	angle[6] += 0.4*rf;		//

	if (angle[1] > 360 || angle[0] > 360) {
		angle[0] -= 360;
		angle[1] -= 360;
	}
	if (angle[2] > 360) angle[2] -= 360;
	if (angle[3] > 360) angle[3] -= 360;
	if (angle[4] > 360) angle[4] -= 360;
	if (angle[5] > 360) angle[5] -= 360;
	if (angle[6] > 360) angle[6] -= 360;
out3:
	glutPostRedisplay();	// call draw() callback func
}



void popup_menu(void) {
	int submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("cube", 1);
	glutAddMenuEntry("cylinder", 2);
	glutAddMenuEntry("sphere", 3);
	glutAddMenuEntry("all", 4);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Clear", 900);

	glutAddSubMenu("view mode", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void sub_menu_function(int option) {
	switch (option) {
	case 1:
		lightMode = 0;
		selector = 1;
		break;
	case 2:
		lightMode = 0;
		selector = 2;
		break;
	case 3:
		lightMode = 0;
		selector = 3;
		break;
	case 4:
		lightMode = 0;
		selector = 4;
		break;
	default:
		break;
	}
	printf("object viewmode%d\n", selector);
	glutPostRedisplay();
}

void main_menu_function(int option) {
	if (option == 999) {
		exit(0);
	}
	else if (option == 900) {
		setDefault();
	}
	glutPostRedisplay();
}



void light_default() {
	glClearColor(0, 0, 0, 1.0f);

	/* Light0 조명 관련 설정 */
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };


	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}



void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case '0':
		lightMode = 0;
		printf("light%d at camera\n", lightMode);
		break;
	case '1':
		lightMode = 1;
		printf("light%d at cube\n", lightMode);
		break;
	case '2':
		lightMode = 2;
		printf("light%d at cylinder\n", lightMode);
		break;
	case '3':
		lightMode = 3;
		printf("light%d at sphere\n", lightMode);
		break;
	default: 	printf("you pressed %c\n", key); break;
	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		theta -= 1;
		if (theta <= -360) theta = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		theta += 1;
		if (theta >= 360) theta = 0.0;
		break;
	case GLUT_KEY_DOWN:
		phi -= 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case GLUT_KEY_UP:
		phi += 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case 5:
		printInstruction();
		setDefault();
		break;
	default:
		printf("%d is pressed\n", key);
		break;
	}
	glutPostRedisplay();
}

void printInstruction() {
	/* 조작법 console 출력 */
	printf("\n------------keyboard navigation------------\n");
	printf("0 : 광원 Camera\n");
	printf("1 : 광원 Cube\n");
	printf("2 : 광원 Cylinder\n");
	printf("3 : 광원 Sphere\n");
	printf("방향키 : camera 위치\n");
}

void setDefault() {
	radius = 18;
	selector = 4;
	lightMode = 0;
	theta = 45.0;
	phi = 45.0;
	rotateFactor = 1.0;
	spinEnable = true;
	display_axis = false;
	set_materialProperty = true;
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void draw_text() {
	char* str = (char*)"Light(camera)";
	switch (lightMode) {
	case 0:
		str = (char*)"Light(camera)";
		break;
	case 1:
		str = (char*)"Light(cube)";
		break;
	case 2:
		str = (char*)"Light(cylinder)";
		break;
	case 3:
		str = (char*)"Light(sphere)";
		break;
	default:
		break;
	}
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, str, -9.5, 8.7, 1, 1, 0);
}

void light_sel() {
	if (lightMode == 0) {
		GLfloat light_position[] = { eyex,eyey,eyez,1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (lightMode == 1) {
		GLfloat light_position[] = { 3.5,0,3.5,1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (lightMode == 2) {
		GLfloat light_position[] = { 2,0,-2,1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (lightMode == 3) {
		GLfloat light_position[] = { -2.8,0,0,1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
}

void cubeTextureMapping() {
	glGenTextures(6, texName);
	int imgWidth, imgHeight, channels;
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		char buf[100];
		sprintf(buf, "img/TexImage%d.bmp", i);
		buf[strlen(buf)] = 0;
		uchar* img = readImageData(buf, &imgWidth, &imgHeight, &channels);
		glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL*/3, imgWidth, imgHeight, 0, /*TEXEL CHANNEL*/GL_RGB, GL_UNSIGNED_BYTE, img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	}
}

void draw_textureCube() {
	// white로 color를 set해주어야 texture색상이 제대로 적용됨
	glColor3f(1.0, 1.0, 1.0);	

	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0, 0);	// -x axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);	//x axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1.0, 0);	// -y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0);	// y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0);	//z axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[5]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0);	//-z축
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	draw_axis();
}

void draw_textureCube3() {
	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0, 0);	// -x axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(3, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(3, 3); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 3); glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);	//x axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(3, 0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(3, 3); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0, 3); glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1.0, 0);	// -y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(3, 0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(3, 3); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0, 3); glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0);	// y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(3, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(3, 3); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0, 3); glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0);	//z axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(3, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(3, 3); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 3); glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[5]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0);	//-z axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(3, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(3, 3); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0, 3); glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	draw_axis();
}

void draw_cylinder() {
	glRotatef(-90, 1, 0, 0);	// 깡통 세우기
	glTranslatef(0, 0, -1);		// 다른 object랑 y축으로 맞추기
	glColor3f(1.0, 1.0, 1.0);

	/* cylinder top view */
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glBindTexture(GL_TEXTURE_2D, texName2[0]);
	gluPartialDisk(qobj, 0, 0.7, 30, 3, 0, 360);
	glPopMatrix();

	/* cylinder middle */
	glBindTexture(GL_TEXTURE_2D, texName2[1]);
	gluCylinder(qobj, 0.7, 0.7, 2, 20, 20);

	/* cylinder bottom view */
	glPushMatrix();
	glRotatef(180, 1, 0, 0);	// normal vector 설정과 같음
	glBindTexture(GL_TEXTURE_2D, texName2[2]);
	gluPartialDisk(qobj, 0, 0.7, 30, 3, 0, 360);
	glPopMatrix();
	draw_axis();
}

void draw_sphere() {
	glRotatef(-90, 1, 0, 0);
	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, *texName3);
	gluSphere(qobj, 0.7, 100, 100);

	draw_axis();
}



void cylinderTextureMapping() {
	glGenTextures(3, texName2);

	glBindTexture(GL_TEXTURE_2D, texName2[0]);
	int width, height, channels;
	uchar* img = readImageData("img/CIDER_T.bmp",&width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName2[1]);
	img = readImageData("img/coke.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName2[2]);
	img = readImageData("img/CIDER_B.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


}

void sphereTextureMapping() {
	glGenTextures(1, texName3);
	uchar* img;
	int width, height, channels;

	glBindTexture(GL_TEXTURE_2D, *texName3);
	img = readImageData("img/EARTH.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}