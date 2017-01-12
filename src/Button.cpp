#include "Button.h"

Shader Button::buttonShader;
bool Button::loaded=false;

Button::Button(){
};

void Button::loadTexture(string path)
{
    glGenTextures(1, &backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    sf::Image image;
    image.loadFromFile(path);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glBindTexture(GL_TEXTURE_2D, 0);

    imgW=image.getSize().x;
    imgH=image.getSize().y;
    imgX=0;imgY=0;
}

void Button::setPosition(int x,int y)
{
    imgX=x;
    imgY=y;
}

void Button::create(string path,int x,int y)
{

    if(loaded==false)
    {
        buttonShader.load("shaders/button/button.vert", "shaders/button/button.frag");
        loaded=true;
    }
    loadTexture(path);
    setPosition(x,y);
    indices[0]=0;
    indices[1]=1;
    indices[2]=2;
    indices[3]=1;
    indices[4]=2;
    indices[5]=3;

    glm::vec2 p;

    p.x=imgX;
    p.y=imgY;
    vertex[ 0]=(2.0f * p.x) / WIDTH - 1.0f;
    vertex[ 1]=1.0f - (2.0f * p.y) / HEIGHT;
    vertex[ 2]=0.0;
    vertex[ 3]=0.0;
    vertex[ 4]=1.0;

    p.x=imgX;
    p.y=imgY+imgH;
    vertex[ 5]=(2.0f * p.x) / WIDTH - 1.0f;
    vertex[ 6]=1.0f - (2.0f * p.y) / HEIGHT;
    vertex[ 7]=0.0;
    vertex[ 8]=0.0;
    vertex[ 9]=0.0;

    p.x=imgX+imgW;
    p.y=imgY;
    vertex[10]=(2.0f * p.x) / WIDTH - 1.0f;
    vertex[11]=1.0f - (2.0f * p.y) / HEIGHT;
    vertex[12]=0.0;
    vertex[13]=1.0;
    vertex[14]=1.0;

    p.x=imgX+imgW;
    p.y=imgY+imgH;
    vertex[15]=(2.0f * p.x) / WIDTH - 1.0f;
    vertex[16]=1.0f - (2.0f * p.y) / HEIGHT;
    vertex[17]=0.0;
    vertex[18]=1.0;
    vertex[19]=0.0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Button::draw()
{
    Button::buttonShader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glUniform1i(glGetUniformLocation(buttonShader.Program, "ourTexture1"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool Button::isButtonPressed(Button b)
{
    int mx,my;
    mx=sf::Mouse::getPosition(window).x;
    my=sf::Mouse::getPosition(window).y;
    if(mx>=b.imgX&&mx<=b.imgX+b.imgW&&my>=b.imgY&&my<=b.imgY+b.imgH)
        return true;
    return false;
}
