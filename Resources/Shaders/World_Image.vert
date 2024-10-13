#version 130

out vec2 TexCoords;

void main()
{
	TexCoords = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}