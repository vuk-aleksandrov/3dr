#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

enum class ShaderType {
	Vertex,
	Fragment
};

std::string loadFile(const char* fileName)
{
	std::ifstream file(fileName);
	std::string line;

	std::string strFile;
	while (getline(file, line))
	{
		strFile += line + '\n';
		std::cout << line << std::endl;
		
	}

	std::cout<<fileName<<std::endl;
	std::cout<<strFile<<std::endl;
	return strFile;
}

int createShader(ShaderType type, const char* srcDest) {
	int shader;

	switch (type)
	{
	case ShaderType::Vertex:
		shader = glCreateShader(GL_VERTEX_SHADER); break;
	case ShaderType::Fragment:
		shader = glCreateShader(GL_FRAGMENT_SHADER); break;
	default:
		shader = 0;
		printf("Wrong Shader type specified!\n");
	}

	std::string src = loadFile(srcDest);
	const char* shaderSrc[] = { src.c_str() };

	glShaderSource(shader, 1, shaderSrc, nullptr);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		fprintf(stderr, "Vertex shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			fprintf(stderr, "Shader log:\n%s", log);
			free(log);
		}

		printf("Shader compilation failed\n");
	}

	return shader;
}

int main() {
	if (!glfwInit()) {
		printf("Failed to init glfw\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(600, 600, "3dr", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	// Vertex Array
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create vertex buffer
	float vertexData[] = {
		//		  vertices			     colors
		-0.5f, -0.5f, 0.0f,	 	1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (const void*)(3 * sizeof(float)));

	// Create index buffer
	uint32_t indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	uint32_t ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int program = glCreateProgram();
	int vertShader = createShader(ShaderType::Vertex, "C:/Projects/3dr/src/shaders/shader.vert");
	int fragShader = createShader(ShaderType::Fragment, "C:/Projects/3dr/src/shaders/shader.frag");

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		fprintf(stderr, "Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(program, logLen, &written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
	}


	glUseProgram(program);

	while (!glfwWindowShouldClose(window)) {
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

		// Swap front and back buffers
		glfwSwapBuffers(window);
		
		// Poll for and process events
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	glDeleteVertexArrays(1, &vao);

	glDeleteProgram(program);

	glfwTerminate();

	return 0;
}