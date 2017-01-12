#include "Skybox.h"

Shader Skybox::shader;

void Skybox::createGPUdata()
{
    int i;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}

void Skybox::create()
{
    int i;
    bool ok=0;

    shader.load("shaders/skybox/skybox.vert","shaders/skybox/skybox.frag");

    texture=loadOpenGLskybox("images/skybox.png");

    createGPUdata();
}

GLuint Skybox::loadOpenGLskybox(const char* path)
{
    sf::Image bigImage,image;
    GLuint texture;

    int i;
    bigImage.loadFromFile(path);
    int imagePosX[6]={bigImage.getSize().x/4*2,                     0,bigImage.getSize().x/4,bigImage.getSize().x/4  ,bigImage.getSize().x/4,bigImage.getSize().x/4*3};
    int imagePosY[6]={bigImage.getSize().y/3  ,bigImage.getSize().y/3,                     0,bigImage.getSize().y/3*2,bigImage.getSize().y/3,bigImage.getSize().y/3  };
    int imgSizeX=bigImage.getSize().x/4;

    for(i=0;i<6;i++)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        image.create(imgSizeX,imgSizeX);
        image.copy(bigImage,0,0,sf::IntRect(imagePosX[i],imagePosY[i],imgSizeX,imgSizeX));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    return texture;
}

void Skybox::draw()
{
    int i;

    glDepthMask(GL_FALSE);

    shader.Use();
    model=glm::eulerAngleXY(glm::radians(pitch),glm::radians(yaw));
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
    GLint projLoc = glGetUniformLocation(shader.Program, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glUniform1i(glGetUniformLocation(shader.Program, "skyboxTexture"), 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

void Skybox::handleEvents()
{
    float minDist=100.0,dist;
    bool ok=0;
    int i,j,k,kF,iF,jF;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        pitch+=2;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        pitch-=2;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        yaw-=2;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        yaw+=2;
}
