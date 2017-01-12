#include "Model.h"

Model::Model()
{

}

void Model::create(int number)
{
    n=number;
    nrSquares=6*n*n;

    createGPUdata();
    calculateVertexData();
    sendDataToGPU();
    calculateNeighbours();

    setColors();
    shader.load("shaders/default/default.vert","shaders/default/default.frag");
    startFinishSquareTexture=loadOpenGLtexture("images/center.png");
    blankSquareTexture=loadOpenGLtexture("images/border.png");
    wallSquareTexture=loadOpenGLtexture("images/wall.png");
    skyboxTexture=loadOpenGLskybox("images/skybox.png");

    generateLevel();
}

void Model::createGPUdata()
{
    int i;

    vertex.resize(6);
    VAO.resize(6);
    VBO.resize(6);
    nrVertexComponents=12;

    for(i=0;i<vertex.size();i++)
    {
        glDeleteVertexArrays(1,&VAO[i]);
        glGenVertexArrays(1, &VAO[i]);
        glGenBuffers(1, &VBO[i]);

        glBindVertexArray(VAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, vertex[i].size()*sizeof(GLfloat), &vertex[i][0] , GL_STATIC_DRAW);

        glBindVertexArray(0); // Unbind VAO
    }
}

void Model::calculateVertexData()
{
    int i,j,k,ind;
    float x,y,z;
    bool tx=0;
    float normal=-1.0;
    float lgCube=1.0;


    for(i=0;i<6;i++)
        vertex[i].resize(nrSquares*nrVertexComponents);

    for(k=0,z=-lgCube/2;z<=lgCube/2;z+=lgCube,k++)
    {
        ind=0;
        for(x=-lgCube/2,i=0;i<n;i++)
            for(y=-lgCube/2,j=0;j<n;j++)
            {
                vertex[k][ind++]=x+j*lgCube/n;vertex[k][ind++]=y+i*lgCube/n;vertex[k][ind++]=z;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=tx;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=normal;

                vertex[k][ind++]=x+(j+1)*lgCube/n;vertex[k][ind++]=y+i*lgCube/n;vertex[k][ind++]=z;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=!tx;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=normal;

                vertex[k][ind++]=x+j*lgCube/n;vertex[k][ind++]=y+(i+1)*lgCube/n;vertex[k][ind++]=z;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=tx;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=normal;

                vertex[k][ind++]=x+(j+1)*lgCube/n;vertex[k][ind++]=y+i*lgCube/n;vertex[k][ind++]=z;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=!tx;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=normal;

                vertex[k][ind++]=x+(j+1)*lgCube/n;vertex[k][ind++]=y+(i+1)*lgCube/n;vertex[k][ind++]=z;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=!tx;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=normal;

                vertex[k][ind++]=x+j*lgCube/n;vertex[k][ind++]=y+(i+1)*lgCube/n;vertex[k][ind++]=z;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=tx;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=normal;
            }
        tx=!tx;
        normal=-normal;
    }
    for(k=2,x=-lgCube/2;x<=lgCube/2;x+=lgCube,k++)
    {
        ind=0;
        for(z=-lgCube/2,i=0;i<n;i++)
            for(y=-lgCube/2,j=0;j<n;j++)
            {
                vertex[k][ind++]=x;vertex[k][ind++]=y+j*lgCube/n;vertex[k][ind++]=z+i*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=!tx;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=normal;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x;vertex[k][ind++]=y+j*lgCube/n;vertex[k][ind++]=z+(i+1)*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=tx;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=normal;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x;vertex[k][ind++]=y+(j+1)*lgCube/n;vertex[k][ind++]=z+i*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=!tx;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=normal;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x;vertex[k][ind++]=y+j*lgCube/n;vertex[k][ind++]=z+(i+1)*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=tx;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=normal;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x;vertex[k][ind++]=y+(j+1)*lgCube/n;vertex[k][ind++]=z+(i+1)*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=1.0;
                vertex[k][ind++]=tx;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=normal;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x;vertex[k][ind++]=y+(j+1)*lgCube/n;vertex[k][ind++]=z+i*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=!tx;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=normal;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
            }
        tx=!tx;
        normal=-normal;
    }
    for(k=4,y=-lgCube/2;y<=lgCube/2;y+=lgCube,k++)
    {
        ind=0;
        for(x=-lgCube/2,i=0;i<n;i++)
            for(z=-lgCube/2,j=0;j<n;j++)
            {
                vertex[k][ind++]=x+j*lgCube/n;vertex[k][ind++]=y;vertex[k][ind++]=z+i*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=normal;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x+j*lgCube/n;vertex[k][ind++]=y;vertex[k][ind++]=z+(i+1)*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=1.0;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=normal;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x+(j+1)*lgCube/n;vertex[k][ind++]=y;vertex[k][ind++]=z+i*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=normal;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x+j*lgCube/n;vertex[k][ind++]=y;vertex[k][ind++]=z+(i+1)*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=1.0;vertex[k][ind++]=1.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=normal;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x+(j+1)*lgCube/n;vertex[k][ind++]=y;vertex[k][ind++]=z+(i+1)*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=1.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=normal;vertex[k][ind++]=0.0;

                vertex[k][ind++]=x+(j+1)*lgCube/n;vertex[k][ind++]=y;vertex[k][ind++]=z+i*lgCube/n;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;
                vertex[k][ind++]=0.0;vertex[k][ind++]=normal;vertex[k][ind++]=0.0;
            }
        tx=!tx;
        normal=-normal;
    }
}

