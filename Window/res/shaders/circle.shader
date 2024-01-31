#shader fragment
#version 330 core

// HAVE TO REWRITE ENTIRE CODE
// DELETE EVERYTHING

layout(location = 0) out vec4 o_Colour;

in Data{
	// This is relative
	vec4 Position;
	vec4 Colour;
} v_Data;

//in vec2 v_TexCoord;
//in float v_TexIndex;

// Window Resolution
uniform mat4 u_MVP;
uniform vec2 windowResolution;
uniform float worldCoordinate;
uniform float radius;

void main()
{
	// X, Y, Z
	// 1, 1, 0

	o_Colour = v_Data.Colour;

	//o_Colour.r = v_Data.Position.x;

	// Change from world coordinates to window coordinates

	//float ProjToWindow = windowResolution.x / worldCoordinate;
	//
	//float circleDiameter = 2 * radius * ProjToWindow;
	//float ratio = windowResolution.x / circleDiameter;

	//float scale = 2 * ratio; // Scale should change depending on zoom
	//float offset = 0; // Offset should change depending on location

	// offset value should be in terms of (x,y)

	// This creates a normalized value, uv is the world space
	//vec2 uv = gl_FragCoord.xy / windowResolution.xy * 2 ;
	//float aspect = windowResolution.x / windowResolution.y;
	//uv.x *= aspect;


	//o_Colour = vec4(vec3(1.0), 1.0);

	//bool alpha;
	//float distance = 1.0 - length(uv);

	//// We need to scale this.
	//o_Colour = vec4(1.0);

	//if (distance > 0.0) {
	//	distance = 1.0;
	//}

	//o_Colour.rgb = vec3(distance);
	//o_Colour.a = 0.0;


	//int index = int(v_TexIndex);
	//vec4 texColour = texture(u_Texture, v_TexCoord);
};