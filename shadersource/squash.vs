#version 120

uniform vec3 lightPos;
uniform float squashStrech;

void main(void)
{
	vec4 squashV = gl_Vertex;
	squashV.xyz *= squashStrech;
	//MVP transform
	gl_Position = gl_ModelViewProjectionMatrix * squashV;
	vec3 squashN = gl_Normal * vec3(squashStrech);
	vec3 N = normalize(gl_NormalMatrix * squashN);
	vec4 V = gl_ModelViewMatrix * squashV;
	vec3 L = normalize(lightPos - V.xyz);
	float NdotL = dot(N, L);
	//diffuse
	gl_FrontColor = vec4(max(0.0, NdotL)) * gl_Color;

	vec3 H = normalize(L + vec3(0.0, 0.0, 1.0));
	float NdotH = max(0.0, dot(N, H));
	vec4 specular = vec4(0.0);
	if (NdotL > 0.0)
		specular = vec4(max(pow(NdotH, 128.0), 0.0));
	gl_FrontColor += specular;
}
