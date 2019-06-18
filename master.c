#include "avlTree.h"
#include <time.h>
#include <math.h>
#include <string.h>
#include <GL/GL.H>
#include <GL/glut.h>

void display();

int result = 0, page = 0;                       /* Store the current number to be inputted*/
int duplicate = 0;
char rotation[50];

///Insert a node into the Tree
struct node* insert(struct node* node, int key) {
	duplicate = 0;
	strcpy(rotation, " ");
	/*1.Perform Normal BST Rotation*/
	if (node == NULL) {
		return(createNode(key));
	}
	
	Sleep(500);

	if (key < node->key) {
		node->left = insert(node->left, key);
	}
	else if (key > node->key){
		node->right = insert(node->right, key);
	}
	else {
		duplicate = 1;
	}

	/* 2. Update height of this ancestor node */
	node->height = max(height(node->left), height(node->right)) + 1;


	/* 3. Get the balance factor of this ancestor node to check whether
	   this node became unbalanced */
	int bal = getBalance(node);

	/* Left Left Rotation case */
	if (bal > 1 && key <= node->left->key) {
		strcpy(rotation, " LEFT LEFT ROTATION ");
		display();
		Sleep(500);
		return rightRotate(node);
	}

	/* Right Right Rotation case */
	if (bal < -1 && key >= node->right->key) {
		strcpy(rotation, " RIGHT RIGHT ROTATION ");
		display();
		Sleep(500);
		return leftRotate(node);
	}


	/* Left Right Rotation case */
	if (bal > 1 && key >= node->left->key) {
		strcpy(rotation, " LEFT RIGHT ROTATION ");
		node->left = leftRotate(node->left);
		display();
		Sleep(500);
		return rightRotate(node);
	}

	/* Right Left Rotation case */
	if (bal < -1 && key <= node->right->key) {
		strcpy(rotation, " RIGHT LEFT ROTATION ");
		node->right = rightRotate(node->right);
		display();
		Sleep(500);
		return leftRotate(node);
	}

	display();      /* Display Tree after normal BST Insertion */

	Sleep(500);

	/* return the (unchanged) node pointer */
	return node;
}

