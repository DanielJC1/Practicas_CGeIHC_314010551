/*
Práctica 8: Iluminación 2 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;
Texture paredTexture;

Model Antorcha_M;
Model Dona_M;

Model Chasis_M;
Model Cofre_M;
Model LlantaTraseraDerecha_M;
Model LlantaTraseraIzquierda_M;
Model LlantaDelanteraDerecha_M;
Model LlantaDelanteraIzquierda_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
// para declarar varias luces de tipo spotlight
// Primer arreglo de luces spotlight
SpotLight spotLights[MAX_SPOT_LIGHTS];
// Segunndo arreglo de luces spotlight
SpotLight spotLights2[MAX_SPOT_LIGHTS];
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

//Segundo Arreglo de luces puntuales
PointLight pointLights2[MAX_POINT_LIGHTS];//MAX_POINT_LIGHTS es el mismo o diferente?

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f, //4
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f, //5
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f, //6
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,//7
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,//8
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,//9
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,//10
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,//11

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,//12
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,//13
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,//14
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,//15

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	paredTexture = Texture("Textures/brickblanco.png");
	paredTexture.LoadTextureA();

	Antorcha_M = Model();
	Antorcha_M.LoadModel("Models/antorcha.obj");
	Dona_M = Model();
	Dona_M.LoadModel("Models/dona.obj");

	Chasis_M = Model();
	Cofre_M = Model();
	LlantaTraseraDerecha_M = Model();
	LlantaTraseraIzquierda_M = Model();
	LlantaDelanteraDerecha_M = Model();
	LlantaDelanteraIzquierda_M = Model();
	Chasis_M.LoadModel("Models/chasis.obj");
	Cofre_M.LoadModel("Models/cofre.obj");
	LlantaTraseraDerecha_M.LoadModel("Models/llanta_trasera_derecha.obj");
	LlantaTraseraIzquierda_M.LoadModel("Models/llanta_trasera_izquierda.obj");
	LlantaDelanteraDerecha_M.LoadModel("Models/llanta_delantera_derecha.obj");
	LlantaDelanteraIzquierda_M.LoadModel("Models/llanta_delantera_izquierda.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	
	unsigned int spotLightCount = 0;

	//Primer arreglo de luces spotlight
	//Luz cofre
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f, // color azul
		1.0f, 2.0f,								// intensidades ambiental y difusa
		0.0f, 0.0f, 0.0f,						// posición inicial del foco
		0.0f, 0.0f, 0.0f,						// dirección (no apunta a ningún lugar)
		1.0f, 0.01f, 0.001f,					// atenuación constante, lineal, exponencial
		20.0f);									// ángulo (edge)
	spotLightCount++;

	//luz delantera

	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f, // color azul
		1.0f, 2.0f,								// intensidades ambiental y difusa
		0.0f, 0.0f, 0.0f,						// posición inicial del foco
		0.0f, 0.0f, 0.0f,						// dirección (no apunta a ningún lugar)
		1.0f, 0.01f, 0.001f,					// atenuación constante, lineal, exponencial
		20.0f);									// ángulo (edge)
	spotLightCount++;


	//Segundo arreglo de luces spotlight
	// Luz cofre
	
	spotLights2[0] = spotLights[0];

	//luz trasera

	spotLights2[1] = SpotLight(1.0f, 0.0f, 0.0f, // color azul
		1.0f, 2.0f,								// intensidades ambiental y difusa
		0.0f, 0.0f, 0.0f,						// posición inicial del foco
		0.0f, 0.0f, 0.0f,						// dirección (no apunta a ningún lugar)
		1.0f, 0.01f, 0.001f,					// atenuación constante, lineal, exponencial
		20.0f);									// ángulo (edge)

	
	//se crean mas luces puntuales y spotlight 

	//contador de luces puntuales
	unsigned int pointLightCount = 0;


	// Primer arreglo de luces puntuales
	//Luz antorcha
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, // poisición inicial 
		1.0f, 0.0f, 0.1f);
	pointLightCount++;


	//Luz dona
	pointLights[1] = PointLight(0.937f, 0.0f, 0.549f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, // poisición inicial 
		1.0f, 0.0f, 0.1f);
	pointLightCount++;

	// Segundo arreglo de luces puntuales
	//Luz dona
	pointLights2[0] = pointLights[1];

	//Luz antorcha
	pointLights2[1] = pointLights[0];





	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux1(1.0);//Matriz auxiliar llantas
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f,0.0f,0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		//lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetSpotLights(spotLights2, MAX_SPOT_LIGHTS);

		//CONTROL DE LUCES

		//LUCES DE OBJETO Y LÁMPARA

		if (mainWindow.getlucestorchdonut())
		{
			if (mainWindow.getluzswitch())
			{
				//Todas encendidads
				shaderList[0].SetPointLights(pointLights, pointLightCount);
			}
			else
			{	//Todas apagadas
				shaderList[0].SetPointLights(pointLights, pointLightCount-2);
			}
		}
		else
		{
			if (mainWindow.getluzswitch())
			{
				//Luz de antorcha, prendida luz de dona apagada
				shaderList[0].SetPointLights(pointLights, pointLightCount-1);
			}
			else
			{	//Luz de dona prendida, luz de antorcha apagada
				shaderList[0].SetPointLights(pointLights2, pointLightCount - 1);

			}
		}

		//LUCES TRASERA Y DELANTERA

		if (mainWindow.getluceschasis() > 0.75f)
		{
			//Prender luz del cofre y luz delantera 
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else if (mainWindow.getluceschasis() < 0.5f && mainWindow.getluceschasis() > 0.0f)
		{
			//Prender luz del cofre y luz trasera
			shaderList[0].SetSpotLights(spotLights2, spotLightCount);		
		}
		else
		{
			// Solo prender la del cofre
			shaderList[0].SetSpotLights(spotLights, spotLightCount - 1);
		}


		//=======================INICIALIZACIÓN DE MODELOS ================================



		//PISO
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pared de enfrente
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 14.1f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		paredTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Pared de arriba
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 30.1f, 0.0f));
		model = glm::scale(model, glm::vec3(82.0f, 2.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		paredTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Pared de atrás
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, 14.1f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		paredTexture.UseTexture();
		meshList[4]->RenderMesh();


		//Lámpara (antorcha)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 30.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Antorcha_M.RenderModel();

		//Luz lámpara
		model = modelaux;
		pointLights[0].SetPos(glm::vec3(model[3].x, model[3].y + 0.25f, model[3].z)); //subir un poco la luz 
		pointLights2[1].SetPos(glm::vec3(model[3].x, model[3].y + 0.25f, model[3].z)); //subir un poco la luz 


		//Dona
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, 30.0f));
		modelaux = model;
		//model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dona_M.RenderModel();
		model = modelaux;
		pointLights[1].SetPos(glm::vec3(model[3])); 
		pointLights2[0].SetPos(glm::vec3(model[3]));


		//Carro
		//Creando Chasis y Matrices auxiliares
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.4f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//Traslación del modelo completo con teclado para que pueda avanzar y retroceder de forma independiente: Y para avanzar, U para retroceder
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmuevecarro()));
		modelaux = model; //Matriz auxiliar para construir el cofre
		modelaux1 = model; // Matriz auxiliar para construir las ruedas
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Chasis_M.RenderModel();

		//Luz delantera
		model = modelaux;
		spotLights[1].SetFlash(glm::vec3(model[3].x-5.6f, model[3].y-0.15f, model[3].z), glm::normalize(glm::vec3(model * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))));

		//Luz trasera
		spotLights2[1].SetFlash(glm::vec3(model[3].x+6.0f, model[3].y-0.375, model[3].z), glm::normalize(glm::vec3(model * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f))));




		//Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.35f, 18.75f));
		model = glm::rotate(model, glm::radians(mainWindow.getmuevecofre()), glm::vec3(1.0f, 0.0f, 0.0f)); // V para cerrar, C para abrir
		modelaux = model; //Almacenar las transformaciones en modelaux para la luz
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();//Renderizar cofre

		//Luz Cofre
		model = modelaux;
		spotLights[0].SetFlash(glm::vec3(model[3]), glm::normalize(glm::vec3(model * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))));
		spotLights2[0].SetFlash(glm::vec3(model[3]), glm::normalize(glm::vec3(model* glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))));

		// Llantas
		// Llanta delantera derecha
		model = modelaux1;
		model = glm::translate(model, glm::vec3(8.0f, -4.0f, -15.0f));
		model = glm::rotate(model, glm::radians(-mainWindow.getmuevecarro()), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotar llantas
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDelanteraDerecha_M.RenderModel();

		// Llanta delantera izquierda
		model = modelaux1;
		model = glm::translate(model, glm::vec3(-8.0f, -4.0f, -15.0f));
		model = glm::rotate(model, glm::radians(-mainWindow.getmuevecarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDelanteraIzquierda_M.RenderModel();

		// Llanta trasera derecha
		model = modelaux1;
		model = glm::translate(model, glm::vec3(8.0f, -4.0f, 14.5f));
		model = glm::rotate(model, glm::radians(-mainWindow.getmuevecarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTraseraDerecha_M.RenderModel();

		// Llanta trasera izquierda
		model = modelaux1;
		model = glm::translate(model, glm::vec3(-8.0f, -4.0f, 14.5f));
		model = glm::rotate(model, glm::radians(-mainWindow.getmuevecarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTraseraIzquierda_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
