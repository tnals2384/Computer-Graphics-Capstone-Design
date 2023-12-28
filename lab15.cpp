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
#define PI 3.1415926537

double theta = 45.0;
double phi = 45.0;
int radius = 10;
double upVector = cos(phi * PI / 180);
double eyex, eyey, eyez;
GLuint g_nCubeTex;
GLUquadric* qobj = gluNewQuadric();
typedef unsigned char uchar;
void init();
void InvertMatrix(GLfloat m[16], GLfloat inv[16]);
void resize(int, int);
void light_default();
void draw();
void specialkeyboard(int, int, int);
void environmentMapSetting();
void draw_skyBox();
void draw_sphere();

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(600, 200);
	glutCreateWindow("12201869 김수민");
	init();

	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glutDisplayFunc(&draw);
	glutSpecialFunc(&specialkeyboard);

	glutMainLoop();
	return 0;
}

void init() {
	glClearColor(0, 0, 0, 1);
	glColor3f(1, 1, 1);

	glutReshapeFunc(resize); // resize 콜백 함수
	light_default(); // light setting

	/* TEXTURE MAPPING SET */

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	//environment mapping
	environmentMapSetting();

	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eyex = radius * sin(theta * PI / 180) * cos(phi * PI / 180);
	eyey = radius * sin(phi * PI / 180);
	eyez = radius * cos(theta * PI / 180) * cos(phi * PI / 180);
	upVector = cos(phi * PI / 180);
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);

	draw_skyBox();

	//glMatrixMode(GL_TEXTURE);

	//glPushMatrix();
	//GLfloat cam[16];
	////선택한 매개 변수의 값 또는 값을 반환
	//glGetFloatv(GL_MODELVIEW_MATRIX, cam);
	////distance에 따라 변하는 값이므로 0으로 바꿈
	//cam[14] = 0.0f;
	//GLfloat cam_inv[16];
	////역행렬 구하는 함수
	//InvertMatrix(cam, cam_inv);
	////현재 행렬에 임의의 행렬을 곱함
	//glMultMatrixf(cam_inv);

	////시선행렬의 역행렬 적용
	//glMatrixMode(GL_MODELVIEW);
	//draw_sphere();
	//glMatrixMode(GL_TEXTURE);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	glutSwapBuffers();
}

void draw_sphere() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glutSolidSphere(1.0f, 30, 30);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
}


void draw_skyBox() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);

	float g_nSkysize = 50; //큐브 사이즈

	glBegin(GL_QUADS);
	//px
	glTexCoord3d(1, -1, -1); glVertex3f(g_nSkysize, -g_nSkysize, -g_nSkysize);
	glTexCoord3d(1, -1, 1); glVertex3f(g_nSkysize, -g_nSkysize, g_nSkysize);
	glTexCoord3d(1, 1, 1); glVertex3f(g_nSkysize, g_nSkysize, g_nSkysize);
	glTexCoord3d(1, 1, -1); glVertex3f(g_nSkysize, g_nSkysize, -g_nSkysize);
	//nx
	glTexCoord3d(-1, -1, -1); glVertex3f(-g_nSkysize, -g_nSkysize, -g_nSkysize);
	glTexCoord3d(-1, -1, 1); glVertex3f(-g_nSkysize, -g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, 1, 1); glVertex3f(-g_nSkysize, g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, 1, -1); glVertex3f(-g_nSkysize, g_nSkysize, -g_nSkysize);
	//py
	glTexCoord3d(1, 1, 1); glVertex3f(g_nSkysize, g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, 1, 1); glVertex3f(-g_nSkysize, g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, 1, -1); glVertex3f(-g_nSkysize, g_nSkysize, -g_nSkysize);
	glTexCoord3d(1, 1, -1); glVertex3f(g_nSkysize, g_nSkysize, -g_nSkysize);
	//ny
	glTexCoord3d(1, -1, 1); glVertex3f(g_nSkysize, -g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, -1, 1); glVertex3f(-g_nSkysize, -g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, -1, -1); glVertex3f(-g_nSkysize, -g_nSkysize, -g_nSkysize);
	glTexCoord3d(1, -1, -1); glVertex3f(g_nSkysize, -g_nSkysize, -g_nSkysize);
	//pz
	glTexCoord3d(1, -1, 1); glVertex3f(g_nSkysize, -g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, -1, 1); glVertex3f(-g_nSkysize, -g_nSkysize, g_nSkysize);
	glTexCoord3d(-1, 1, 1); glVertex3f(-g_nSkysize, g_nSkysize, g_nSkysize);
	glTexCoord3d(1, 1, 1); glVertex3f(g_nSkysize, g_nSkysize, g_nSkysize);
	//nz
	glTexCoord3d(1, -1, -1); glVertex3f(g_nSkysize, -g_nSkysize, -g_nSkysize);
	glTexCoord3d(-1, -1, -1); glVertex3f(-g_nSkysize, -g_nSkysize, -g_nSkysize);
	glTexCoord3d(-1, 1, -1); glVertex3f(-g_nSkysize, g_nSkysize, -g_nSkysize);
	glTexCoord3d(1, 1, -1); glVertex3f(g_nSkysize, g_nSkysize, -g_nSkysize);
	glEnd();
}