void drawCircle(float segments, float radius, float sx, float sy)
{
	float theta = 2 * 3.1415926 / segments;
	float tan_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = radius;
	float y = 0;

	int cache_pt = 0;
	double cache_x;
	double cache_y;

	glBegin(GL_POLYGON);
	for (int ii = 0; ii < segments; ii++) {
		if (!cache_pt) {
			cache_x = x + sx;
			cache_y = y + sy;
			cache_pt = 1;
		}
		else {
			glVertex2f(cache_x, cache_y);
			//glVertex2f(x + sx, y + sy);
			cache_x = x + sx;
			cache_y = y + sy;
		}
		float tx = -y;
		float ty = x;
		x += tx * tan_factor;
		y += ty * tan_factor;
		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

/* Function to draw a line to connect nodes */
void draw_line(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

/* Function to display the text */
void draw_text(char* text, float x, float y)
{
	int i;
	printf("\nWriting Text @ x : %f, y = %f\n", x, y);
	glRasterPos3f(x-0.5, y-0.5, 1);
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
}

/* A recursive function to draw a binary tree */
void drawNode(struct node* t_root, float x1, float y1, int level)
{
	char buff[5];

	if (t_root == NULL)
		return;

	float segments = 25;
	float radius = 1.5;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level * 2.5;
	float angle_change = 20;

	/* Draw the current node */
	if (t_root == CURRENT)
		glColor3f(0.0, 1.0, 0.0);           /* If the node to be drawn is the currently inputted node
								Set color of node to green until its placed in its position */
	else
		glColor3f(1.0, 0.0, 0.0);     /* else set color of node to red */

	glPushMatrix();
	drawCircle(segments, radius, x1, y1);
	glPopMatrix();

	sprintf(buff, "%d", t_root->key); //atoi
	glColor3f(1.0, 1.0, 1.0);
	draw_text(buff, (x1), (y1));
	/* Display value of the node*/

	if (t_root->left)    /*Checks if parent has a left child node to be drawn*/
	{
		/* Draw the left child node */
		float angle = left_angle - level * angle_change;
		double radian = angle * 3.14 / 180;
		//float m = (double)tan((double)radian);
		float x2 = x1 + branch_length * sin((double)radian);
		float y2 = y1 + branch_length * cos((double)radian);
		drawNode(t_root->left, x2, y2, level + 1);
		glColor3f(0.0, 0.0, 0.0);
		draw_line(x1, y1, x2, y2);
		sprintf(buff, "%d", t_root->left->key); //atoi
		glColor3f(1.0, 1.0, 1.0);
		draw_text(buff, (x2), (y2));
	}
	if (t_root->right) /*Checks if parent has a Right child node to be drawn*/
	{
		/* Draw the Right child node */
		float angle = right_angle + level * angle_change;
		float radian = angle * 3.14 / 180;
		//float m = (double)tan((double)radian);
		float x2 = x1 + branch_length * sin((double)radian);
		float y2 = y1 + branch_length * cos((double)radian);
		drawNode(t_root->right, x2, y2, level + 1);
		glColor3f(0.0, 0.0, 0.0);
		draw_line(x1, y1, x2, y2);
		sprintf(buff, "%d", t_root->right->key); //atoi
		glColor3f(1.0, 1.0, 1.0);
		draw_text(buff, (x2), (y2));
	}
}

void printAbout()
{
	int i;
	char buffer[50] = "What is an AVL tree?.\0";
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-11, -0.5, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
	strcpy(buffer, "1. AVL tree is a self-balancing binary search tree\0");
	glRasterPos3f(-11, -5.5, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);


	strcpy(buffer, "2. The heights of the two child subtrees of\0");
	glRasterPos3f(-11, -9, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);

	strcpy(buffer, " any node differ by at most one\0");
	glRasterPos3f(-11, -11, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);

	strcpy(buffer, "3. If at any time they differ by more than one,\0");
	glRasterPos3f(-11, -15, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);

	strcpy(buffer, "     rebalancing is done to the tree\0");
	glRasterPos3f(-11, -17, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
}

void printCredits()
{
	int i;
	char buffer[50] = "Project By ";
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(-5, -2.5, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);

	strcpy(buffer, "SRIGOVIND NAYAK");
	glRasterPos3f(-5, -5.5, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);


	strcpy(buffer, "Semester VI");
	glRasterPos3f(-5, -9, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);

	strcpy(buffer, "USN 1BG16CS104");
	glRasterPos3f(-5, -12, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
}

void drawFirstPage()
{
	int i;
	char buffer[50] = "AVL TREE CONSTRUCTION ";
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos3f(-11, 2, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

	glColor3f(1.0, 0.0, 0.0);
	strcpy(buffer, "1. START");
	glRasterPos3f(-5, -2, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

	glColor3f(1.0, 0.0, 1.0);
	strcpy(buffer, "2. ABOUT");
	glRasterPos3f(-5, -8, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

	glColor3f(0.0, 1.0, 1.0);
	strcpy(buffer, "3. CREDITS");
	glRasterPos3f(-5, -14, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

	glColor3f(1.0, 0.0, 0.0);
	strcpy(buffer, "4. EXIT");
	glRasterPos3f(-5, -20, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

	glColor3f(0.0, 1.0, 1.0);
	strcpy(buffer, "by SRIGOVIND NAYAK 1BG16CS104");
	glRasterPos3f(-10, -24, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
}

void showInsert() {
	char buffer[50];
	int i;
	strcpy(buffer, "INSERT BUFFER : ");
	glRasterPos3f(8, -25, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

	sprintf(buffer, "%d", result);
	glRasterPos3f(18, -25, 1.5);
	for (i = 0; buffer[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
}

void display()
{
	if (page == 0)
	{
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, 10, -30);
		glColor3f(0.0, 0.0, 0.0);

		drawFirstPage();
		glutSwapBuffers();
	}
	if (page == 1)
	{
		int i;
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, 10, -30);
		glColor3f(1, 1, 1);

		char buffer[50] = "AVL TREE CONSTRUCTION";
		glColor3f(0.0, 0.0, 0.0);

		glRasterPos3f(-10, 3, 1.5);
		for (i = 0; buffer[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

		strcpy(buffer, "Enter value and press I on the keyboard");
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos3f(-10, 2, 1.5);
		for (i = 0; buffer[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, buffer[i]);

		if (duplicate == 1) {
			strcpy(buffer, "Duplicate Element added.");
			glRasterPos3f(-5, -25, 1.5);
			for (i = 0; buffer[i] != '\0'; i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);
		}
		
		strcpy(buffer, rotation);
		glRasterPos3f(-15, -25, 1.5);
		for (i = 0; buffer[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buffer[i]);

		showInsert();

		drawNode(ROOT, 0, 0, 0);

		glutSwapBuffers();
	}
	if (page == 2)
	{
		glClearColor(1, 1, 1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, 10, -30);
		glColor3f(1, 1, 1);

		printAbout();
		glutSwapBuffers();
	}

	if (page == 3)
	{
		glClearColor(1, 1, 1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, 10, -30);
		glColor3f(1, 1, 1);

		printCredits();
		glutSwapBuffers();
	}

}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void rightClickMenu(int ch) {
	if (page == 1) {
		switch (ch) {
		case 1:
			ROOT = NULL;
			duplicate = 0;
			strcpy(rotation, "");
			result = 0;
			display();
			break;
		case 2:
			exit(0);
			break;
		}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	char b[5];
	if (page == 0)
	{
		switch (key)
		{
		case '1':
			page = 1;
			display();
			break;

		case '2':
			page = 2;
			display();
			break;

		case '3':
			page = 3;
			display();
			break;

		case '4':
			exit(0);
		}
	}
	else if (page == 1)
	{
		switch (key)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			result = result * 10 + (key - '0');
			display();
			break;
		case 'i':
			ROOT = insert(ROOT, result);
			display();
			result = 0;
			break;
		case 'x':
			exit(0);
		}
	}
	else if (page == 2)
	{
		page = 0;
		display();
	}
	else if (page == 3)
	{
		page = 0;
		display();
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("AVL tree : A self balancing B-Tree");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutCreateMenu(rightClickMenu);
	glutAddMenuEntry("RESET", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}