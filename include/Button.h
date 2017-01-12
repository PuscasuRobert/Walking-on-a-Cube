#ifndef BUTTON_H
#define BUTTON_H

#include "Includes.h"
#include "Shader.h"

class Button
{
    public:
        static Shader buttonShader;
        static bool loaded;

        GLfloat vertex[20];
        GLuint indices[6];

        GLuint VAO,VBO,EBO,backgroundTexture;
        int imgX,imgY,imgW,imgH;
        Button();
        void loadTexture(string path);
        void setPosition(int x,int y);
        void create(string path,int x,int y);
        void draw();
        static bool isButtonPressed(Button b);
};

#endif // BUTTON_H
