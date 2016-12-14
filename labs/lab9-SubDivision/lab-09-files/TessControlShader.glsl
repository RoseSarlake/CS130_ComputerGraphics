#version 430

layout(vertices = 4) out;
uniform float TessLevelInner;
uniform float TessLevelOuter;

void main(void)
{
    gl_TessLevelInner[0] = 15;
    gl_TessLevelOuter[0] = 10;
    gl_TessLevelOuter[1] = 10;
    gl_TessLevelOuter[2] = 10;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    
}
//the second number controls number of triangles

