#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include "engine/game_core.h"

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

GLuint vbo_bird_vertices;
GLuint vbo_tube_vertices;
GLuint vbo_cube_vertices;
GLuint ibo_obj_elements;
GLuint vao_id;

GLint attribute_coord3d;
GLint uniform_model;
GLint uniform_projection;
GLint uniform_color;

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
	
	GLfloat bird_vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	GLfloat tube_vertices[] = {
		-0.5f, 0.f, 0.f,
		0.5f, 2.f, 0.f,
		-0.5f, 2.f, 0.f,
		-0.5f, 0.f, 0.f,
		0.5f, 0.f, 0.f,
		0.5f, 2.f, 0.f
	};

	GLfloat cube_vertices[] = {
		-0.5f, -0.5f, 0.f,
		0.5f, 0.5f, 0.f,
		-0.5f, 0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.5f, 0.5f, 0.f
	};

	/*
	GLshort indeces[] = {
		0, 1, 2,
		0, 1, 3,
		0, 2, 3,
		1, 2, 3
	};*/

	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	glGenBuffers(1, &vbo_bird_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_bird_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bird_vertices), bird_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_tube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tube_vertices), tube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	/*
	glGenBuffers(1, &ibo_obj_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_obj_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	*/

	if (get_attrib_location("coord3d", &attribute_coord3d)) return 1; 
	if (get_uniform_location("model", &uniform_model)) return 1;
	if (get_uniform_location("projection", &uniform_projection)) return 1;
	if (get_uniform_location("color", &uniform_color)) return 1;

	return 0;
}

void on_key_down(unsigned char key, int x, int y) {
	keys[key] = true;
}

void on_key_up(unsigned char key, int x, int y) { 
	keys[key] = false;
}

const int map_size = 3;
const float dist_between_tubes = 0.9f;
const float tube_width = 0.4f;

GameCore_t<map_size> game_core(0.1f, -0.05f, 0.3f, dist_between_tubes, tube_width, 0.5f, -2.f, 0.08f, 0.08f);

int last_time_since_update = 0;
void idle() {
	int time = glutGet(GLUT_ELAPSED_TIME);
	float dtime = (time - last_time_since_update) / 100.f;
	last_time_since_update = time;

	game_core.step(dtime, keys[' ']);

	glutPostRedisplay();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	int size;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_obj_elements);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, nullptr);*/

	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_bird_vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	auto bv = game_core.get_bird_velocity();

	float velocity = length(glm::vec2(bv.x, bv.y));
	float cos_b = bv.y / velocity;
	float sin_b = bv.x / velocity;

	auto rotate_matrix = glm::mat4x4(
		cos_b,  -sin_b, 0,    0,
		sin_b,  cos_b,  0,    0,
		0,      0,      1,    0,
		0,      0,      0,    1
	);

	auto bird_pos = game_core.get_bird_pos();

	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &glm::mat4x4()[0][0]);
	//glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &glm::ortho(-2.f, 2.f, -2.f, 2.f)[0][0]);
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &(scale(glm::translate(glm::mat4x4(), glm::vec3(0.f, bird_pos.y, 0.0f)), glm::vec3(0.1f, 0.1f, 0.1f)) * rotate_matrix)[0][0]);

	glUniform3f(uniform_color, 1, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_tube_vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glUniform3f(uniform_color, 0, 1, 0);

	auto map = game_core.get_map();
	int curr_ceil = game_core.get_current_ceil();

	for (int i = 0; i < map_size; i++) {
 		auto tube = map[(curr_ceil + i) % map_size];

		float hole_size = tube.hole_size;
		float hole_pos = tube.y_pos;
		float x_pos = tube.x_pos - bird_pos.x;
		auto up_tube = glm::scale(
			glm::translate(glm::mat4x4(), glm::vec3(x_pos, hole_pos + hole_size / 2, 0.f)),
			glm::vec3(tube_width, 1.f, 1.f)
		);
		auto bottom_tube = glm::scale(
			glm::translate(glm::mat4x4(), glm::vec3(x_pos, hole_pos - hole_size / 2, 0.f)),
			glm::vec3(tube_width, -1.f, 1.f)
		);
		
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &up_tube[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &bottom_tube[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisableVertexAttribArray(attribute_coord3d);

	const unsigned char text[] = "asd";

	glutBitmapString(GLUT_BITMAP_9_BY_15, text);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(140, 140);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow(argv[0]);

	glewExperimental = true;
	GLenum glew_status = glewInit();

	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %p\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}

	if (!init_resources()) {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glutDisplayFunc(draw);
		glutIdleFunc(idle);

		glutKeyboardFunc(on_key_down);
		glutKeyboardUpFunc(on_key_up);

		glutMainLoop();
	} else {
		std::cout << "Something went wrong" << std::endl;
	}
}