void Model::sendDataToGPU()
{
    int i;

    for(i=0;i<vertex.size();i++)
    {
        glBindVertexArray(VAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, vertex[i].size()*sizeof(GLfloat), &vertex[i][0] , GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, nrVertexComponents * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, nrVertexComponents * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, nrVertexComponents * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, nrVertexComponents * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, nrVertexComponents * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
        glEnableVertexAttribArray(4);

        glBindVertexArray(0); // Unbind VAO
    }
}


void Model::calculateNeighbours()
{
    int i,j,k,a,b,c;
    glm::vec3 c1,c2;
    vector<Face>dist;

    neighbour.resize(nrSquares);
    for(i=0;i<neighbour.size();i++)
        neighbour[i].resize(8);

    for(k=0;k<6;k++)
        for(i=0;i<n;i++)
            for(j=0;j<n;j++)
            {
                int ind1=6*nrVertexComponents*(i*n+j);
                glm::vec3 v0= glm::vec3(vertex[k][ind1+0*nrVertexComponents+ 0],vertex[k][ind1+0*nrVertexComponents+ 1],vertex[k][ind1+0*nrVertexComponents+ 2]);
                glm::vec3 v1= glm::vec3(vertex[k][ind1+1*nrVertexComponents+ 0],vertex[k][ind1+1*nrVertexComponents+ 1],vertex[k][ind1+1*nrVertexComponents+ 2]);
                glm::vec3 v2= glm::vec3(vertex[k][ind1+2*nrVertexComponents+ 0],vertex[k][ind1+2*nrVertexComponents+ 1],vertex[k][ind1+2*nrVertexComponents+ 2]);
                glm::vec3 v3= glm::vec3(vertex[k][ind1+4*nrVertexComponents+ 0],vertex[k][ind1+4*nrVertexComponents+ 1],vertex[k][ind1+4*nrVertexComponents+ 2]);

                c1=v0+v1+v2+v3;
                c1.x/=4;c1.y/=4;c1.z/=4;

                for(c=0;c<6;c++)
                    for(a=0;a<n;a++)
                        for(b=0;b<n;b++)
                        {
                            int ind2=6*nrVertexComponents*(a*n+b);
                            glm::vec3 p0= glm::vec3(vertex[c][ind2+0*nrVertexComponents+ 0],vertex[c][ind2+0*nrVertexComponents+ 1],vertex[c][ind2+0*nrVertexComponents+ 2]);
                            glm::vec3 p1= glm::vec3(vertex[c][ind2+1*nrVertexComponents+ 0],vertex[c][ind2+1*nrVertexComponents+ 1],vertex[c][ind2+1*nrVertexComponents+ 2]);
                            glm::vec3 p2= glm::vec3(vertex[c][ind2+2*nrVertexComponents+ 0],vertex[c][ind2+2*nrVertexComponents+ 1],vertex[c][ind2+2*nrVertexComponents+ 2]);
                            glm::vec3 p3= glm::vec3(vertex[c][ind2+4*nrVertexComponents+ 0],vertex[c][ind2+4*nrVertexComponents+ 1],vertex[c][ind2+4*nrVertexComponents+ 2]);

                            c2=p0+p1+p2+p3;
                            c2.x/=4;c2.y/=4;c2.z/=4;

                            dist.push_back(Face());
                            dist[dist.size()-1].d=sqrt((c2.x-c1.x)*(c2.x-c1.x)+(c2.y-c1.y)*(c2.y-c1.y)+(c2.z-c1.z)*(c2.z-c1.z));
                            dist[dist.size()-1].i=a;
                            dist[dist.size()-1].j=b;
                            dist[dist.size()-1].k=c;
                        }

                sort(dist.begin(),dist.begin()+dist.size(),compareFace);

                neighbour[k*n*n+i*n+j][0]=dist[1].k*n*n+dist[1].i*n+dist[1].j;
                neighbour[k*n*n+i*n+j][1]=dist[2].k*n*n+dist[2].i*n+dist[2].j;
                neighbour[k*n*n+i*n+j][2]=dist[3].k*n*n+dist[3].i*n+dist[3].j;
                neighbour[k*n*n+i*n+j][3]=dist[4].k*n*n+dist[4].i*n+dist[4].j;
                if(n!=1)
                {
                    neighbour[k*n*n+i*n+j][4]=dist[4].k*n*n+dist[4].i*n+dist[4].j;
                    neighbour[k*n*n+i*n+j][5]=dist[5].k*n*n+dist[5].i*n+dist[5].j;
                    neighbour[k*n*n+i*n+j][6]=dist[6].k*n*n+dist[6].i*n+dist[6].j;
                    neighbour[k*n*n+i*n+j][7]=dist[7].k*n*n+dist[7].i*n+dist[7].j;
                }
                dist.clear();
            }
}

void Model::generateLevel()
{
    int i,j,k;

    bigUse.clear();
    for(i=0;i<nrSquares;i++)
        bigUse.push_back(0);

    for(i=0;i<nrSquares;i++)
        poz.push_back(i);

    srand(static_cast<unsigned int>(time(0)));
    for(i=0;poz.size()>0;i++)
    {
        int f=rand()%(poz.size());

        use.clear();
        for(j=0;j<nrSquares;j++)
            use.push_back(0);

        use[poz[f]]=1;
        bigUse[poz[f]]=1;

        sol.clear();
        sol.push_back(poz[f]);
        if(generatePath(rand()%(3*n)+5,i)==0)
            {createWall();i--;}

        for(j=0;j<poz.size();j++)
            for(k=0;k<sol.size();k++)
                if(poz[j]==sol[k])
                {
                    poz.erase(poz.begin()+j);
                    j--;
                    break;
                }
    }
}

bool Model::generatePath(int lgPath,int colorInd)
{
    int v[4],sCurrent,i,j,k,sum1,sum2;

    for(i=0;i<lgPath;i++)
    {
        int sizeV=0;
        sCurrent=sol[i];
        for(j=0;j<4;j++)
            if(bigUse[neighbour[sCurrent][j]]==0&&use[neighbour[sCurrent][j]]==0)
            {
                sum1=sum2=0;
                for(k=0;k<4;k++)
                    if(use[neighbour[neighbour[sCurrent][j]][k]]==1)
                       sum1++;
                for(k=4;k<8;k++)
                    if(use[neighbour[neighbour[sCurrent][j]][k]]==1)
                        sum2++;
                if(sum1<=1&&sum2<=1)
                    v[sizeV++]=neighbour[sCurrent][j];
            }
        if(sizeV!=0)
        {
            int r=rand()%sizeV;
            use[v[r]]=1;
            sol.push_back(v[r]);
        }
        else
        {
            if(i<2)
                return 0;
            break;
        }
    }

    for(i=0;i<sol.size();i++)
    {
        //changeTriangleColor(sol[i],color[colorInd]);
        bigUse[sol[i]]=1;
    }
    changeTriangleType(sol[0],1.0,color[colorInd]);
    changeTriangleType(sol[sol.size()-1],1.0,color[colorInd]);

    return 1;
}

void Model::createWall()
{
    int i;
    for(i=0;i<sol.size();i++)
        changeTriangleType(sol[i],-1.0,glm::vec3(0.0,0.0,0.0));
}

void Model::changeTriangleType(int number,float type,glm::vec3 color)
{
    int i;
    glm::vec3 square=numberToPoint(number);
    int ind=6*nrVertexComponents*(square.y*n+square.z);

    for(i=0;i<6;i++)
    {
        vertex[square.x][ind+i*nrVertexComponents+3]=color.r;
        vertex[square.x][ind+i*nrVertexComponents+4]=color.g;
        vertex[square.x][ind+i*nrVertexComponents+5]=color.b;
        vertex[square.x][ind+i*nrVertexComponents+8]=type;
    }
    sendDataToGPU();
}

glm::vec4 Model::getMouseRay()
{
    float x=sf::Mouse::getPosition(window).x;
    float y=sf::Mouse::getPosition(window).y;
    x=(2.0*x)/WIDTH-1.0;
    y=1.0-(2.0*y)/HEIGHT;
    glm::vec3 rayNDS = glm::vec3(x, y, 1.0);
    glm::vec4 rayClip = glm::vec4(rayNDS.x,rayNDS.y, -1.0, 1.0);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x,rayEye.y, -1.0, 0.0);
    glm::vec4 rayWorld = (glm::inverse(view) * rayEye);
    rayWorld = glm::normalize(rayWorld);
    return rayWorld;
}

