#version 120
uniform vec3 lightPos;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//caculate diffuse
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec4 V = (gl_ModelViewMatrix * gl_Vertex);
	vec3 L = normalize(lightPos - V.xyz);
	float NdotL = dot(N,L);
	vec4 diffuse = vec4(max(NdotL, 0.0)) * gl_Color;

	//specular
	vec3 H = normalize(vec3(0.0, 0.0, 1.0) + L);
	float NdotH = max(0.0, dot(N,H));
	const float expose = 128.0;
	vec4 specular = vec4(0.0);
	if (NdotL > 0.0)
		specular = vec4(pow(NdotH, expose));

	gl_FrontColor = diffuse + specular;
}
