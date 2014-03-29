#version 120

uniform vec3 lightPos;
uniform vec4 cLight;
vec4 GetDiffuse(const vec3 N, const vec3 L, const vec4 cMat, const vec4 cLit)
{
	vec3 NdotL = normalize(dot(N,L));
	vec4 cDiff = cMat * cLit * vec4(max(0.0, NdotL));
	return cDiff;
}

vec4 GetSpecular(const vec3 N, const vec3 H, const vec4 cMat, const vec4 cLig, const float exp)
{
	float NdotH = max(dot(N, H), 0);
	vec4 specular = vec4(pow(NdotH, exp));
	return specular;
}

void main(void)
{
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec3 L = normalize(lightPos - V.xyz);
	gl_FrontColor = GetDiffuse(N, L, gl_Color, cLight);
}
