#version 120
uniform vec3 lightPos[3];
varying vec4 gl_TexCoord[3];
uniform vec3 camaraPos;

void main(void)
{
	//MVP transform
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//经过视图变换后的点
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec3 N[3], L[3], H[3];
	gl_FrontColor = vec4(0.0);
	for (int i = 0; i < 3; ++i)
	{
		N[i] = normalize(gl_NormalMatrix * gl_Normal);
		L[i] = normalize(lightPos[i] - V.xyz);
		float NdotL = dot(N[i], L[i]);
		//accumalte diffuse light
		gl_FrontColor += vec4(max(0.0, NdotL)) * gl_Color;

		//指向光源的向量，与指向照相机的向量的。半角向量。
		H[i] = normalize(L[i] + normalize(camaraPos));
		float NdotH = 0.0;
		if (NdotL > 0.0)
			NdotH = max(0.0, dot(N, H) * 8.0 - 7.0);
		gl_TexCoord[i] = vec4(NdotH, 0.0, 0.0, 1.0);

	}
	
}
