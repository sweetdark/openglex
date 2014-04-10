#version 120
uniform vec3 lightPos;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	//计算散射光
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec3 L = normalize(lightPos - V.xyz);
	float NdotL = max(0.0, dot(N,L));
	gl_FrontColor = vec4(NdotL) * gl_Color;

	//计算点的大小
	float pointSize = length(V);
	pointSize *= pointSize;
	gl_PointSize = clamp(1000000000.0/pointSize, 1.0, 7.0);
}
