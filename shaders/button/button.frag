#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	color = texture(ourTexture1, TexCoord);
}
