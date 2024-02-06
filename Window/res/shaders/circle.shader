#shader vertex
#version 460 core
// ----------------------
//
//	Circle Vertex shader 
//
// ----------------------

// Inputs
layout(location = 0) in vec4 a_WorldPosition;
layout(location = 1) in vec4 a_LocalPosition;
layout(location = 2) in vec4 a_Colour;

//out vec4 v_Colour;

struct VertexOutput{
	vec4 LocalPosition;
	vec4 Colour;
};

// Outputs
layout(location = 0) out VertexOutput Output;

void main()
{
	// Setting output values
	Output.LocalPosition = a_LocalPosition;
	Output.Colour = a_Colour;

	gl_Position = a_WorldPosition;
};



#shader fragment
#version 460 core

// -----------------------------------
// 
// Circle Fragment Shader
// 
// -----------------------------------

// Outputs
layout(location = 0) out vec4 o_Colour;

struct VertexOutput {
	vec4 LocalPosition;
	vec4 Colour;
};

// Inputs
layout(location = 0) in VertexOutput Input;

// Window Resolution
uniform mat4 u_MVP;
uniform vec2 windowResolution;
uniform float worldCoordinate;
uniform float radius;

void main()
{
	float distance = 1 - length(Input.LocalPosition.xyz);
	float circle = smoothstep(0.0, 0.1, distance);

	o_Colour = Input.Colour;
	o_Colour.a *= circle;

};