glm::vec3 Model::numberToPoint(int number)
{
    int kF,iF,jF;
    kF=number/(n*n);
    iF=(number-kF*n*n)/n;
    jF=(number-kF*n*n)%n;
    return glm::vec3(kF,iF,jF);
}

void Model::changeTriangleColor(int nr,glm::vec3 color)
{
    int i;
    glm::vec3 square=numberToPoint(nr);
    int ind=6*nrVertexComponents*(square.y*n+square.z);

    for(i=0;i<6;i++)
    {
        vertex[square.x][ind+i*nrVertexComponents+3]=color.r;
        vertex[square.x][ind+i*nrVertexComponents+4]=color.g;
        vertex[square.x][ind+i*nrVertexComponents+5]=color.b;
        vertex[square.x][ind+i*nrVertexComponents+8]=2.0;
    }

    sendDataToGPU();
};

bool Model::intersect_triangle(glm::vec3 orig, glm::vec3 dir, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2)
{
    double epsilon =0.00001;
    double u,v;
    glm::vec3 edge1,edge2,tvec,pvec,qvec;
    double det,inv_det;

    edge1=vert1-vert0;
    edge2=vert2-vert0;
    pvec=glm::cross(dir,edge2);
    det=glm::dot(edge1,pvec);

    if(det>-epsilon&&det<epsilon)
        return 0;
    inv_det=1.0/det;
    tvec=orig-vert0;
    u=  glm::dot(tvec,pvec)*inv_det;
    if(u<0.0||u>1.0)
        return 0;
    qvec=glm::cross(tvec,edge1);
    v=glm::dot(dir,qvec)*inv_det;
    if(v<0.0||u+v>1.0)
        return 0;
    return 1;
}

