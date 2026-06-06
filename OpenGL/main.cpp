#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


const unsigned int width = 800;
const unsigned int height = 800;


// Vertices coordinates
GLfloat vertices[] =
{
	// Positions              // Texture Coords

	// Front face
	-0.5f, -0.5f,  0.5f,      0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,      0.0f, 1.0f,

	// Back face
	-0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,      0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,      1.0f, 1.0f,

	// Left face
	-0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,      0.0f, 1.0f,

	// Right face
	 0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,      0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,      0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,      1.0f, 1.0f,

	 // Top face
	 -0.5f,  0.5f, -0.5f,      0.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f,      0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,      1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,      1.0f, 1.0f,

	  // Bottom face
	  -0.5f, -0.5f, -0.5f,      1.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,      0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,      0.0f, 1.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,  2, 3, 0,
	4, 5, 6,  6, 7, 4,
	8, 9,10, 10,11, 8,
   12,13,14, 14,15,12,
   16,17,18, 18,19,16,
   20,21,22, 22,23,20
};



int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	
	// Texture
	Texture boxTex("box.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	boxTex.texUnit(shaderProgram, "tex0", 0);

	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.12f, 0.05f, 0.31f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1.0f / 60.0f)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}

		// Initializes matrices so they are not the null matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		// Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


		// Binds texture so that is appears in rendering
		boxTex.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	boxTex.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}