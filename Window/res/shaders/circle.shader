#shader fragment
#version 330 core

in vec4 v_Position;

out vec4 o_Colour;

//in vec2 v_TexCoord;
//in float v_TexIndex;

// Window Resolution
uniform vec2 iResolution;
uniform float radius;

void main()
{
	// This value is in respect to screen resolution

	// NONE OF THIS IS RELATIVE TO A PARTICLE.
	// THIS IS ALL RELATIVE TO THE WINDOW.

	// let radius = 1;
	float ProjToWindow = iResolution.x / 800.0f;

	float circleDiameter = 2 * radius * ProjToWindow;

	float ratio = iResolution.x / circleDiameter;

	float scale = 2 * ratio; // Scale should change depending on zoom
	float offset = -scale / 2 ; // Offset should change depending on location

	// This creates a normalized value, uv is the world space
	vec2 uv = gl_FragCoord.xy / iResolution.xy * scale + offset;
	float aspect = iResolution.x / iResolution.y;
	uv.x *= aspect;


	//o_Colour = vec4(vec3(1.0), 1.0);

	bool alpha;
	float distance = 1.0 - length(uv);

	// We need to scale this.
	o_Colour.rg = uv;

	if (distance > 0.0) {
		distance = 1.0;
	}

	o_Colour.rgb = vec3(distance);
	//o_Colour.a = 0.0;


	//int index = int(v_TexIndex);
	//vec4 texColour = texture(u_Texture, v_TexCoord);
};