#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

//Adapted from Edward Angels InitShader code

// Create a NULL-terminated string by reading the provided file
static char* readShaderSource(const char* shaderFile)
{
	ifstream ifs(shaderFile, ios::in | ios::binary | ios::ate);
	if (ifs.is_open())
	{
		unsigned int filesize = static_cast<unsigned int>(ifs.tellg());
		ifs.seekg(0, ios::beg);
		char* bytes = new char[filesize + 1];
		memset(bytes, 0, filesize + 1);
		ifs.read(bytes, filesize);
		ifs.close();
		return bytes;
	}
	return nullptr;
}

void printShaderCompileError(GLuint shader)
{
	GLint logSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
	char* logMsg = new char[logSize];
	glGetShaderInfoLog(shader, logSize, nullptr, logMsg);
	cerr << logMsg << endl;
	delete[] logMsg;
}

void printProgramLinkError(GLuint program)
{
	GLint logSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
	char* logMsg = new char[logSize];
	glGetProgramInfoLog(program, logSize, nullptr, logMsg);
	cerr << logMsg << endl;
	delete[] logMsg;
}

GLuint InitShader(const char* computeShaderFile)
{
	bool error = false;
	struct Shader
	{
		const char* filename;
		GLenum type;
		GLchar* source;
	} shaders[1] =
		{
			{computeShaderFile, GL_COMPUTE_SHADER, nullptr}
		};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 1; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == nullptr)
		{
			cerr << "Failed to read " << s.filename << endl;
			error = true;
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, nullptr);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			cerr << s.filename << " failed to compile:" << endl;
			printShaderCompileError(shader);
			error = true;
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		cerr << "Shader program failed to link" << endl;
		printProgramLinkError(program);

		error = true;
	}

	if (error == true)
	{
		return -1;
	}

	/* use program object */
	glUseProgram(program);

	return program;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char* vShaderFile, const char* fShaderFile)
{
	bool error = false;
	struct Shader
	{
		const char* filename;
		GLenum type;
		GLchar* source;
	} shaders[2] =
		{
			{vShaderFile, GL_VERTEX_SHADER, nullptr},
			{fShaderFile, GL_FRAGMENT_SHADER, nullptr}
		};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 2; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == nullptr)
		{
			cerr << "Failed to read " << s.filename << endl;
			error = true;
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, nullptr);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			cerr << s.filename << " failed to compile:" << endl;
			printShaderCompileError(shader);
			error = true;
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		cerr << "Shader program failed to link" << endl;
		printProgramLinkError(program);

		error = true;
	}

	if (error == true)
	{
		return -1;
	}

	/* use program object */
	glUseProgram(program);
	return program;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char* vShaderFile, const char* gShaderFile, const char* fShaderFile)
{
	bool error = false;
	struct Shader
	{
		const char* filename;
		GLenum type;
		GLchar* source;
	} shaders[3] =
		{
			{vShaderFile, GL_VERTEX_SHADER, nullptr},
			{gShaderFile, GL_GEOMETRY_SHADER, nullptr},
			{fShaderFile, GL_FRAGMENT_SHADER, nullptr}
		};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 3; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == nullptr)
		{
			cerr << "Failed to read " << s.filename << endl;
			error = true;
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, nullptr);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			cerr << s.filename << " failed to compile:" << endl;
			printShaderCompileError(shader);
			error = true;
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		cerr << "Shader program failed to link" << endl;
		printProgramLinkError(program);

		error = true;
	}

	if (error == true)
	{
		return -1;
	}

	/* use program object */
	glUseProgram(program);

	return program;
}

GLuint InitShader_TransformFeedback(const char* vertexShaderFile, const char* fragmentShaderFile,
                                    vector<string> varying_names)
{
	bool error = false;
	struct Shader
	{
		const char* filename;
		GLenum type;
		GLchar* source;
	} shaders[2] =
		{
			{vertexShaderFile, GL_VERTEX_SHADER, nullptr},
			{fragmentShaderFile, GL_FRAGMENT_SHADER, nullptr}
		};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 2; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == nullptr)
		{
			cerr << "Failed to read " << s.filename << endl;
			error = true;
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, nullptr);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			cerr << s.filename << " failed to compile:" << endl;
			printShaderCompileError(shader);
			error = true;
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	// Tranformfeed back
	const size_t size = varying_names.size();
	const GLchar* feedbackVaryings[10];
	for(auto i = 0; i < 10; ++i)
	{
		feedbackVaryings[i] = varying_names[i].c_str();
	}

	glTransformFeedbackVaryings(program, sizeof(varying_names), feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	/* link  and error check */
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		cerr << "Shader program failed to link" << endl;
		printProgramLinkError(program);

		error = true;
	}

	if (error == true)
	{
		return -1;
	}

	/* use program object */
	glUseProgram(program);
	return program;
}


GLuint InitShader_TransformFeedback(const char* vertexShaderFile, const char* geometryShader,
                                    const char* fragmentShaderFile, vector<string> varying_names)
{
	bool error = false;
	struct Shader
	{
		const char* filename;
		GLenum type;
		GLchar* source;
	} shaders[3] =
		{
			{vertexShaderFile, GL_VERTEX_SHADER, nullptr},
			{geometryShader, GL_GEOMETRY_SHADER, nullptr},
			{fragmentShaderFile, GL_FRAGMENT_SHADER, nullptr}
		};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 3; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == nullptr)
		{
			cerr << "Failed to read " << s.filename << endl;
			error = true;
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, nullptr);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			cerr << s.filename << " failed to compile:" << endl;
			printShaderCompileError(shader);
			error = true;
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}


	// Tranformfeed back
	const GLchar* feedbackVaryings[10];
	for (unsigned int i = 0; i < 10; ++i)
	{
		if (i < varying_names.size())
			feedbackVaryings[i] = varying_names[i].c_str();
		else
			feedbackVaryings[i] = "";
	}

	glTransformFeedbackVaryings(program, varying_names.size(), feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
	
	/* link  and error check */
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		cerr << "Shader program failed to link" << endl;
		printProgramLinkError(program);

		error = true;
	}

	if (error == true)
	{
		return -1;
	}

	/* use program object */
	glUseProgram(program);

	return program;
}
