#version 120
uniform vec3 lightPos[1];
uniform vec3 camaraPos;
uniform float density;

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

	//计算雾因子
	const float e = 2.71828;
	float fogFactor = density * length(V);
	fogFactor *= fogFactor;
	fogFactor = clamp(pow(e, -fogFactor), 0.0, 1.0);

	const vec4 fogColor = vec4(0.5, 0.8, 0.5, 1.0);
	//把雾颜色和 光的颜色 根据雾因子进行混合
	gl_FrontColor = mix(fogColor, clamp(diffuse + specular, 0.0, 1.0), fogFactor);
}