void Model::handleEvents()
{
    float minDist=100.0,dist;
    bool ok=0;
    int i,j,k,kF,iF,jF;

    if(solvedLevel()==1)
    {
        screen=2;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        pitch+=2;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        pitch-=2;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        yaw-=2;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        yaw+=2;
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)||sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        glm::vec4 rayWorld=getMouseRay();
        for(k=0;k<6;k++)
            for(i=0;i<n;i++)
                for(j=0;j<n;j++)
                {
                    int ind=6*nrVertexComponents*(i*n+j);

                    glm::vec4 v0= view*model*glm::vec4(vertex[k][ind+0*nrVertexComponents+ 0],vertex[k][ind+0*nrVertexComponents+ 1],vertex[k][ind+0*nrVertexComponents+ 2],1.0);
                    glm::vec4 v1= view*model*glm::vec4(vertex[k][ind+1*nrVertexComponents+ 0],vertex[k][ind+1*nrVertexComponents+ 1],vertex[k][ind+1*nrVertexComponents+ 2],1.0);
                    glm::vec4 v2= view*model*glm::vec4(vertex[k][ind+2*nrVertexComponents+ 0],vertex[k][ind+2*nrVertexComponents+ 1],vertex[k][ind+2*nrVertexComponents+ 2],1.0);
                    glm::vec4 v3= view*model*glm::vec4(vertex[k][ind+4*nrVertexComponents+ 0],vertex[k][ind+4*nrVertexComponents+ 1],vertex[k][ind+4*nrVertexComponents+ 2],1.0);

                    if(intersect_triangle(glm::vec3(0,0,0),
                                             glm::vec3(rayWorld.x,rayWorld.y,rayWorld.z),
                                             glm::vec3(v0.x,v0.y,v0.z),
                                             glm::vec3(v1.x,v1.y,v1.z),
                                             glm::vec3(v2.x,v2.y,v2.z))==1||
                       intersect_triangle(glm::vec3(0,0,0),
                                             glm::vec3(rayWorld.x,rayWorld.y,rayWorld.z),
                                             glm::vec3(v1.x,v1.y,v1.z),
                                             glm::vec3(v3.x,v3.y,v3.z),
                                             glm::vec3(v2.x,v2.y,v2.z))==1)
                        {
                            glm::vec4 vF=v0+v1+v2+v3;
                            vF.x/=4.0;vF.y/=4.0;vF.z/=4.0;

                            dist=sqrt(vF.x*vF.x+vF.y*vF.y+vF.z*vF.z);
                            if(dist<minDist)
                            {
                                ok=1;
                                kF=k;
                                iF=i;
                                jF=j;
                                minDist=dist;
                            }
                        }
                }
        if(ok==1)
        {
            //cout<<kF*n*n+iF*n+jF<<' '<<getSquareType(kF*n*n+iF*n+jF)<<'\n';
            int ind=kF*n*n+iF*n+jF;
            if(getSquareType(ind)==1)
                currentColor=getSquareColor(ind);
            if(getSquareType(ind)==0||getSquareType(ind)==2)
            {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    if(currentColor!=glm::vec3(0.0,0.0,0.0))
                        changeTriangleColor(ind,currentColor);

                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    changeTriangleType(ind,0.0,glm::vec3(0.0,0.0,0.0));
            }
        }

    }
}

