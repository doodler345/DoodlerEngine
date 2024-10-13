
#version 140

in vec2 TexCoords;
uniform sampler2D tex;

void main()
{
	gl_FragColor = texture2D(tex, TexCoords);
}



/*
const int MY_ARRAY_SIZE = 512;

layout (std140) uniform myArrayBlock {
  int myArray [MY_ARRAY_SIZE];
};

void main (void) {
  gl_FragColor = vec4 (float(myArray[0]) * 0.1, vec3 (1.0));
}
*/