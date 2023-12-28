#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void resize(int, int);
void idle();
void main_menu_function(int);
void keyboard(unsigned char key, int x, int y);

float sunAngle = 0; // �¾� ����
float earth_rotAngle = 0; // ���� ����
float earth_revAngle = 0; // ���� ����
float moon_rotAngle = 0; // �� ����
float moon_revAngle = 0; // �� ����

//ȸ�� ����, 1�̸� ȸ����
bool spin_state = 1;
//Ű���� �Է¿� ���� �ٲ�� light_position
int light_position = 0;


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
	glutKeyboardFunc(keyboard);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);

	//depth ����
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//0�� ���� ���� ����
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//GLfloat shininess = 128.0f; // shininess �� ����
	//glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
	//glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	//Global ambient Light
	//GLfloat lmodel_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	

	//Local or infinite Viewpoint 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	//cutoff ���� 
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	

	//���� �Ӽ� ����
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);
	//

	// ���� ����ġ�� 0�� ���� ����ġ �ѱ�
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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

		/* ������ ����, ���� ���� ��ȭ */
		earth_rotAngle = earth_rotAngle + 0.05;
		if (earth_rotAngle > 360)
			earth_rotAngle -= 360;
		earth_revAngle = earth_revAngle + 0.1;
		if (earth_revAngle > 360)
			earth_revAngle -= 360;

		/* ���� ����, ����  ���� ��ȭ */
		moon_rotAngle = moon_rotAngle + 0.02;
		if (moon_rotAngle > 360)
			moon_rotAngle -= 360;
		moon_revAngle = moon_revAngle + 0.05;
		if (moon_revAngle > 360)
			moon_revAngle -= 360;

	}
	glutPostRedisplay();
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//ī�޶� ��ġ�� ���� ����
	if (light_position == 0) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);

	//�¾� ��ġ�� ���� ����
	if (light_position == 1) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	glRotatef(sunAngle, 0, 1, 0); // �¾� ����
	glColor3f(1, 0, 0);
	// �¾� �׸���
	glutSolidSphere(3, 50, 50);

	glPushMatrix(); // ������ ��� stack�� ����

	//����
	glRotatef(earth_revAngle, 0, 1, 0); // ���� ����
	glTranslatef(7, 0, 0);
	glRotatef(earth_rotAngle, 0, 1, 0); // ���� ����

	
	//���� ��ġ�� ���� ����
	if (light_position == 2) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	//���� �׸���
	glColor3f(0, 0, 1);
	glutSolidSphere(1.3, 50, 50);

	//��
	glRotatef(moon_revAngle, 0, 1, 0); // �� ����
	glTranslatef(2, 0, 0);
	glRotatef(moon_rotAngle, 0, 1, 0); // �� ����

	//�� ��ġ�� ���� ����
	if (light_position == 3) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	
	// �� �׸���
	glColor3f(0, 0.5, 1);
	glutSolidSphere(0.3, 30, 30);

	
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == '0') {
		printf("camera position\n");
		light_position = 0;
	}
	else if (key == '1') {
		printf("Sun position\n");
		light_position = 1;
	}
	else if (key == '2') {
		printf("earth position\n");
		light_position = 2;
	}
	else if (key == '3') {
		printf("moon position\n");
		light_position = 3;
	}
	glutPostRedisplay();
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