void Model::draw()
{
    int i;

    shader.Use();
    model=glm::eulerAngleXY(glm::radians(pitch),glm::radians(yaw));
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
    GLint projLoc = glGetUniformLocation(shader.Program, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glUniform1i(glGetUniformLocation(shader.Program, "skyboxTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,startFinishSquareTexture);
    glUniform1i(glGetUniformLocation(shader.Program, "startFinishSquareTexture"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, blankSquareTexture);
    glUniform1i(glGetUniformLocation(shader.Program, "blankSquareTexture"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, wallSquareTexture);
    glUniform1i(glGetUniformLocation(shader.Program, "wallSquareTexture"), 3);

    for(i=0;i<6;i++)
    {
        glBindVertexArray(VAO[i]);
        glDrawArrays(GL_TRIANGLES, 0, vertex[i].size()/nrVertexComponents);
        glBindVertexArray(0);
    }
}

bool Model::solvedLevel()
{
    int i;
    use.clear();
    for(i=0;i<nrSquares;i++)use.push_back(0);

    for(i=0;i<nrSquares;i++)
        if(getSquareType(i)==1.0)
        {
            bool found=0;

            use[i]=1;
            dfs(i,i,getSquareColor(i),found);
            use[i]=0;
            if(found==0)
                return 0;
        }
    return 1;
}

void Model::dfs(int k,int start,glm::vec3 squareColor,bool &found)
{
    int i;
    if(getSquareType(k)==1.0&&k!=start)
        found=1;
    else
    {
        for(i=0;i<4;i++)
            if(use[neighbour[k][i]]==0)
                if(getSquareColor(neighbour[k][i])==squareColor)
                {
                    use[neighbour[k][i]]=1;
                    dfs(neighbour[k][i],start,squareColor,found);
                    use[neighbour[k][i]]=0;
                }
    }
}

glm::vec3 Model::getSquareColor(int nr)
{
    glm::vec3 square=numberToPoint(nr);
    int ind=6*nrVertexComponents*(square.y*n+square.z);
    glm::vec3 color=glm::vec3(vertex[square.x][ind+3],vertex[square.x][ind+4],vertex[square.x][ind+5]);
    return color;
}

float Model::getSquareType(int nr)
{
    glm::vec3 square=numberToPoint(nr);
    int ind=6*nrVertexComponents*(square.y*n+square.z);
    return vertex[square.x][ind+8];
}

GLuint Model::loadOpenGLskybox(const char* path)
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

GLuint Model::loadOpenGLtexture(const char* path)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    sf::Image image;
    image.loadFromFile(path);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

bool compareFace(Face i,Face j)
{
    return i.d<j.d;
}

void Model::setColors()
{
    color.push_back(glm::vec3(1.0,0.0,0.0));
    color.push_back(glm::vec3(0.0,1.0,0.0));
    color.push_back(glm::vec3(0.0,0.0,1.0));
    color.push_back(glm::vec3(1.0,1.0,0.0));
    color.push_back(glm::vec3(0.0,1.0,1.0));
    color.push_back(glm::vec3(1.0,0.0,1.0));
    color.push_back(glm::vec3(1.0,0.0,0.0));
    color.push_back(glm::vec3(0.0,1.0,0.0));
    color.push_back(glm::vec3(0.0,0.0,1.0));
    color.push_back(glm::vec3(1.0,1.0,0.0));
    color.push_back(glm::vec3(0.0,1.0,1.0));
    color.push_back(glm::vec3(1.0,0.0,1.0));
    color.push_back(glm::vec3(1.0,0.0,0.0));
    color.push_back(glm::vec3(0.0,1.0,0.0));
    color.push_back(glm::vec3(0.0,0.0,1.0));
    color.push_back(glm::vec3(1.0,1.0,0.0));
    color.push_back(glm::vec3(0.0,1.0,1.0));
    color.push_back(glm::vec3(1.0,0.0,1.0));
    currentColor=glm::vec3(0.0,0.0,0.0);
}
