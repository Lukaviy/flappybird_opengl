#version 330 
attribute vec3 coord3d;

void main(void) {
   vec4 a = vec4(coord3d, 0);

   gl_Position = a;
} 