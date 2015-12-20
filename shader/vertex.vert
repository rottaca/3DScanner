#version 400

in vec4 vPos;
in vec4 vCol;

varying vec4 vColOut;

uniform mat4 mMVP;
uniform mat4 mModelMatrix;

void main() 
{
	vColOut = vCol;
  	gl_Position = mMVP*mModelMatrix*vPos;
}