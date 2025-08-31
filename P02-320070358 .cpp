//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
#include"Mesh.h" // Dibujo de figuras
#include"Shader.h" // Iniciar figuras
#include"Window.h" // Todo necesario para glfw
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader (Vienen en pares)
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* fShaderRojo = "shaders/rojo.frag";
static const char* fShaderVerde = "shaders/verde.frag";
static const char* fShaderAzul = "shaders/azul.frag";
static const char* fShaderCafe = "shaders/cafe.frag";
static const char* fShaderVerdeOscuro = "shaders/verdeOscuro.frag";

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
		-0.5f, -0.5f,0.0f, //0
		0.5f,-0.5f,0.0f, //1
		0.0f,0.5f, -0.25f, //2
		0.0f,-0.5f,-0.5f, //3

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
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letraA[] = {
		// A
		//x		y		z		R		G		B
		-0.7f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.8f,0.6f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.8f,0.1f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.8f,0.1f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f,0.1f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.35f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.35f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.25f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.35f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.25f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.25f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.1f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.4f, 0.1f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.4f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.4f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.1f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.5f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.7f, 0.5f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f,0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.6f,0.0f, 0.0f, 0.0f, 1.0f,
	};
	MeshColor* letraA = new MeshColor();
	letraA->CreateMeshColor(vertices_letraA, 144);
	meshColorList.push_back(letraA);


	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	};


	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);
	// Index 3

	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	// Index 4
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	// Index: 5
	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	// Index: 6
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_letraT[] = {
		// T
		//x		y		z		R		G		B
		0.8f, 0.6f,0.0f, 1.0f, 0.0f, 0.0f,
		0.4f, 0.6f,0.0f, 1.0f, 0.0f, 0.0f,
		0.4f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
		0.4f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
		0.8f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
		0.8f, 0.6f,0.0f, 1.0f, 0.0f, 0.0f,
		0.55f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
		0.55f, 0.1f,0.0f, 1.0f, 0.0f, 0.0f,
		0.65f, 0.1f,0.0f, 1.0f, 0.0f, 0.0f,
		0.55f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
		0.65f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
		0.65f, 0.1f,0.0f, 1.0f, 0.0f, 0.0f,
	};
	MeshColor* letraT = new MeshColor();
	letraT->CreateMeshColor(vertices_letraT, 77);
	meshColorList.push_back(letraT);

	GLfloat vertices_letraL[] = {
		// L
		-0.8f, -0.2f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.8f, -0.6f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.7f, -0.6f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.8f, -0.2f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.7f, -0.2f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.7f, -0.6f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.7f, -0.5f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.7f, -0.6f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.6f,0.0f, 0.0f, 1.0f, 0.0f,
		-0.7f, -0.6f,0.0f, 0.0f, 1.0f, 0.0f

	};
	MeshColor* letraL = new MeshColor();
	letraL->CreateMeshColor(vertices_letraL, 77);
	meshColorList.push_back(letraL);

}

void CreateShaders()
{
	// Index 0
	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	// Index 1
	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	// Nuevos shaders 
	// Index 2
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShader, fShaderRojo);
	shaderList.push_back(*shaderRojo);

	// Index 3
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShader, fShaderVerde);
	shaderList.push_back(*shaderVerde);

	// Index 4
	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShader, fShaderAzul);
	shaderList.push_back(*shaderAzul);

	// Index 5
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShader, fShaderCafe);
	shaderList.push_back(*shaderCafe);

	// Index 6
	Shader* shaderVerdeOscuro = new Shader();
	shaderVerdeOscuro->CreateFromFiles(vShader, fShaderVerdeOscuro);
	shaderList.push_back(*shaderVerdeOscuro);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f) ,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		//DESCOMENTAR EL ANGULO SI SE QUIERE GIRAR LA CASA
		// angulo += 0.01f;  

		
		//	EJERCICIO 1

		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList
		shaderList[1].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		/*
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		// A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		// T
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();
		// L
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[8]->RenderMeshColor();
		*/

		
		// EJERCICIO 2

		// Cuadrado rojo grande
		shaderList[2].useShader(); // Asegurarse de que el shader rojo esté activo
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.345f, -4.0f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.29f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Triangulo azul
		shaderList[4].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, -4.0f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Cuadrado verde izquierda
		shaderList[3].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, 0.0, -3.9f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.35f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cuadrado verde derecha
		shaderList[3].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, 0.0, -3.9f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.35f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cuadrado verde abajo
		shaderList[3].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.8, -3.9f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.35f, 1.0f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cuadrado cafe izquierda
		shaderList[5].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation(); 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, -0.85, -4.0f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.30f, 0.23f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cuadrado cafe derecha
		shaderList[5].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.78f, -0.85, -4.0f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.3f, 0.23f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Triangulo verde izquierda
		shaderList[6].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, -0.35, -4.0f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.7f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Triangulo verde derecha
		shaderList[6].useShader(); // Se llama la shader en la ubicacion 1 dentro de la lista de shader (shader color en este caso)
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.78f, -0.35, -4.0f));
		model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.7f, 0.4f));	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		


		/*
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		*/

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
//////////////// ESCALA ////////////////
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