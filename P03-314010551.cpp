//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
const float escala = 1.001f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*> meshColorList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
//Otros Shaders


Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks
float angulo = 0.0f;



void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}



// Pirámide triangular de colores
void CrearPiramideTriangular()
{
    GLfloat vertices_piramide_colores[] = {
        // Cara roja
        -0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, -0.25f,  1.0f, 0.0f, 0.0f,

        // Cara verde
         0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,
         0.0f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, -0.25f,  0.0f, 1.0f, 0.0f,

        // Cara amarilla
         0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f,   1.0f, 1.0f, 0.0f,
         0.0f,  0.5f, -0.25f,  1.0f, 1.0f, 0.0f,

        // Cara cyan
        -0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 1.0f,
         0.0f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f
    };

    MeshColor* obj1 = new MeshColor(); //Usamos MeshColor porque no dibujamos con índices, sino con vértices
    obj1->CreateMeshColor(vertices_piramide_colores, 72); // 12 vértices * 6 floats = 72
    meshColorList.push_back(obj1); //Se guarda en la posición cero
}

void CrearPiramideTriangularBordes()
{
	GLfloat vertices_piramide_bordes[] = {
	
		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, -0.25f,  0.0f, 0.0f, 0.0f,

		0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		0.0f,  0.5f, -0.25f,  0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.0f,  0.5f, -0.25f,  0.0f, 0.0f, 0.0f,


		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f
	};

	MeshColor* obj2 = new MeshColor(); //Usamos MeshColor porque no dibujamos con índices, sino con vértices
	obj2->CreateMeshColor(vertices_piramide_bordes, 72); // 12 vértices * 6 floats = 72
	meshColorList.push_back(obj2); //Se guarda en la posición uno
}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,	-0.5f,	0.5f,	
		0.5f,	-0.5f,	-0.5f,	
		-0.5f,	-0.5f,	-0.5f,	
		-0.5f,	-0.5f,	0.5f,	
		0.0f,	0.5f,	0.0f,	
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);


}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa
	CrearCubo();//índice 0 en MeshList

	CrearPiramideTriangular();//índice 0 en MeshColorList
	CrearPiramideTriangularBordes();//índice 1 en MeshColorList
	CrearCilindro(5, 1.0f);//índice 1 en MeshList
	CrearCono(25, 2.0f);//índice 2 en MeshList
	CrearPiramideCuadrangular();//índice 3 en MeshList
	CreateShaders();
	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint outlineShader = 0;

	//GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	//glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;


	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[1].useShader();   // shader para color
		GLint uniformModel = shaderList[1].getModelLocation();
		GLint uniformView = shaderList[1].getViewLocation();
		GLint uniformProjection = shaderList[1].getProjectLocation();
		
		angulo += 0.1;
		
		//PIRÁMIDE 1

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //SOLO CON DECLARARLA UNA VEZ ES SUFICIENTE DENTRO DEL WHILE
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();		//Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines();	//Llamar a Render Mesh Color Lines para dibujar bordes

		
		//PIRÁMIDE 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.4330127019f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.4330127019f)); // en el origen
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); // tamaño original
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.8660254038f)); // en el origen
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); // tamaño original
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, 0.8660254038f)); // en el origen
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); // tamaño original
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordesmeshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 6
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.8660254038f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 7
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -0.4330127019f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 8
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.8660254038f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes
		
		//PIRÁMIDE 9
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértices
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		//PIRÁMIDE 10
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050808f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor(); //Llamar a Render Mesh Color para dibujar desde los vértice
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		// PIRÁMIDE 11 QUE ABARCA TODO EL TAMAÑO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla E se rota sobre el eje x
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla R se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla T se rota sobre el eje z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.8660254038f)); // en el origen
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 5.196152423f)); // tamaño original
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //SOLO CON DECLARARLA UNA VEZ ES SUFICIENTE DENTRO DEL WHILE
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines(); //Llamar a Render Mesh Color Lines para dibujar bordes

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		