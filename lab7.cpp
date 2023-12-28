#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void draw_axis();
void resize(int, int);
void idle();
void main_menu_function(int);


float sunAngle = 0; // 태양 자전
float venus_rotAngle = 0; //금성 자전
float venus_revAngle = 0; //금성 공전
float earth_rotAngle = 0; // 지구 자전
float earth_revAngle = 0; // 지구 공전
float moon_rotAngle = 0; // 달 자전
float moon_revAngle = 0; // 달 공전
float mars_rotAngle = 0; // 화성 자전
float mars_revAngle = 0; // 화성 공전

//회전 상태, 1이면 회전함
bool spin_state = 1;

int main(int argc, char** argv) {
	/* Window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12201869 김수민");
	init(); // 사용자 초기화 함수

	/* Callback 함수 정의 */
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	/* Popup menu 생성 및 추가 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Spin On/Off", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}

/* 초기화 함수 */
void init() {
	/* 화면의 기본색으로 검정색 설정 */
	glClearColor(0.f, 0.f, 0.f, 0.f);

	printf("init 함수 호출\n");
}

// 창 크기 변경 콜백 함수
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	printf("resize 함수 호출\n");
}


void idle(void) {
	if (spin_state) {
		/* 태양의 자전 각도 변화 */
		sunAngle = sunAngle + 0.02;
		if (sunAngle > 360)
			sunAngle -= 360;

		/*금성의 공전, 자전 각도 변화*/
		venus_rotAngle = venus_rotAngle + 0.2; 
		if (venus_rotAngle > 360)
			venus_rotAngle -= 360;
		venus_revAngle = venus_revAngle + 5;
		if (venus_revAngle > 360)
			venus_revAngle -= 360;

		/* 지구의 공전, 자전 각도 변화 */
		earth_rotAngle = earth_rotAngle + 0.5;
		if (earth_rotAngle > 360)
			earth_rotAngle -= 360;
		earth_revAngle = earth_revAngle + 1;
		if (earth_revAngle > 360)
			earth_revAngle -= 360;

		/* 달의 공전, 자전  각도 변화 */
		moon_rotAngle = moon_rotAngle + 0.2;
		if (moon_rotAngle > 360)
			moon_rotAngle -= 360;
		moon_revAngle = moon_revAngle + 0.5;
		if (moon_revAngle > 360)
			moon_revAngle -= 360;

		/* 화성의 자전, 공전 각도 변화 */
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
	glLineWidth(2); // 좌표축의 두께
	glBegin(GL_LINES);

	glColor3f(1, 0, 0); // x축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0); // y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4); // z축은 blue

	glEnd();
	glLineWidth(1); // 두께 다시 환원
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	draw_axis(); // World 좌표계 그리기

	glRotatef(sunAngle, 0, 1, 0); // 태양 자전
	glColor3f(1, 0, 0);
	glutWireSphere(3, 50, 50); // 태양 그리기

	glPushMatrix(); // 현재의 행렬 stack에 저장

	glRotatef(venus_revAngle, 0, 1, 0); // 금성 공전
	glTranslatef(2, 0, 2);
	glRotatef(venus_rotAngle, 0, 1, 0); // 금성 자전
	glColor3f(0, 0, 1);
	glutWireSphere(0.7, 50, 50); // 금성 그리기
	
	glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀
	glPushMatrix(); // 현재의 행렬 stack에 저장

	glRotatef(earth_revAngle, 0, 1, 0); // 지구 공전
	glTranslatef(4, 0, 3);
	glRotatef(earth_rotAngle, 0, 1, 0); // 지구 자전
	glColor3f(0, 1, 1);
	glutWireSphere(1, 50, 50); // 지구 그리기

	glRotatef(moon_revAngle, 0, 1, 0); // 달 공전
	glTranslatef(1.2, 0, 1.2);
	glRotatef(moon_rotAngle, 0, 1, 0); // 달 자전
	glColor3f(0,0.5, 1);
	glutWireSphere(0.3, 50, 50); // 달 그리기

	glPopMatrix(); // 태양만 그렸을 때의 상태로 행렬 복귀

	glRotatef(mars_revAngle, 0, 1, 0); // 화성 공전
	glTranslatef(8, 0, 0);
	glRotatef(mars_rotAngle, 0, 1, 0); // 화성 자전
	glColor3f(0, 1, 0);
	glutWireSphere(1.5, 50, 50); // 화성 그리기

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