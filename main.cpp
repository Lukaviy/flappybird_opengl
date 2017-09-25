#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <mat4x4.hpp>

void print_log(GLuint object) {
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		std::cout << "printlog: Not a shader or a program" << std::endl;
		return;
	}

	char* log = new char[log_length];

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, nullptr, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, nullptr, log);

	std::cout << log << std::endl;
	delete[] log;
}

int create_shader(char* file_name, GLenum shader_type) {
	GLuint res;
	GLint compile_ok = GL_FALSE;

	std::ifstream fin(file_name);
	if (!fin) {
		std::cout << "Can't open file: " << file_name << std::endl;
		return 0;
	}
	fin.seekg(0, fin.end);
	int file_size = fin.tellg();
	fin.seekg(0, fin.beg);

	char *s_source = new char[file_size + 1];
	memset(s_source, 0, file_size);
	fin.read(s_source, file_size);

	res = glCreateShader(shader_type);
	glShaderSource(res, 1, &s_source, nullptr);
	glCompileShader(res);
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

	if (!compile_ok) {
		std::cout << "Error in shader: " << file_name << std::endl;
		print_log(res);
		return 0;
	}

	return res;
}

bool keys[256];

GLuint program;

int get_attrib_location(const char* attrib_name, GLint* attrib) {
	*attrib = glGetAttribLocation(program, attrib_name);
	if (*attrib == -1) {
		std::cout << "Could not bind attribute " << attrib_name << std::endl;
		return 1;
	}
	return 0;
}

int get_uniform_location(const char* uniform_name, GLint* uniform) {
	*uniform = glGetUniformLocation(program, uniform_name);
	if (*uniform == -1) {
		std::cout << "Could not bind uniform " << uniform_name << std::endl;
		return 1;
	}
	return 0;
}

GLuint vbo_obj_vertices;
GLuint ibo_obj_elements;
GLuint vao_id;

GLint attribute_coord3d;
GLint attribute_texcoord;
GLint uniform_map;
GLint uniform_x_pos;
GLint uniform_bird_y_pos;
GLint uniform_bird_y_velocity;

int init_resources() {
	memset(&keys, 0, sizeof(keys));
	// Компилируем вершинный шейдер

	GLuint vs;

	if (!(vs = create_shader("shaders/main_shader.vs", GL_VERTEX_SHADER)))
		return 1;

	//Компилируем фрагментный шейдер

	GLuint fs;
	if (!(fs = create_shader("shaders/main_shader.fs", GL_FRAGMENT_SHADER)))
		return 1;

	GLint link_ok;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

	if (!link_ok) {
		print_log(vs);
		return 1;
	}
	glUseProgram(program);

	GLfloat vertices[] = {
		+0.0f, +1.0f, +0.0f,
		-1.0f, -1.0f, +0.0f,
		+1.0f, -1.0f, +0.0f
	};

	/*
	GLshort indeces[] = {
		0, 1, 2,
		0, 1, 3,
		0, 2, 3,
		1, 2, 3
	};*/

	glGenBuffers(1, &vbo_obj_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_obj_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	glGenBuffers(1, &ibo_obj_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_obj_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	*/

	glUseProgram(program);
	if (get_attrib_location("coord3d", &attribute_coord3d)) return 1;
	glEnableVertexAttribArray(attribute_coord3d);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//if (get_uniform_location("map", &uniform_map)) return 1;

	return 0;
}

void on_key_down(unsigned char key, int x, int y) {
	keys[key] = true;
}

void on_key_up(unsigned char key, int x, int y) {
	keys[key] = false;
}

const int map_size = 10;
const float* map = new float[map_size];

static struct {
	float y_pos;
	float y_velocity;
} bird = { 0, 0 };

static const float x_velocity = 0.01f;
static float x_pos = 0;

void idle() {
	
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	int size;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_obj_elements);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, nullptr);*/

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(140, 140);
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow(argv[0]);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glewExperimental = true;
	GLenum glew_status = glewInit();

	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %p\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}

	if (!init_resources()) {
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glutDisplayFunc(draw);

		glutKeyboardFunc(on_key_down);
		glutKeyboardUpFunc(on_key_up);

		glutMainLoop();
	} else {
		std::cout << "Something went wrong" << std::endl;
	}
}