#version 330 core
in vec3 Color;
in vec3 Normal;
in vec3 Position;

in vec2 TexCoord;
in float Type;

out vec4 color;

uniform samplerCube skyboxTexture;
uniform sampler2D blankSquareTexture;
uniform sampler2D startFinishSquareTexture;
uniform sampler2D wallSquareTexture;

void main()
{
    color=vec4(0.0,0.0,0.0,1.0);
    if(Type==0.0)
    {
        vec3 I = normalize(Position);
        vec3 R = reflect(I, normalize(Normal));
        if(texture(blankSquareTexture, TexCoord)==vec4(1.0,1.0,1.0,1.0))
            color=mix(texture(skyboxTexture, R),vec4(1.0,1.0,1.0,1.0),0.3);
    }
    else if(Type==1.0)
    {
        if(texture(startFinishSquareTexture, TexCoord)==vec4(1.0,1.0,1.0,1.0))
            color=vec4(Color,1.0);
    }
    else if(Type==-1.0)
    {
        if(texture(wallSquareTexture, TexCoord)==vec4(0.0,0.0,0.0,1.0))
            color=vec4(0.6,0.6,0.6,1.0);
    }
    else if(Type==2.0)
    {
        if(texture(blankSquareTexture, TexCoord)==vec4(1.0,1.0,1.0,1.0))
            color=mix(vec4(Color,1.0),vec4(1.0,1.0,1.0,1.0),0.3);
    }
}

/*
Type==-1 ---> wall square
Type== 0 ---> blank square(reflects light)
Type== 1 --->  start/finish square
Type== 2 ---> colored square that was once a black square
*/
