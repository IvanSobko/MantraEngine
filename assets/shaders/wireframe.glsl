#me_define fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_Barycentric;

void main()
{
	// Edge detection using barycentric coordinates
	vec3 bary = v_Barycentric;
	float minDist = min(bary.x, min(bary.y, bary.z));
	float edge = step(0.02, minDist);
	
	vec3 wireColor = vec3(0.0);  // Black wireframe
	vec3 fillColor = vec3(1.0);  // White fill
	
	vec3 finalColor = mix(wireColor, fillColor, edge);
    float alpha = mix(1.0, 0.0, edge);
	color = vec4(finalColor, alpha);
}

#me_define vertex

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_Barycentric;

void main()
{
	v_Normal = mat3(u_Transform) * a_Normal;
	
	// Simple barycentric: cycle through (1,0,0), (0,1,0), (0,0,1)
	// This is imperfect but good enough for visualization
	int vertexID = gl_VertexID % 3;
	v_Barycentric = vec3(0.0);
	v_Barycentric[vertexID] = 1.0;
	
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
