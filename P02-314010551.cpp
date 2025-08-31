//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas SUS
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
//const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* redShader = "shaders/rojo.vert"; //Shader rojo
static const char* greenShader = "shaders/verde.vert"; //Shader verde
static const char* blueShader = "shaders/azul.vert"; //Shader azul
static const char* brownShader = "shaders/cafe.vert"; //Shader cafe
static const char* darkgreenShader = "shaders/verdeoscuro.vert"; //Shader verde oscuro

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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


/*
void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X		 Y		Z		R		G		B
		// Dibujando D Verde
		-0.95f,  0.4f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f,  0.4f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.95f,  0.0f, 0.0f,   0.0f, 0.6f, 0.3f,
	
		-0.95f, -0.4f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f, -0.4f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.95f,  0.0f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.75f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f,  0.4f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.35f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.75f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f, -0.4f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.35f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.55f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.35f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.35f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.35f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.75f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.75f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.95f,  0.0f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.65f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f,  0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f,  0.1f, 0.0f,   0.0f, 0.6f, 0.3f,

		-0.65f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f, -0.2f, 0.0f,   0.0f, 0.6f, 0.3f,
		-0.55f, -0.1f, 0.0f,   0.0f, 0.6f, 0.3f,

		
		// Dibujando J Morado
		-0.1f,  0.4f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.1f,  0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.3f,  0.2f, 0.0f,   0.5f, 0.2f, 0.7f,

		-0.1f,  0.4f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.3f,  0.4f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.3f,  0.2f, 0.0f,   0.5f, 0.2f, 0.7f,

		 0.0f,  0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.0f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.2f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,

		 0.0f,  0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.2f,  0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.2f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,

		 0.1f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.2f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.1f, -0.4f, 0.0f,   0.5f, 0.2f, 0.7f,

		-0.3f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.2f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.2f, -0.4f, 0.0f,   0.5f, 0.2f, 0.7f,

		-0.2f, -0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.2f, -0.4f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.1f, -0.4f, 0.0f,   0.5f, 0.2f, 0.7f,

		-0.2f, -0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.1f, -0.2f, 0.0f,   0.5f, 0.2f, 0.7f,
		 0.1f, -0.4f, 0.0f,   0.5f, 0.2f, 0.7f,

		-0.3f, -0.1f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.3f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.1f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,

		-0.3f, -0.1f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.1f, -0.1f, 0.0f,   0.5f, 0.2f, 0.7f,
		-0.1f, -0.3f, 0.0f,   0.5f, 0.2f, 0.7f,


		// Dibujando C Beige Claro
		 0.55f,  0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.35f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f, -0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.35f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.35f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.35f,  0.0f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.35f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.35f,  0.0f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f, -0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.35f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f,  0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f,  0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f, -0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f, -0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f,  0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.95f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f, -0.4f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.95f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.55f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.55f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.35f,  0.0f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.75f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f,  0.1f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.95f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.75f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f, -0.1f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.95f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.95f,  0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f,  0.1f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.95f,  0.1f, 0.0f,   0.95f, 0.9f, 0.75f,

		 0.95f, -0.2f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.75f, -0.1f, 0.0f,   0.95f, 0.9f, 0.75f,
		 0.95f, -0.1f, 0.0f,   0.95f, 0.9f, 0.75f,
	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 594);
	meshColorList.push_back(letras);
}
*/


void CreateShaders()
{
	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//Shader Rojo
	Shader* shader3 = new Shader();
	shader3->CreateFromFiles(redShader, fShader);
	shaderList.push_back(*shader3);

	//Shader Verde
	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(greenShader, fShader);
	shaderList.push_back(*shader4);

	//Shader Azul
	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(blueShader, fShader);
	shaderList.push_back(*shader5);

	//Shader Cafe
	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(brownShader, fShader);
	shaderList.push_back(*shader6);

	//Shader Verde Oscuro
	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(darkgreenShader, fShader);
	shaderList.push_back(*shader7);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformview = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		/*
		
		*************************************** BLOQUE PARA LAS LETRAS ***********************************

		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas de las letras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.1f)); //Mover las letras "adentro de la pantalla""
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor(); // El 0 son las letras
		***************************************************************************************************
		
		*/


		//Pirámide Azul
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		//angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -1.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.70f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Pirámides Verde Oscuro
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		//angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para la pirámide derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.79f, -0.35f, -1.0f)); //Hacia la derecha
		model = glm::scale(model, glm::vec3(0.42f, 0.70f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para la pirámide izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.79, -0.35f, -1.0f)); // Hacia la izquierda
		model = glm::scale(model, glm::vec3(0.42f, 0.70f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Cubo Rojo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		//angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para el cubo rojo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.345f, -1.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubos Verdes
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		//angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para el cubo verde izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.24f, -0.05f, -0.9f));
		model = glm::scale(model, glm::vec3(0.34f, 0.4532f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para el cubo verde derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.7684f, -0.9f));
		model = glm::scale(model, glm::vec3(0.34f, 0.4532f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para el cubo verde inferior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.24f, -0.05f, -0.9f));
		model = glm::scale(model, glm::vec3(0.34f, 0.4532f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubos Cafés
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		//angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para el cubo café izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.79f, -0.84825f, -0.8f));
		model = glm::scale(model, glm::vec3(0.2250f, 0.2950f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas para el cubo café derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.79f, -0.84825f, -0.8f));
		model = glm::scale(model, glm::vec3(0.2250f, 0.2950f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/