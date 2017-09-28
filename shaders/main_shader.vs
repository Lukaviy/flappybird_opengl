#version 330

attribute vec3 coord3d;
uniform mat4 projection;
uniform mat4 model;

void main(void) {
   gl_Position = projection * model * vec4(coord3d, 1.0);
}