void InvertMatrix(GLfloat m[16], GLfloat inv[16]) {
	m[14] = 0.f;

	inv[0] =
		+m[5] * m[10] * m[15]
		- m[5] * m[11] * m[14]
		- m[9] * m[6] * m[15]
		+ m[9] * m[7] * m[14]
		+ m[13] * m[6] * m[11]
		- m[13] * m[7] * m[10];

	inv[4] =
		-m[4] * m[10] * m[15]
		+ m[4] * m[11] * m[14]
		+ m[8] * m[6] * m[15]
		- m[8] * m[7] * m[14]
		- m[12] * m[6] * m[11]
		+ m[12] * m[7] * m[10];

	inv[8] =
		+m[4] * m[9] * m[15]
		- m[4] * m[11] * m[13]
		- m[8] * m[5] * m[15]
		+ m[8] * m[7] * m[13]
		+ m[12] * m[5] * m[11]
		- m[12] * m[7] * m[9];

	inv[12] =
		-m[4] * m[9] * m[14]
		+ m[4] * m[10] * m[13]
		+ m[8] * m[5] * m[14]
		- m[8] * m[6] * m[13]
		- m[12] * m[5] * m[10]
		+ m[12] * m[6] * m[9];

	inv[1] =
		-m[1] * m[10] * m[15]
		+ m[1] * m[11] * m[14]
		+ m[9] * m[2] * m[15]
		- m[9] * m[3] * m[14]
		- m[13] * m[2] * m[11]
		+ m[13] * m[3] * m[10];

	inv[5] =
		+m[0] * m[10] * m[15]
		- m[0] * m[11] * m[14]
		- m[8] * m[2] * m[15]
		+ m[8] * m[3] * m[14]
		+ m[12] * m[2] * m[11]
		- m[12] * m[3] * m[10];

	inv[9] =
		-m[0] * m[9] * m[15]
		+ m[0] * m[11] * m[13]
		+ m[8] * m[1] * m[15]
		- m[8] * m[3] * m[13]
		- m[12] * m[1] * m[11]
		+ m[12] * m[3] * m[9];

	inv[13] =
		+m[0] * m[9] * m[14]
		- m[0] * m[10] * m[13]
		- m[8] * m[1] * m[14]
		+ m[8] * m[2] * m[13]
		+ m[12] * m[1] * m[10]
		- m[12] * m[2] * m[9];

	inv[2] =
		+m[1] * m[6] * m[15]
		- m[1] * m[7] * m[14]
		- m[5] * m[2] * m[15]
		+ m[5] * m[3] * m[14]
		+ m[13] * m[2] * m[7]
		- m[13] * m[3] * m[6];

	inv[6] =
		-m[0] * m[6] * m[15]
		+ m[0] * m[7] * m[14]
		+ m[4] * m[2] * m[15]
		- m[4] * m[3] * m[14]
		- m[12] * m[2] * m[7]
		+ m[12] * m[3] * m[6];

	inv[10] =
		+m[0] * m[5] * m[15]
		- m[0] * m[7] * m[13]
		- m[4] * m[1] * m[15]
		+ m[4] * m[3] * m[13]
		+ m[12] * m[1] * m[7]
		- m[12] * m[3] * m[5];

	inv[14] =
		-m[0] * m[5] * m[14]
		+ m[0] * m[6] * m[13]
		+ m[4] * m[1] * m[14]
		- m[4] * m[2] * m[13]
		- m[12] * m[1] * m[6]
		+ m[12] * m[2] * m[5];

	inv[3] =
		-m[1] * m[6] * m[11]
		+ m[1] * m[7] * m[10]
		+ m[5] * m[2] * m[11]
		- m[5] * m[3] * m[10]
		- m[9] * m[2] * m[7]
		+ m[9] * m[3] * m[6];

	inv[7] =
		+m[0] * m[6] * m[11]
		- m[0] * m[7] * m[10]
		- m[4] * m[2] * m[11]
		+ m[4] * m[3] * m[10]
		+ m[8] * m[2] * m[7]
		- m[8] * m[3] * m[6];

	inv[11] =
		-m[0] * m[5] * m[11]
		+ m[0] * m[7] * m[9]
		+ m[4] * m[1] * m[11]
		- m[4] * m[3] * m[9]
		- m[8] * m[1] * m[7]
		+ m[8] * m[3] * m[5];

	inv[15] =
		+m[0] * m[5] * m[10]
		- m[0] * m[6] * m[9]
		- m[4] * m[1] * m[10]
		+ m[4] * m[2] * m[9]
		+ m[8] * m[1] * m[6]
		- m[8] * m[2] * m[5];

}

void light_default() {
	glClearColor(0, 0, 0, 1.0f);
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	GLfloat diffuseMaterial[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_AMBIENT, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void resize(int x, int y) {
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)x / (float)y, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}


void specialkeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_DOWN) {
		if (phi == 0) phi = 360;
		phi = phi - 2.5;
	}
	else if (key == GLUT_KEY_UP) {
		if (phi == 360) phi = 0;
		phi = phi + 2.5;
	}
	else if (key == GLUT_KEY_LEFT) {
		if (theta == 0) theta = 360;
		theta = theta - 2.5;
	}
	else if (key == GLUT_KEY_RIGHT) {
		if (theta == 360) theta = 0;
		theta = theta + 2.5;
	}
	glutPostRedisplay();
}

void environmentMapSetting() {
	glGenTextures(1, &g_nCubeTex);
	int width, height, channels;
	uchar* img0 = readImageData("universe.bmp", &width, &height, &channels);
	uchar* img1 = readImageData("skyTexture/1024nx.bmp", &width, &height, &channels);
	uchar* img2 = readImageData("skyTexture/1024py.bmp", &width, &height, &channels);
	uchar* img3 = readImageData("skyTexture/1024ny.bmp", &width, &height, &channels);
	uchar* img4 = readImageData("skyTexture/1024pz.bmp", &width, &height, &channels);
	uchar* img5 = readImageData("skyTexture/1024nz.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);

	//텍스처 속성 지정
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	//텍스처 자동 좌표 생성
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

}