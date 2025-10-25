/*
Pr�ctica 8: Iluminaci�n 2 
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

//para iluminaci�n
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

Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dado8CarasTexture;


Model Poste_M;

Model Porsche2014Carro_M;
Model Porsche2014Llanta_M;
Model Porsche2014Cofre_M;
Model Blackhawk_M;

Model Mago_M;

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
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

//Segundo Arreglo de luces puntuales
PointLight pointLights2[MAX_POINT_LIGHTS];//MAX_POINT_LIGHTS es el mismo o diferente?

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//funci�n de calculo de normales por promedio de v�rtices 
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

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDadoOchoCaras()
{
	unsigned int cubo8caras_indices[] = {
		// Top
		// front
		0, 1, 2,
		// Right
		3, 4, 5,
		// back
		6, 7, 8,
		// left
		9, 10, 11,
		// Bottom
		// front
		12, 13, 14,
		// right
		15, 16, 17,
		// back
		18, 19, 20,
		// left
		21, 22, 23
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo8caras_vertices[] = {
		// Arriba
		// front
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f,  0.0f,		0.50f,  0.25f,		0.0f,	-1.0f,	-1.0f,	//0
		-0.5f, 0.0f,  0.5f,		0.75f,	0.5f,		0.0f,	-1.0f,	-1.0f,	//1
		0.5f,  0.0f,  0.5f,		0.25f,	0.5f,		0.0f,	-1.0f,	-1.0f,	//2
		// right
		//x		y		z		S		T
		0.0f,  0.5f,  0.0f,	    0.50f,  0.25f,		-1.0f,	-1.0f,	0.0f,	//3
		0.5f,  0.0f,  0.5f,		0.25f,	0.5f,		-1.0f,	-1.0f,	0.0f,	//4
		0.5f,  0.0f, -0.5f,		0.007f,	0.25f,		-1.0f,	-1.0f,	0.0f,	//5
		// back
		0.0f,  0.5f, 0.0f,		0.50f,  0.25f,		0.0f,	-1.0f,	1.0f,	//6
		-0.5f, 0.0f, -0.5f,		0.25f,	0.05f,		0.0f,	-1.0f,	1.0f,	//7
		0.5f,  0.0f, -0.5f,		0.05f,	0.25f,		0.0f,	-1.0f,	1.0f,	//8

		// left
		//x		y		z		S		T
		0.0f,  0.5f,  0.0f,		0.50f,  0.25f,		1.0f,	-1.0f,	0.0f,	//9
		-0.5f,  0.0f,  0.5f,	0.75f,	0.5f,		1.0f,	-1.0f,	0.0f,	//10
		-0.5f,  0.0f, -0.5f,	0.98f,	0.26f,		1.0f,	-1.0f,	0.0f,	//11

		// bottom
		// front
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.0f,		0.50f,  0.75f,		0.0f,	1.0f,	-1.0f,	//12
		-0.5f, 0.0f,  0.5f,		0.66f,	0.50f,		0.0f,	1.0f,	-1.0f,	//13
		0.5f,  0.0f,  0.5f,		0.33f,	0.50f,		0.0f,	1.0f,	-1.0f,	//14
		// right
		//x		y		z		S		T
		0.0f,  -0.5f, 0.0f,	    0.50f,  0.75f,		-1.0f,	1.0f,	0.0f,	//15
		0.5f,  0.0f,  0.5f,		0.25f,	0.5f,		-1.0f,	1.0f,	0.0f,	//16
		0.5f,  0.0f, -0.5f,		0.01f,	0.75f,		-1.0f,	1.0f,	0.0f,	//17
		// back
		0.0f,  -0.5f, 0.0f,		0.50f,  0.75f,		0.0f,	1.0f,	1.0f,	//18
		-0.5f, 0.0f, -0.5f,		0.25f,	1.0f,		0.0f,	1.0f,	1.0f,	//19
		0.5f,  0.0f, -0.5f,		0.01f,	0.75f,		0.0f,	1.0f,	1.0f,	//20

		// left
		//x		y		z		S		T
		0.0f,  -0.5f, 0.0f,		0.50f,  0.75f,		1.0f,	1.0f,	0.0f,	//21
		-0.5f,  0.0f,  0.5f,	0.75f,	0.5f,		1.0f,	1.0f,	0.0f,	//22
		-0.5f,  0.0f, -0.5f,	1.0f,	0.75f,		1.0f,	1.0f,	0.0f,	//23

	};

	Mesh* dadoOchoCaras = new Mesh();
	dadoOchoCaras->CreateMesh(cubo8caras_vertices, cubo8caras_indices, 24 * 8, 24);
	meshList.push_back(dadoOchoCaras);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDadoOchoCaras();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dado8CarasTexture = Texture("Textures/dado8caras.png");
	dado8CarasTexture.LoadTextureA();


	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Porsche2014Carro_M = Model();
	Porsche2014Carro_M.LoadModel("Models/porsche2014_carro.obj");

	Porsche2014Llanta_M = Model();
	Porsche2014Llanta_M.LoadModel("Models/porsche2014_llanta.obj");

	Porsche2014Cofre_M = Model();
	Porsche2014Cofre_M.LoadModel("Models/porsche2014_cofre.obj");

	Poste_M = Model();
	Poste_M.LoadModel("Models/poste.obj");

	Mago_M = Model();
	Mago_M.LoadModel("Models/mago.obj");

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

	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// Cofre
	spotLights[1] = SpotLight(1.0f, 0.2f, 0.2f,
		0.7f, 0.7f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0001f,
		15.0f);
	spotLightCount++;

	// Helicoptero
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.7f, 0.5f,
		3.0f, 2.0f, 5.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.3f, 0.0f,
		15.0f);
	spotLightCount++;

	// Faro carro delantero
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.001f,
		15.0f);
	spotLightCount++;

	// Faro carro trasero
	spotLights[4] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.001f,
		15.0f);
	spotLightCount++;

	// Segundo arreglo para spotLights (Luz delantera al final)
	spotLights2[0] = spotLights[0];
	spotLights2[1] = spotLights[1];
	spotLights2[2] = spotLights[2];
	spotLights2[3] = spotLights[4];
	spotLights2[4] = spotLights[3];

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaraci�n de primer arreglo de luces puntuales
	//Para el mago 
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		1.0f, 0.7f,
		-57.0f, 16.0f, 0.0,
		0.0f, 0.0f, 0.015f);
	pointLightCount++;
	// Luz para el poste
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.6f, 0.8f,
		-8.0f, 10.0f, -7.0f,
		0.0f, 0.0f, 0.01f);
	pointLightCount++;

	//Segundo arreglo  de luces puntuales: blanca,verde, azul, roja
	pointLights2[0] = pointLights[1];
	//pointLights2[0].SetPos(glm::vec3(-3.0f, 1.0f, 2.0f)); //cambiar posici�n
	pointLights2[1] = pointLights[0];

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelCofre(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);

	// Para que el cofre rote con la luz 
	glm::vec4 luzCofrePosLocal = glm::vec4(0.0f, -0.25f, -1.0f, 1.0f); 
	glm::vec4 luzCofreDirLocal = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);  // Direcci�n RELATIVA al cofre (w=0)

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

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());
		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
	

		if (mainWindow.getprendida())
		{
			if (mainWindow.getlucespuntuales() < 0.5f)
			{
				// Ambos ON
				shaderList[0].SetPointLights(pointLights, pointLightCount);;
			}
			else
			{
				// Ambos OFF
				shaderList[0].SetPointLights(pointLights2, 0); 
			}
		}
		else
		{
			if (mainWindow.getlucespuntuales() < 0.5f)
			{
				// Objeto ON, Poste OFF
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
			}
			else
			{
				// Poste ON, Objeto OFF
				shaderList[0].SetPointLights(pointLights2, pointLightCount - 1);
			}
		}

		//Movimiento del carro 
		if (mainWindow.getDireccion() > 0.5f) // Avanzando
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount-1);
		}
		else {
			shaderList[0].SetSpotLights(spotLights2, spotLightCount-1);
		}

		// Dado 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.5f, 15.5f, -2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8CarasTexture.UseTexture();
		meshList[4]->RenderMesh();

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), -0.2f, 4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche2014Carro_M.RenderModel();

		// Articulacion Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.718f, -0.69f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelCofre = model;
		// Cofre
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche2014Cofre_M.RenderModel();
		// Luz cofre 
		spotLights[1].SetFlash(glm::vec3(modelCofre * luzCofrePosLocal), glm::normalize(glm::vec3(modelCofre * luzCofreDirLocal)));
		spotLights2[1].SetFlash(glm::vec3(modelCofre * luzCofrePosLocal), glm::normalize(glm::vec3(modelCofre * luzCofreDirLocal)));
		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.77f, 0.18f, 1.3f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche2014Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.77f, 0.18f, 1.3f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche2014Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.74f, 0.17f, -1.17f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche2014Llanta_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.74f, 0.17f, -1.17f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Porsche2014Llanta_M.RenderModel();

		// Luz en el faro trasero
		model = modelaux;
		spotLights2[3].SetPos(glm::vec3(model[3].x+ 7.0f, model[3].y+ 2.5f, model[3].z+ 4.0f));

		// Luz en el faro delantero
		model = modelaux;
		spotLights[3].SetPos(glm::vec3(model[3].x + -7.0f, model[3].y + 2.5f, model[3].z + 4.0f));

		// Helicoptero
		//mainWindow.getmuevexhelicoptero();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f+ mainWindow.getmuevexhelicoptero(), 5.0f, 12.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// Luz en el helicoptero
		spotLights[2].SetPos(glm::vec3(model[3].x, model[3].y, model[3].z));
		spotLights2[2].SetPos(glm::vec3(model[3].x, model[3].y, model[3].z));

		// Mago
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mago_M.RenderModel();

		/* Mago apoyo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-57.0f, 16.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mago_M.RenderModel();
		*/
		//Poste
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 7.0f, -12.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste_M.RenderModel();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
