#include "Includes.h"
#include "Shader.h"
#include "Button.h"
#include "Skybox.h"
#include "Model.h"

using namespace std;

void loadResources();
void setupOpenGL();

sf::Event event;
sf::RenderWindow window;

glm::mat4 view;
glm::mat4 projection;

glm::vec3 center=glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront=glm::vec3(0.0f, 0.0f,-1.0f);
glm::vec3 cameraUp=glm::vec3(0.0f, 1.0f,0.0f);
glm::vec3 modelDistance=glm::vec3(0.0,0.0,2.0);

int screen=0;

Button easyButton,mediumButton,hardButton,backButton,nextlevelButton,changedifficultyButton,clearButton,impossibleButton;

int main()
{
    setupOpenGL();
    loadResources();

    Model cube;
    Skybox skybox;

    skybox.create();

    bool running=true;
    int currentDifficulty=0;
    while(running)
    {
        projection=glm::perspective(45.0f,(GLfloat)WIDTH/(GLfloat)HEIGHT,0.1f,100.0f);

        while(window.pollEvent(event))
            if(event.type==sf::Event::Closed)
                running=false;

        if(screen==0)
        {
            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(Button::isButtonPressed(easyButton))
                {
                    screen=1;
                    currentDifficulty=2;
                    cube.create(currentDifficulty);
                }
                if(Button::isButtonPressed(mediumButton))
                {
                    screen=1;
                    currentDifficulty=3;
                    cube.create(currentDifficulty);
                }
                if(Button::isButtonPressed(hardButton))
                {
                    screen=1;
                    currentDifficulty=4;
                    cube.create(currentDifficulty);
                }
                if(Button::isButtonPressed(impossibleButton))
                {
                    screen=1;
                    currentDifficulty=5;
                    cube.create(currentDifficulty);
                }
            }
            easyButton.draw();
            mediumButton.draw();
            hardButton.draw();
            impossibleButton.draw();

            view=glm::lookAt(center,center+cameraFront,cameraUp);
            skybox.draw();
            window.display();
        }
        if(screen==1)
        {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(Button::isButtonPressed(backButton))
                {
                    skybox.pitch=0.0;
                    skybox.yaw=0.0;
                    screen=0;
                }
                if(Button::isButtonPressed(clearButton))
                    cube.clearColors();
            }

            cube.handleEvents();
            skybox.handleEvents();

            backButton.draw();
            clearButton.draw();

            view=glm::lookAt(center,center+cameraFront,cameraUp);
            skybox.draw();

            view=glm::lookAt(center+modelDistance,center+modelDistance+cameraFront,cameraUp);
            cube.draw();
            window.display();
        }
        if(screen==2)
        {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(Button::isButtonPressed(nextlevelButton))
                {
                    cube.pitch=0.0;
                    cube.yaw=0.0;
                    skybox.pitch=0.0;
                    skybox.yaw=0.0;
                    screen=1;
                    cube.create(currentDifficulty);
                }
                if(Button::isButtonPressed(changedifficultyButton))
                {
                    cube.pitch=0.0;
                    cube.yaw=0.0;
                    skybox.pitch=0.0;
                    skybox.yaw=0.0;
                    screen=0;
                }
            }

            changedifficultyButton.draw();
            nextlevelButton.draw();

            view=glm::lookAt(center,center+cameraFront,cameraUp);
            skybox.draw();

            view=glm::lookAt(center+modelDistance,center+modelDistance+cameraFront,cameraUp);
            cube.draw();

            window.display();
        }
    }
    return 0;
}

void loadResources()
{
    easyButton.create("images/easyButton.png",300,50);
    mediumButton.create("images/mediumButton.png",300,200);
    hardButton.create("images/hardButton.png",300,350);
    impossibleButton.create("images/impossibleButton.png",300,500);

    backButton.create("images/backButton.png",10,10);
    clearButton.create("images/clearButton.png",690,10);
    nextlevelButton.create("images/nextlevelButton.png",500,490);
    changedifficultyButton.create("images/changedifficultyButton.png",100,490);

}

void setupOpenGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    window.create(sf::VideoMode(WIDTH, HEIGHT), "Walking on a cube", sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    srand(static_cast<unsigned int>(time(0)));

}
