#include <windows.h>
#include <gl/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>

void DrawBackground();
void DrawSphere();
void init(void);
void draw(void);
void resize(int width, int height);
void Picking(int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

float g_pPosition[6] = { 0.0f, 144.0f, -100.0f, -100.0f, 100.0f, -100.0f }; //������ ��Ʈ���� �ʱ�ȭ
int g_nX, g_nY;
int g_nSelect = 0; //���õ� ��ü ����

int g_nGLWidth = 500, g_nGLHeight = 500; //������ ũ��


void init(void) {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//�����ְ� ���� ���� ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
}


void DrawBackground()
{
	//��׶��� �׸���
	glPushAttrib(GL_ALL_ATTRIB_BITS); //���� ���� ����
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i;
	int offset = 50;
	glBegin(GL_LINES);
	for (i = -g_nGLWidth / 2.0f; i < g_nGLWidth / 2.0f; i += offset)
	{
		glVertex3f(i, -g_nGLHeight / 2.0f, 0.0f);
		glVertex3f(i, g_nGLHeight / 2.0f, 0.0f);
	}
	for (i = -g_nGLHeight / 2.0f; i < g_nGLHeight / 2.0f; i += offset)
	{
		glVertex3f(-g_nGLWidth / 2.0f, i, 0.0f);
		glVertex3f(g_nGLWidth / 2.0f, i, 0.0f);
	}
	glEnd();
	glPopAttrib(); //�������� ����
}


void DrawSphere()
{
	//depth���� �ٸ��� �Ͽ� ���׸���
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 1.0f);
	glColor3f(1, 0, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 2.0f);
	glColor3f(0, 1, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 3.0f);
	glColor3f(0, 0, 1);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawBackground();
	DrawSphere();

	glFlush();
	glutSwapBuffers();
}

void resize(int width, int height)
{
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f,-100, 100);
	glMatrixMode(GL_MODELVIEW);
}


void Picking(int x, int y) {
	// first step
	GLuint selectBuf[256];
	glSelectBuffer(256, selectBuf); // buffer�� GLuint ���� �迭�� picking�� ��ü�� ���� ���� ����

	// second step gl select mode����
	glRenderMode(GL_SELECT); // GL_SELECT�� ����Ͽ� ���� ��� ����

	// third step name stack �ʱ�ȭ
	glInitNames();
	//	// third step    glpushname (-1)
	glPushName(-1);

	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);  
	glPushMatrix(); // ���� ���� matrix stack�� ����
	glLoadIdentity(); 
	// fourth step    ��Ʈ���� �Լ� 
	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	//  viewport�� ���� ������ �׸��� projection matrix �����Ͽ� matrix stack����� ��

	// five step
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f,
		g_nGLHeight / 2.0f, -100, 100); 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	DrawSphere(); // ���ο��� glLoadName(1),(2),(3)�� ȣ���Ͽ� �� ����
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	// sixth step //gl render���� �ٲٱ�
	GLint hits = glRenderMode(GL_RENDER);
	if (hits <= 0) return;

	for (int i = 0; i < hits * 4 + 1; i++) {
		if (selectBuf[i] == 0) break;
		printf("%u ", selectBuf[i]);
	}

	// seven step  select buffer���� hit �� ��ü�� id�� depth������
	int stack = selectBuf[0];
	unsigned int zMin = selectBuf[1];
	unsigned int zMax = selectBuf[2];
	g_nSelect = selectBuf[3];
	printf("\ng_nSelect : %d\n", g_nSelect);
	int index = 3 + stack;
	int i;

	//zMax 
	//for (i = 1; i < hits; i++) {
	//	stack = selectBuf[index];
	//	if (zMax > selectBuf[index + 2]) {
	//		zMax = selectBuf[index + 2];
	//		g_nSelect = selectBuf[index + 3];
	//	}
	//	index += 3 + stack;
	//}

	//zMin
	for (i = 1; i < hits; i++) {
		stack = selectBuf[index];
		if (zMin < selectBuf[index + 1]) {
			zMin = selectBuf[index + 1];
			g_nSelect = selectBuf[index + 3];
		}
		index += 3 + stack;
	}
	printf("hits : %d\n", hits);

}

void mouse(int button, int state, int x, int y) {
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		g_nSelect = 0;
	}
}
void motion(int x, int y) {
	y = g_nGLHeight - y;
	if (g_nSelect > 0) {
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12201869 �����");
	init();
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}
