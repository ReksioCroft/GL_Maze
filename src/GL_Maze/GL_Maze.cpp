// Functii de desenare in Open GL. Randare instantiata 
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"
#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;

#define INSTANCE_COUNT 40
const GLfloat PI = 3.141592f;

// identificatori 
GLuint
VaoId,
VBPos,
VBCol,
VBModelMat,
EboId,
ColorBufferId,
ProgramId,
viewLocation,
projLocation,
codColLocation,
lightColorLoc,
lightPosLoc,
viewPosLoc,
matrUmbraLocation,
codCol;

// variabile pentru matricea de vizualizare
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float Alpha = 0.0f, Beta = 0.0f, dist = 200.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0f, Vy = 0.0f, Vz = -1.0f;
float incr_alpha1 = 0.01f, incr_alpha2 = 0.01f;

// variabile pentru matricea de proiectie
float width = 800, height = 600, znear = 1, fov = 30;

// vectori
glm::vec3 Obs, PctRef, Vert;
glm::vec4 Colors[INSTANCE_COUNT];

// matrice utilizate
glm::mat4 view, projection, MatModel[INSTANCE_COUNT], matrUmbra;

// sursa de lumina
float xL = 500.f, yL = 0.f, zL = 400.f;



void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		dist += 50.0;
		break;
	case '+':
		dist -= 50.0;
		break;
	default:
		break;
	}
}


void processSpecialKeys(int key, int xx, int yy) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Beta -= 0.01f;
		break;
	case GLUT_KEY_RIGHT:
		Beta += 0.01f;
		break;
	case GLUT_KEY_UP:
		Alpha += incr_alpha1;
		if (abs(Alpha - PI / 2) < 0.05)
		{
			incr_alpha1 = 0.f;
		}
		else
		{
			incr_alpha1 = 0.01f;
		}
		break;
	case GLUT_KEY_DOWN:
		Alpha -= incr_alpha2;
		if (abs(Alpha + PI / 2) < 0.05)
		{
			incr_alpha2 = 0.f;
		}
		else
		{
			incr_alpha2 = 0.01f;
		}
		break;
	}
}


