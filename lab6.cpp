#include <gl/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
void init(void);
void draw(void);
void main_menu_function(int option);
void resize(int width, int height);
void draw_axis(void);
void mouseWheel(int button, int dir, int x, int y);
void special_keyboard(int ky, int x, int y);
double radian(int);


double radius = 20;
double theta = 45, phi = 45;
double cam[3] ;
double center[3] = { 0,0,0 };
double up[3] = { 0,1,0 };
const double pi = 3.14;

// ������ �������� ��ȯ
double radian(int value) {
	return (value * (pi / 180));
}

int main(int argc, char **argv) {
	/*openGL ��ǥ�迡 ���� cam ��ġ ����*/
	cam[0] = radius * sin(radian(theta)) * sin(radian(phi));
	cam[1] = radius * cos(radian(theta));
	cam[2] = radius * sin(radian(theta)) * cos(radian(phi));
	/* Window �ʱ�ȭ */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("����� Lab6");
	init(); // ����� �ʱ�ȭ �Լ�

	/* ���� �޴� */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // ������ ���콺 ������ �� �޴� �߰��ϱ�

	/* Callback �Լ� ���� */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw); // ���� �׸��� �Լ�
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(special_keyboard); // Ű���� �Է� 
	/* Looping ���� */
	glutMainLoop();

	return 0;
}
void init(void)
{
	/* ȭ���� �⺻�� ���� */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/* ȭ�� ��ǥ ���� ���� */
	glMatrixMode(GL_PROJECTION); //����Ǹ�带 ������ķ� ����
}

// â ũ�� ���� �ݹ� �Լ�
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height,1, 500);
	glMatrixMode(GL_MODELVIEW);
	printf("resize is called\n");
}

void draw_axis(void)
{
	glLineWidth(3); // ��ǥ���� �β�
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); // X���� red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0); // Y���� green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1); // Z���� blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1); // �β� �ٽ� ȯ��
}

 
void draw(void) {
	/* ȭ���� �����ϰ� ����� */
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	// resize() �Լ����� viewport�� projection T.F�� �̹� ����
	glColor3f(1, 1, 0);
	glutWireTeapot(4);
	draw_axis();
	glutSwapBuffers(); // double buffering��

	glFlush();
}


// ���콺 �� �̺�Ʈ�� ó���Ͽ� Ȯ�� �� ���
void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) { 
		radius -= 1.0; 
		if (radius < 1.0) {
			radius = 1.0; // �ּ� Ȯ�� ���� ����
		}
	}
	else { 
		radius += 1.0; 
		if (radius > 50.0) {
			radius = 50.0; // �ִ� ��� ���� ����
		}
	}
	// ���ο� ī�޶� ��ġ ���
	cam[0] = radius * sin(radian(theta)) * sin(radian(phi));
	cam[1] = radius * cos(radian(theta));
	cam[2] = radius * sin(radian(theta)) * cos(radian(phi));
	glutPostRedisplay();
}

//����Ű�� viewing transform
void special_keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		phi -= 5;
		if (phi <= 0) phi = 360;
	}

	if (key == GLUT_KEY_RIGHT) {
		phi += 5;
		if (phi >= 360) phi = 0;
	}

	if (key == GLUT_KEY_DOWN) {
		if (int(theta) % 180 == 0)
			up[1] = -1;

		theta += 5;

		if (theta > 360) {
			theta =5;
			up[1] = 1;
		}
	}

	if (key == GLUT_KEY_UP) {
		theta -= 5;

		if (int(theta) % 180 == 0)
			up[1] = 1;

		if (theta <= 0) {
			theta = 360;
			up[1] = -1;
		}
	}
	// ���ο� ī�޶� ��ġ ���
	cam[0] = radius * sin(radian(theta)) * sin(radian(phi));
	cam[1] = radius * cos(radian(theta));
	cam[2] = radius * sin(radian(theta)) * cos(radian(phi));
	printf("theta : %d, phi : %d\n", int(theta), int(phi));
	glutPostRedisplay();
}



void main_menu_function(int option) {
	if (option == 999) {
		printf("youe selected Quit menu\n");
		exit(0);
	}
	if (option == 1) {
		glClear(GL_COLOR_BUFFER_BIT);
		//���� �浵 ������
		theta = 45, phi = 45;
		radius = 20;
		up[1] = 1;
		glutPostRedisplay();
	}
	
}

