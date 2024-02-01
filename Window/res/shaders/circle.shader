// ----------------------
//
//	Circle Vertex shader 
//
// ----------------------

#shader vertex
#version 330 core

// Inputs
layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Colour;

//out vec4 v_Colour;

struct VertexOutput{
	vec3 LocalPosition;
	vec4 Colour;
};

// Outputs
layout(location = 0) out VertexOutput Output;

uniform mat4 u_MVP;

void main()
{
	Output.LocalPosition = a_LocalPosition;
	Output.Colour = a_Colour;

	// Output variables
	//v_Colour = a_Colour;
	gl_Position = u_MVP * vec4(a_WorldPosition);
	//v_TexCoord = a_TexCoord;
	//v_TexIndex = a_TexIndex;
};

// -----------------------------------
// 
// Circle Fragment Shader
// 
// -----------------------------------

#shader fragment
#version 330 core

// Outputs
layout(location = 0) out vec4 o_Colour;

struct VertexOutput {
	vec3 LocalPosition;
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
	float distance = 1.0 - length(Input.LocalPosition);
	vec3 colour = vec3(smoothstep(0.0, 0.005, distance));

	o_Colour = vec4(colour, 1.0);
	o_Colour.rgb = Input.Colour;
};