// ----------------------
//
//	Quad Vertex shader 
//
// ----------------------

#shader vertex
#version 330 core

// Inputs
layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec4 a_Colour;

// Outputs
layout(location = 0) out vec4 v_Colour;

uniform mat4 u_MVP;

void main()
{
	vec4 pos = vec4(a_WorldPosition, 1.0);
	v_Colour = a_Colour;
	//gl_Position = u_MVP * vec4(a_WorldPosition, 1.0);
};


// -----------------------------------
// 
// Quad Fragment Shader
// 
// -----------------------------------

#shader fragment
#version 330 core

// Outputs
layout(location = 0) out vec4 o_Colour;

// Inputs
layout(location = 0) in vec4 v_Colour;

// Window Resolution

void main()
{
	o_Colour = v_Colour;
};