void CreateVBO(void)
{
	// Varfurile 
	GLfloat Vertices[] =
	{
		//baza pirmadida
		-50.0f, -50.0f, 50.0f, 1.0f,
		50.0f,  -50.0f,50.0f, 1.0f,
		-50.0f, 50.0f,  50.0f,1.0f,
		50.0f, 50.0f, 50.0f, 1.0f,
		//varf
		0.0f,  0.0f,-150.f, 1.0f,

		// podeaua din sticla
		// varfurile din planul z=-50  
		// coordonate
		-2000.0f,  -2000.0f, 80.0f, 1.0f, 
		2000.0f,  -2000.0f, 80.0f, 1.0f,  
		2000.0f,  2000.0f,  80.0f, 1.0f,   
		-2000.0f,  2000.0f, 80.0f, 1.0f,  
		// varfurile din planul z=+50  
		// coordonate
		-2000.0f,  -2000.0f, 50.0f, 1.0f,
		2000.0f,  -2000.0f,  50.0f, 1.0f,
		2000.0f,  2000.0f,  50.0f, 1.0f,   
		-2000.0f,  2000.0f, 50.0f, 1.0f, 
	};

	// Culorile instantelor
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		Colors[n][0] = 0.35f + 0.30f * (sinf(a + 2.0f) + 1.0f);
		Colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
		Colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
		Colors[n][3] = 1.0f;
	}

	// Matricele instantelor
	//int fib1=0, fib2=1;
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		//if (n < 3)
			//MatModel[n] = glm::mat4(0);
		//else 
		//int fib = fib1 + fib2;
		//fib1 = fib2;
		//fib2 = fib;

			//MatModel[n] = glm::translate(glm::mat4(1.0f), glm::vec3(fib * sin( n * 180 / PI), fib * cos( n * 180 / PI), 0.0)) ;
		MatModel[n] = glm::translate(glm::mat4(1.0f), glm::vec3(INSTANCE_COUNT * n * sin(n * 180 / PI), INSTANCE_COUNT * n * cos(n * 180 / PI), 0.0));

	}

	// indicii pentru varfuri
	GLubyte Indices[] =
	{
		//fetele
		0, 1, 4, 4, 2, 3,
		4, 1, 3, 4, 2, 0,
		0, 1, 2, 2, 1, 3,
		///muchiile
		0, 1, 3, 2, //muchie baza
		0, 4,  //muchie fata
		1, 4,  //muchie fata
		2, 4,  //muchie fata
		3, 4,  //muchie fata

		// podeaua de sticla
		6, 5, 7,   7, 5, 8,  //  Fata "de jos"
		7, 8, 11,   11, 8, 10,  // Lateral 
		12, 8, 9,   9, 8, 5,  // Lateral 
		9, 5, 10,   10, 5, 6,  // Lateral 
		6, 7, 10,   10, 7, 11,  // Lateral 
		10, 11, 9,   9, 11, 12, //  Fata "de sus"
		5, 6, 7, 8,  // Contur fata de jos
		9, 10, 11, 12,  // Contur fata de sus
		5, 9, // Muchie laterala
		6, 10, // Muchie laterala
		7, 11, // Muchie laterala
		8, 12 // Muchie laterala
	};

	// generare buffere
	glGenVertexArrays(1, &VaoId);
	glGenBuffers(1, &VBPos);
	glGenBuffers(1, &VBCol);
	glGenBuffers(1, &VBModelMat);
	glGenBuffers(1, &EboId);

	// legarea VAO 
	glBindVertexArray(VaoId);

	// 0: Pozitie
	glBindBuffer(GL_ARRAY_BUFFER, VBPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	// 1: Culoare
	glBindBuffer(GL_ARRAY_BUFFER, VBCol); // legare buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);  // rata cu care are loc distribuirea culorilor per instanta

	// 2..5 (2+i): Matrice de pozitie
	glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MatModel), MatModel, GL_STATIC_DRAW);
	for (int i = 0; i < 4; i++) // Pentru fiecare coloana
	{
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i,              // Location
			4, GL_FLOAT, GL_FALSE,                // vec4
			sizeof(glm::mat4),                    // Stride
			(void*)(sizeof(glm::vec4) * i));      // Start offset
		glVertexAttribDivisor(2 + i, 1);
	}

	// Indicii 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(3); // atributul 2 = normale
}


void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBPos);
	glDeleteBuffers(1, &VBCol);
	glDeleteBuffers(1, &VBModelMat);
	glDeleteBuffers(1, &EboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}


void CreateShaders(void)
{
	ProgramId = LoadShaders("GL_Maze_Shader.vert", "GL_Maze_Shader.frag");
	glUseProgram(ProgramId);
}


void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}


void Initialize(void)
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // culoarea de fond a ecranului
	CreateVBO();
	CreateShaders();
	viewLocation = glGetUniformLocation(ProgramId, "viewMatrix");
	projLocation = glGetUniformLocation(ProgramId, "projectionMatrix");
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	lightColorLoc = glGetUniformLocation(ProgramId, "lightColor");
	lightPosLoc = glGetUniformLocation(ProgramId, "lightPos");
	viewPosLoc = glGetUniformLocation(ProgramId, "viewPos");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
}


void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // decomentati acest rand daca este cazul 
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VBPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	//pozitia observatorului
	Obsx = Refx + dist * cos(Alpha) * cos(Beta);
	Obsy = Refy + dist * cos(Alpha) * sin(Beta);
	Obsz = Refz + dist * sin(Alpha);

	// reperul de vizualizare
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie
	projection = glm::infinitePerspective(fov * PI / 180, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);


	// Variabile uniforme pentru iluminare
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, xL, yL, zL);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);

	// matricea pentru umbra
	float D = -5.f;
	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);


	// Cubul
	// Fetele
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(30));
	// Muchiile
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glLineWidth(3.5);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(66));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(70));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(74));


	// Fetele
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0, INSTANCE_COUNT);
	// Muchiile
	/*codCol = 1;
	glUniform1i(codColLocation, codCol);
	glLineWidth(2.5);
	glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(18), INSTANCE_COUNT);
	glDrawElementsInstanced(GL_LINE_LOOP, 8, GL_UNSIGNED_BYTE, (void*)(22), INSTANCE_COUNT);*/


	glutSwapBuffers();
	glFlush();
}


void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("<<Instanced rendering>>");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
	return 0;
}