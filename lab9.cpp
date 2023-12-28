#include<stdio.h>
#include<GL/glut.h>

void init();
void draw();
void resize(int, int);
void idle();
void main_menu_function(int);
void keyboard(unsigned char key, int x, int y);

float sunAngle = 0; // 태양 자전
float earth_rotAngle = 0; // 지구 자전
float earth_revAngle = 0; // 지구 공전
float moon_rotAngle = 0; // 달 자전
float moon_revAngle = 0; // 달 공전

//회전 상태, 1이면 회전함
bool spin_state = 1;
//키보드 입력에 따라 바뀌는 light_position
int light_position = 0;


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
	glutKeyboardFunc(keyboard);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);

	//depth 설정
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//0번 조명 관련 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//GLfloat shininess = 128.0f; // shininess 값 설정
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

	//cutoff 구현 
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	

	//감쇄 속성 지정
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);
	//

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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

		/* 지구의 공전, 자전 각도 변화 */
		earth_rotAngle = earth_rotAngle + 0.05;
		if (earth_rotAngle > 360)
			earth_rotAngle -= 360;
		earth_revAngle = earth_revAngle + 0.1;
		if (earth_revAngle > 360)
			earth_revAngle -= 360;

		/* 달의 공전, 자전  각도 변화 */
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

	//카메라 위치에 조명 설정
	if (light_position == 0) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);

	//태양 위치에 조명 설정
	if (light_position == 1) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	glRotatef(sunAngle, 0, 1, 0); // 태양 자전
	glColor3f(1, 0, 0);
	// 태양 그리기
	glutSolidSphere(3, 50, 50);

	glPushMatrix(); // 현재의 행렬 stack에 저장

	//지구
	glRotatef(earth_revAngle, 0, 1, 0); // 지구 공전
	glTranslatef(7, 0, 0);
	glRotatef(earth_rotAngle, 0, 1, 0); // 지구 자전

	
	//지구 위치에 조명 설정
	if (light_position == 2) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	//지구 그리기
	glColor3f(0, 0, 1);
	glutSolidSphere(1.3, 50, 50);

	//달
	glRotatef(moon_revAngle, 0, 1, 0); // 달 공전
	glTranslatef(2, 0, 0);
	glRotatef(moon_rotAngle, 0, 1, 0); // 달 자전

	//달 위치에 조명 설정
	if (light_position == 3) {
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	
	// 달 그리기
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