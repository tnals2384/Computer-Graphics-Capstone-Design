#include <gl/glut.h>
#include <stdio.h>

void init(void);
void draw(void);
void draw_blending();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void draw_string(void* font, const char* str, int x, int y);
void main_menu_function(int option);
void sub_menu_function(int option);

//antialiase, blend �Ѱ� ����
bool antialiase_on = FALSE;
bool blend_on = FALSE;

//x,y ��ǥ
GLint point_x[150];
GLint point_y[150];

int draw_mode = 0; 
int draw_type[50] = { 0 };
int num = 0;
int num_p = 0;
int a = 0;


int main(int argc, char **argv) {
	/* Window �ʱ�ȭ */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("����� Lab4");
	init(); // ����� �ʱ�ȭ �Լ�

	/* Popup menu ���� �� �߰� */
	/*����޴�*/
	int submenu = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);

	/* ���� �޴� */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Clear", 4);
	glutAddSubMenu("draw_mode", submenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // ������ ���콺 ������ �� �޴� �߰��ϱ�

	/* Callback �Լ� ���� */
	glutDisplayFunc(draw); // ���� �׸��� �Լ�
	glutMouseFunc(mouse); // ���콺 �Է�
	glutKeyboardFunc(keyboard); // Ű���� �Է� 
	/* Looping ���� */
	glutMainLoop();

	return 0;
}
void init(void)
{
	/* ȭ���� �⺻������ Blue ���� */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	/* ȭ�� ��ǥ ���� ���� */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

/* string �׸��� */
void draw_string(void* font, const char* str, int x, int y) {
	unsigned int i;
	//���� ��� ��ġ ����
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++) {
		//glut�� ������ bitmap font�̿�, ������ ���� �ѹ��ھ� ���
		glutBitmapCharacter(font, str[i]);
	}
}

void draw(void) {
	/* ȭ���� �����ϰ� ����� */
	glClear(GL_COLOR_BUFFER_BIT);
	//draw_blending();
	//glColor3f(1.0f, 1.0f, 0);
	//draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", 10, 10);

	//glColor3f(0.5f, 1.0f, 0);
	//draw_string(GLUT_BITMAP_HELVETICA_18, "I Like Computer Graphics!!", 250, 475);

	if (antialiase_on) { //antialisase�� ������ Ȱ��ȭ
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	}
	else { //��Ȱ��ȭ
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
	
	if (blend_on) 
		//Ȱ��ȭ
		glEnable(GL_BLEND);
	//��Ȱ��ȭ
	else glDisable(GL_BLEND);

	int i = 0, i_p = 0;
	//sub menu���� ������ draw mode �� ���� ����
	while (draw_type[i] != 0) {
		//�����
		if (draw_type[i] == 1) {
			glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			glVertex2i(point_x[i_p], point_y[i_p]);
			glEnd();
			i_p++;
		}
		//���׸���
		else if (draw_type[i] == 2) {
			glLineWidth(3.0f);
			glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
			glBegin(GL_LINES); 
			glVertex2i(point_x[i_p], point_y[i_p]);
			glVertex2i(point_x[i_p+1], point_y[i_p+1]);
			glEnd();
			i_p += 2;
		}
		//�ﰢ�� �׸���
		else if (draw_type[i] == 3) {
			glColor4f(0.0f, 0.8f, 0.8f, 0.5f);
			glBegin(GL_TRIANGLES);
			glVertex2i(point_x[i_p], point_y[i_p]);
			glVertex2i(point_x[i_p + 1], point_y[i_p + 1]);
			glVertex2i(point_x[i_p + 2], point_y[i_p + 2]);
			glEnd();
			i_p += 3;
		}
		i++;
	 }
	glFlush();
}



void mouse(int button, int state, int x, int y) {
	//���콺 ���� ��ư�� ������
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			printf("Mouse button is clicked! (%d, %d)\n", x, 500 - y);
			draw_type[num] = draw_mode;

			point_x[num_p] = x;
			point_y[num_p] = 500 - y;
			num_p++;
			a++;
		}
	}

	if (a >= draw_mode) {
		num++;
		a = 0;
		glutPostRedisplay();
	}
}


void draw_blending() {

	//blend Ȱ��ȭ
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//yellow
	glColor4f(1.0, 1.0, 0.0, 0.75);
	glBegin(GL_TRIANGLES);
	glVertex2i(50, 50);
	glVertex2i(50, 450);
	glVertex2i(350, 250);
	glEnd();
	//cian
	glColor4f(0.0, 1.0, 1.0, 0.75);
	glBegin(GL_TRIANGLES);
	glVertex2i(450, 450);
	glVertex2i(450, 50);
	glVertex2i(150, 250);
	glEnd();
}

/* Ű���� �Է� ó�� */
void keyboard(unsigned char key, int x, int y) { 
	printf("You pressed %C, ", key);
	//a ������ antialiase_on
	if (key == 'a') {
		if (!antialiase_on) {
			printf("antialiase on\n");
			antialiase_on = TRUE;
		}
		else {
			printf("antialiase off\n");
			antialiase_on = FALSE;
		}
	}

	//b������ blend on
	if (key == 'b') {
		if (!blend_on) {
			printf("blend on\n");
			blend_on = TRUE;
		}
		else {
			printf("blend off\n");
			blend_on = FALSE;
		}
	}

	glutPostRedisplay();
}


void sub_menu_function(int option) {
	printf("Sub menu %d has been selected\n", option);
	draw_mode = option;
}


void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);

	//quit ������ ����
	if (option == 999) {
		exit(0);
	}
	//clean ������ ȭ�� clean
	else if (option == 4) {
		int i = 0;
		if (draw_type[i] != 0) {
			draw_type[i] = 0;
			i++;
		}
		i = 0;
		while (point_x[i] != 0) {
			point_x[i] = 0;
			point_y[i] = 0;
			i++;
		 }
		num = 0;
		num_p = 0;
		glutPostRedisplay();
	}
}

