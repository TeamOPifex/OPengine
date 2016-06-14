#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 v2UVredIn;
layout(location = 2) in vec2 v2UVGreenIn;
layout(location = 3) in vec2 v2UVblueIn;
  out vec2 v2UVred;
  out vec2 v2UVgreen;
  out vec2 v2UVblue;

void main()
{
	v2UVred = v2UVredIn;
	v2UVgreen = v2UVGreenIn;
	v2UVblue = v2UVblueIn;
	gl_Position = position;
}