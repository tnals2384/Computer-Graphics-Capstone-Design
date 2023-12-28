#include <gl/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>
void init(void);
void idle(void);
void draw(void);
void main_menu_function(int option);
void resize(int width, int height);
void draw_axis(void);
void mouseWheel(int button, int dir, int x, int y);
void special_keyboard(int ky, int x, int y);
double radian(int);

double m_RotateAngle = 0;

double radius = 20;
double theta = 45, phi = 45;
double cam[3];
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("����� Lab8");
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
	glutIdleFunc(idle);

	/* Looping ���� */
	glutMainLoop();

	return 0;
}
void init(void)
{
	glClearColor(0.f, 0.f, 0.f, 0.f); // ȭ���� �⺻�� -> ������
	glClearDepth(1.0f); // Buffer clearing�� �̿��� ��

	//step 3-1 
	//������ ���� wire�� ǥ��
	//glPolygonMode(GL_BACK, GL_LINE);

	//step 3-2
	//������ ���� culling
	glEnable(GL_CULL_FACE);

	// Light ����
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* Depth - Test Enable */
	glFrontFace(GL_CW); // CW�� ���� - Default�� CCW
	glEnable(GL_DEPTH_TEST); // Depth buffer ��Ȱ��ȭ

}

// â ũ�� ���� �ݹ� �Լ�
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	// resize() �Լ����� viewport�� projection T.F�� �̹� ����
	glColor3f(1, 1, 0);

	glPushMatrix();
	glRotatef(m_RotateAngle, 0, 1, 0);
	GLdouble eq[4] = { 1.0, 0.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eq);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();

	glutSolidTeapot(4);
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
			theta = 5;
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


void idle() {
	m_RotateAngle += 0.05;
	if (m_RotateAngle > 360)
		m_RotateAngle -= 360;
	glutPostRedisplay();
}