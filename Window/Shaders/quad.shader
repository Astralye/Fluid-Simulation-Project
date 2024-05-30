#shader vertex
#version 410 core

// ----------------------
//
//	Quad Vertex shader 
//
// ----------------------

// Inputs
layout(location = 0) in vec4 a_WorldPosition;
layout(location = 1) in vec4 a_Colour;

// Outputs
layout(location = 0) out vec4 v_Colour;

void main()
{
	gl_Position = a_WorldPosition;
	v_Colour = a_Colour;
};



#shader fragment
#version 410 core

// -----------------------------------
// 
// Quad Fragment Shader
// 
// -----------------------------------


// Outputs
layout(location = 0) out vec4 o_Colour;

// Inputs
layout(location = 0) in vec4 v_Colour;

// Window Resolution

void main()
{
	o_Colour = v_Colour;
};