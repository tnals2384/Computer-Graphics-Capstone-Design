#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void draw_axis();
void resize(int, int);
void idle();
void main_menu_function(int);


float sunAngle = 0; // �¾� ����
float venus_rotAngle = 0; //�ݼ� ����
float venus_revAngle = 0; //�ݼ� ����
float earth_rotAngle = 0; // ���� ����
float earth_revAngle = 0; // ���� ����
float moon_rotAngle = 0; // �� ����
float moon_revAngle = 0; // �� ����
float mars_rotAngle = 0; // ȭ�� ����
float mars_revAngle = 0; // ȭ�� ����

//ȸ�� ����, 1�̸� ȸ����
bool spin_state = 1;

int main(int argc, char** argv) {
	/* Window �ʱ�ȭ */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12201869 �����");
	init(); // ����� �ʱ�ȭ �Լ�

	/* Callback �Լ� ���� */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	/* Popup menu ���� �� �߰� */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Spin On/Off", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Looping ���� */
	glutMainLoop();

	return 0;
}

/* �ʱ�ȭ �Լ� */
void init() {
	/* ȭ���� �⺻������ ������ ���� */
	glClearColor(0.f, 0.f, 0.f, 0.f);

	printf("init �Լ� ȣ��\n");
}

// â ũ�� ���� �ݹ� �Լ�
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	printf("resize �Լ� ȣ��\n");
}


void idle(void) {
	if (spin_state) {
		/* �¾��� ���� ���� ��ȭ */
		sunAngle = sunAngle + 0.02;
		if (sunAngle > 360)
			sunAngle -= 360;

		/*�ݼ��� ����, ���� ���� ��ȭ*/
		venus_rotAngle = venus_rotAngle + 0.2; 
		if (venus_rotAngle > 360)
			venus_rotAngle -= 360;
		venus_revAngle = venus_revAngle + 5;
		if (venus_revAngle > 360)
			venus_revAngle -= 360;

		/* ������ ����, ���� ���� ��ȭ */
		earth_rotAngle = earth_rotAngle + 0.5;
		if (earth_rotAngle > 360)
			earth_rotAngle -= 360;
		earth_revAngle = earth_revAngle + 1;
		if (earth_revAngle > 360)
			earth_revAngle -= 360;

		/* ���� ����, ����  ���� ��ȭ */
		moon_rotAngle = moon_rotAngle + 0.2;
		if (moon_rotAngle > 360)
			moon_rotAngle -= 360;
		moon_revAngle = moon_revAngle + 0.5;
		if (moon_revAngle > 360)
			moon_revAngle -= 360;

		/* ȭ���� ����, ���� ���� ��ȭ */
		mars_rotAngle = mars_rotAngle + 0.8;
		if (mars_rotAngle > 360)
			mars_rotAngle -= 360;
		mars_revAngle = mars_revAngle + 0.5;
		if (mars_revAngle > 360)
			mars_revAngle -= 360;
	}
	glutPostRedisplay();
}

void draw_axis() {
	glLineWidth(2); // ��ǥ���� �β�
	glBegin(GL_LINES);

	glColor3f(1, 0, 0); // x���� red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0); // y���� green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4); // z���� blue

	glEnd();
	glLineWidth(1); // �β� �ٽ� ȯ��
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	draw_axis(); // World ��ǥ�� �׸���

	glRotatef(sunAngle, 0, 1, 0); // �¾� ����
	glColor3f(1, 0, 0);
	glutWireSphere(3, 50, 50); // �¾� �׸���

	glPushMatrix(); // ������ ��� stack�� ����

	glRotatef(venus_revAngle, 0, 1, 0); // �ݼ� ����
	glTranslatef(2, 0, 2);
	glRotatef(venus_rotAngle, 0, 1, 0); // �ݼ� ����
	glColor3f(0, 0, 1);
	glutWireSphere(0.7, 50, 50); // �ݼ� �׸���
	
	glPopMatrix(); // �¾縸 �׷��� ���� ���·� ��� ����
	glPushMatrix(); // ������ ��� stack�� ����

	glRotatef(earth_revAngle, 0, 1, 0); // ���� ����
	glTranslatef(4, 0, 3);
	glRotatef(earth_rotAngle, 0, 1, 0); // ���� ����
	glColor3f(0, 1, 1);
	glutWireSphere(1, 50, 50); // ���� �׸���

	glRotatef(moon_revAngle, 0, 1, 0); // �� ����
	glTranslatef(1.2, 0, 1.2);
	glRotatef(moon_rotAngle, 0, 1, 0); // �� ����
	glColor3f(0,0.5, 1);
	glutWireSphere(0.3, 50, 50); // �� �׸���

	glPopMatrix(); // �¾縸 �׷��� ���� ���·� ��� ����

	glRotatef(mars_revAngle, 0, 1, 0); // ȭ�� ����
	glTranslatef(8, 0, 0);
	glRotatef(mars_rotAngle, 0, 1, 0); // ȭ�� ����
	glColor3f(0, 1, 0);
	glutWireSphere(1.5, 50, 50); // ȭ�� �׸���

	glFlush();
	glutSwapBuffers();
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);

	if (option == 999) {
		exit(0);
	}
	else if (option == 1) {
		glClear(GL_COLOR_BUFFER_BIT);
		spin_state = !spin_state;
	}
}