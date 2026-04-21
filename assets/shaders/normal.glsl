#me_define fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;

void main()
{
	// Visualize normals as color: normalize to [-1, 1] then shift to [0, 1]
	vec3 normalColor = normalize(v_Normal) * 0.5 + 0.5;
	color = vec4(normalColor, 1.0);
}

#me_define vertex

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;

void main()
{
	// Transform normal to world space (simple approach: assume uniform scaling)
	v_Normal = mat3(u_Transform) * a_Normal;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
