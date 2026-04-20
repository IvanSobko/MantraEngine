#me_define fragment 

#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_WorldPos;

uniform float u_GridSize;
uniform float u_LineWidth;
uniform vec4 u_GridColor;
uniform vec4 u_AxisColor;

float grid(vec2 pos, float size) {
    vec2 coord = pos / size;
    vec2 grid_coord = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid_coord.x, grid_coord.y);
    return 1.0 - min(line, 1.0);
}

void main()
{
    float gridValue = grid(v_WorldPos, u_GridSize);
    
    // Create thicker lines for main axes (x=0 and z=0)
    float axisX = smoothstep(0.0, u_LineWidth * 2.0, abs(v_WorldPos.x));
    float axisZ = smoothstep(0.0, u_LineWidth * 2.0, abs(v_WorldPos.y));
    
    vec4 finalColor = mix(u_AxisColor, u_GridColor, min(axisX, axisZ));
    
    color = vec4(finalColor.rgb, gridValue * finalColor.a);
    
    if (color.a < 0.01) discard;
}


#me_define vertex

#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_WorldPos;

uniform mat4 u_ViewProjection;

out vec2 v_WorldPos;

void main()
{
    v_WorldPos = a_WorldPos;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}