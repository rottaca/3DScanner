#version 400


in vec3 vPos;
in mat4 mMVP;
in mat4 mMV;

void main () 
{
  	gl_Position = vec4 (vPos, 1.0);
}