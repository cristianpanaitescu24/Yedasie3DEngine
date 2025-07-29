#version 430

in vec2 v_TexCoords;
in vec3 v_Position;
in vec3 v_Normal;

uniform sampler2D u_Texture;

uniform mat4 u_ModelMatrix;
uniform vec3 u_LightPosition;
uniform vec3 u_EyePosition;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform float u_Shininess;

out vec4 fragColor;

void main()
{
	vec3 worldPos = (u_ModelMatrix * vec4(v_Position, 1)).xyz;
	vec3 worldNormal = normalize( mat3(u_ModelMatrix) * v_Normal);
	vec3 L = normalize(u_LightPosition - worldPos);
	vec3 V = normalize(u_EyePosition - worldPos);

	float pr = (dot(worldNormal, L) > 0) ? 1.0 : 0.0;

	vec3 H = normalize(L + V);

	vec3 A = u_Ka;
	vec3 D = u_Kd * max(dot(worldNormal, L), 0);
	vec3 S = u_Ks * pr * pow((max(dot(worldNormal, H), 0)), u_Shininess);
	vec3 SpecularColor = vec3(0);
	vec3 VertexToEye = normalize(u_EyePosition - worldPos);
	vec3 LightReflect = normalize(reflect(normalize(worldPos - u_LightPosition), worldNormal));
	float SpecularFactor = dot(VertexToEye, LightReflect);
	if (SpecularFactor > 0) {
		SpecularFactor = pow(SpecularFactor, u_Shininess);
		SpecularColor = vec3(SpecularFactor);
	}
	
	vec3 light = A + SpecularColor + D;

	fragColor = vec4(light, 1.0) * texture2D(u_Texture, v_TexCoords);
}
