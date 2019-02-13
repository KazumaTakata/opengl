/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| "Hello Triangle". Just the basics.                                           |
| If you're on Apple un-comment the version number code at the beginning. It   |
| will give you the latest, even if you say 3.2!                               |
| This uses the libraries GLEW and GLFW3 to start GL. Download and compile     |
| these first. Linking them might be a pain, but you'll need to master this.   |
\******************************************************************************/

/* NOTE(Anton) for OS X Mojave users:
I updated this .c file and Makefile.osx to compile and run on Mojave
The older compiled libraries in this repository will not link, so I modified the Makefile to
link against dynamic libraries installed on the system.
I suggest you:
1. Download and install Homebrew
2. brew install glfw and brew install glew
- I also had to delete and reinstall XCode to update the c build tools.
- Note that you may get a black screen until the window is moved or resized. I understand both GLFW
and SDL libraries have this problem since Mojave and may require a patch to access the context in a
different order than previous versions of OS X.
*/

#include <GL/glew.h>	/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

char *fileread(char *filename);

int main()
{
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;
	GLuint vao;
	GLuint vbo;

	/* geometry to use. these are 3 xyz points (9 floats total) to make a triangle */
	float points[] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
	};
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int EBO;

	/* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
	// const char *vertex_shader = "#version 410\n"
	// 							"in vec3 vp;"
	// 							"void main () {"
	// 							"  gl_Position = vec4(vp, 1.0);"
	// 							"}";
	const char *vertex_shader = fileread("vertex_shader.glsl");

	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */

	const char *fragment_shader = fileread("fragment_shader.glsl");

	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vert_shader,
		frag_shader;
	/* GL shader programme object [combined, to link] */
	GLuint shader_program;

	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	/* start GLEW extension handler */
	glewExperimental = GL_TRUE;
	glewInit();

	/* get version info */
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION);   /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer
	than anything already drawn at that pixel */
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
	/* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
	glDepthFunc(GL_LESS);

	/* a vertex buffer object (VBO) is created here. this stores an array of
	data on the graphics adapter's memory. in our case - the vertex points */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	/* the vertex array object (VAO) is a little descriptor that defines which
	data from vertex buffer objects should be used as input variables to vertex
	shaders. in our case - use our only VBO, and say 'every three floats is a
	variable' */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/* "attribute #0 should be enabled when this vao is bound" */

	/* this VBO is already bound, but it's a good habit to explicitly specify which
	VBO's data the following vertex attribute pointer refers to */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	/* "attribute #0 is created from every 3 variables in the above buffer, of type
	float (i.e. make me vec3s)" */
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* here we copy the shader strings into GL shaders, and compile them. we
	then create an executable shader 'program' and attach both of the compiled
			shaders. we link this, which matches the outputs of the vertex shader to
	the inputs of the fragment shader, etc. and it is then ready to use */
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader);
	shader_program = glCreateProgram();
	glAttachShader(shader_program, frag_shader);
	glAttachShader(shader_program, vert_shader);
	glLinkProgram(shader_program);

	/* this loop clears the drawing surface, then draws the geometry described
			by the VAO onto the drawing surface. we 'poll events' to see if the window
	was closed, etc. finally, we 'swap the buffers' which displays our drawing
			surface onto the view area. we use a double-buffering system which means
			that we have a 'currently displayed' surface, and 'currently being drawn'
			surface. hence the 'swap' idea. in a single-buffering system we would see
			stuff being drawn one-after-the-other */

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("sample.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	}
	stbi_image_free(data);

	while (!glfwWindowShouldClose(window))
	{
		/* wipe the drawing surface clear */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);

		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shader_program, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

		unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		/* draw points 0-3 from the currently bound VAO with current in-use shader */
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/* update other events like input handling */
		glfwPollEvents();
		/* put the stuff we've been drawing onto the display */
		glfwSwapBuffers(window);
	}

	/* close GL context and any other GLFW resources */
	glfwTerminate();
	return 0;
}

char *fileread(char *filename)
{
	char *buffer = 0;
	long length;
	FILE *f = fopen(filename, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char *)malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}

	return buffer;
}
