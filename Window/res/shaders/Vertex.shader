#shader vertex
#version 330 core

// Modify the vecX value depending on size in buffer

// a_Position is vec2 passed in but converted to vec4
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;
//layout(location = 2) in vec2 a_TexCoord;
//layout(location = 3) in float a_TexIndex;

//out vec4 v_Colour;

out Data{
	vec4 Position;
	vec4 Colour;
} v_Data;

//out vec2 v_TexCoord;
//out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
	vec4 newPos = u_MVP * a_Position;

	v_Data.Position = a_Position;
	v_Data.Colour = a_Colour;

	/*
	vec2 local_space = VertexArray[index]

					   (X   ,   y)
	vec2 world_space = local_space * radius + position
	
	out vec4 clip_space = vec4(local_space, 0.0, 1.0);
	out vec2 local_space



	*/


	// Output variables
	//v_Colour = a_Colour;
	gl_Position = newPos;
	//v_TexCoord = a_TexCoord;
	//v_TexIndex = a_TexIndex;
};