#version 120
uniform vec3 lightPos[1];
uniform vec3 camaraPos;

void main(void)
{
	//MVP transform
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec3 L = normalize(lightPos[0] - V.xyz);
	float NdotL = dot(N, L);
	vec4 diffuse = max(0.0, NdotL) * gl_Color;

	const float expose = 128.0;
	vec3 H = normalize(L + normalize(camaraPos));
	float NdotH = 0.0;
	if (NdotL > 0.0)
		NdotH = max(0.0, dot(N, H));
	vec4 specular = vec4(pow(NdotH, expose));
	gl_FrontColor = diffuse + specular;

	//计算雾坐标
	gl_FogFragCoord = length(V);
}
