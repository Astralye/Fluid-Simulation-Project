#shader vertex
#version 330 core

// Modify the vecX value depending on size in buffer
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;
//layout(location = 2) in vec2 a_TexCoord;
//layout(location = 3) in float a_TexIndex;

out vec4 v_Colour;
//out vec2 v_TexCoord;
//out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * a_Position;
	v_Colour = a_Colour;
	//v_TexCoord = a_TexCoord;
	//v_TexIndex = a_TexIndex;
};