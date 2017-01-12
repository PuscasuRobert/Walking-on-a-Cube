#ifndef INCLUDES_H
#define INCLUDES_H

using namespace std;

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>


#define WIDTH 800
#define HEIGHT 600

class Shader;

extern sf::RenderWindow window;
extern glm::mat4 projection,view;

class Button;
extern Button easyButton,mediumButton,hardButton,backButton;

extern int screen;

#endif // INCLUDES_H
