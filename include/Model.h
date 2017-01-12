#ifndef MODEL_H
#define MODEL_H

#include "Includes.h"
#include "Shader.h"
#include "Button.h"

struct Face
{
    Face(){};
    float d;
    int k,i,j;
};

class Model
{
    public:
        GLfloat yaw   = 0.0f;
        GLfloat pitch =   0.0f;
        glm::vec3 currentColor;
        glm::mat4 model;
        vector<glm::vec3>color;
        Model();
        void create(int number);
        void sendDataToGPU();
        void handleEvents();
        void draw();
        void generateLevel();
        GLuint loadOpenGLtexture(const char* path);
        GLuint loadOpenGLskybox(const char* path);

    private:
        Shader shader;
        GLuint blankSquareTexture,startFinishSquareTexture,skyboxTexture,wallSquareTexture;
        int nrVertexComponents,nrSquares,n;
        vector<vector<GLfloat> >vertex;
        vector<GLuint>VAO;
        vector<GLuint>VBO;
        vector<vector<int> >neighbour;
        vector<bool>use;
        vector<bool>bigUse;
        vector<int>sol;
        vector<int>poz;

        glm::vec4 getMouseRay();
        void createGPUdata();
        void calculateNeighbours();
        void calculateVertexData();
        void setColors();
        bool generatePath(int lgPath,int colorInd);
        void changeTriangleColor(int nr,glm::vec3 color);
        void changeTriangleType(int number,float type,glm::vec3 color=glm::vec3(0.0,0.0,0.0));
        bool intersect_triangle(glm::vec3 orig, glm::vec3 dir, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2);
        float getSquareType(int nr);
        glm::vec3 getSquareColor(int nr);
        glm::vec3 numberToPoint(int number);
        void createWall();
        bool solvedLevel();
        void dfs(int k,int start,glm::vec3 squareColor,bool &found);
};

bool compareFace(Face i,Face j);

#endif // MODEL_H
