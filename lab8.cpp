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

// 각도를 라디안으로 변환
double radian(int value) {
	return (value * (pi / 180));
}

int main(int argc, char **argv) {
	/*openGL 좌표계에 맞춰 cam 위치 설정*/
	cam[0] = radius * sin(radian(theta)) * sin(radian(phi));
	cam[1] = radius * cos(radian(theta));
	cam[2] = radius * sin(radian(theta)) * cos(radian(phi));
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("김수민 Lab8");
	init(); // 사용자 초기화 함수

	/* 메인 메뉴 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // 오른쪽 마우스 눌렀을 때 메뉴 뜨게하기

	/* Callback 함수 정의 */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw); // 실제 그리기 함수
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(special_keyboard); // 키보드 입력 
	glutIdleFunc(idle);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}
void init(void)
{
	glClearColor(0.f, 0.f, 0.f, 0.f); // 화면의 기본색 -> 검정색
	glClearDepth(1.0f); // Buffer clearing시 이용할 값

	//step 3-1 
	//주전자 내부 wire로 표현
	//glPolygonMode(GL_BACK, GL_LINE);

	//step 3-2
	//주전자 내부 culling
	glEnable(GL_CULL_FACE);

	// Light 설정
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
	glFrontFace(GL_CW); // CW로 정의 - Default는 CCW
	glEnable(GL_DEPTH_TEST); // Depth buffer 비활성화

}

// 창 크기 변경 콜백 함수
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
	glLineWidth(3); // 좌표축의 두께
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); // X축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0); // Y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1); // Z축은 blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1); // 두께 다시 환원
}


void draw(void) {
	/* 화면을 깨끗하게 지우기 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	// resize() 함수에서 viewport와 projection T.F는 이미 수행
	glColor3f(1, 1, 0);

	glPushMatrix();
	glRotatef(m_RotateAngle, 0, 1, 0);
	GLdouble eq[4] = { 1.0, 0.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eq);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();

	glutSolidTeapot(4);
	glutSwapBuffers(); // double buffering시

	glFlush();
}


// 마우스 휠 이벤트를 처리하여 확대 및 축소
void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		radius -= 1.0;
		if (radius < 1.0) {
			radius = 1.0; // 최소 확대 수준 설정
		}
	}
	else {
		radius += 1.0;
		if (radius > 50.0) {
			radius = 50.0; // 최대 축소 수준 설정
		}
	}
	// 새로운 카메라 위치 계산
	cam[0] = radius * sin(radian(theta)) * sin(radian(phi));
	cam[1] = radius * cos(radian(theta));
	cam[2] = radius * sin(radian(theta)) * cos(radian(phi));
	glutPostRedisplay();
}

//방향키로 viewing transform
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
	// 새로운 카메라 위치 계산
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
		//위도 경도 원래로
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