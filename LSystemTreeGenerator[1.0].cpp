/*
* Current Issues:   1) DONE bring my code into a new OpenGL project and get it running 
*                   2) Learn how to compile/export this project for distrubtion to others
*                          a) DONE Build on RELEASE mode
*                   3) Upload to a new Git repository on GitHub -DONE
*                           a) improve by move headers to soln folder in a new headers folder and re-link
*                   3) Clean up and comment the code properly
*                   4) Upload to itch.io and one other file-sharing site (one that doesn't require a minimum donation to me)
*                   5) Share my project on
*                       a) LinkedIn
*                       b) Facebook
*                       c) Reddit
*                       d) Discord friends
*                       e) Burroughs Friends
*Date:24 Feb, 2023
*Author: Alexander Sanfilippo
* Brief: This program uses openGL and L-Systems to draw procedurally generated tree-like modules by placing points in space
*       and connecting these points as the vertices of triangles.  The goal here is to apply what I have learned from
*       teaching myself OpenGL as well as for the joy of artistic expression.
*
*
*/

//TESTING GIT PUSHING ljksdflkjsdflksdfljksfdlksdfjlksfdljksdfljksfdlkjsdf
//sdfsdf

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <headers/shader_s.h>

#include <iostream>

/*Add Matric Algebra Functions*/
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

/*Texture Loading*/
#include "headers/stb_image.h"

/*Camera*/
#include <headers/camera.h>

/*Trees*/
#include <headers/turtle1_4.h>


//function headers
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// stores how much we're seeing of either texture
float mixValue = 0.2f;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
/*[CAMERA] Create Camera Object*/
// camera
Camera ourCam = Camera(glm::vec3(0.0f, 13.0f, 35.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//animate drawing of tree
int numRectangles = 1;
unsigned int numRectanglesMax;
bool gotKey = false;

bool rotateCamera = true;

//change tree color on press C
int colorSetVal = 0;
int colorSetChange = 0;

//change camera rotation radius in-program UP and DOWN keys
float radius = 60.0f;

//Forr spawning new tree - change name later
float stretch = 1.0f;
bool changeTree = false;
//For "growing" tree by manipulating scale over time
float scale = 1.0;
bool doAnimate = false;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL_L_Systems", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //for camera-mouse input
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //discrete key press functionality 
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    //tp 
    std::cout << "calling Shader constructor..." << std::endl;

    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");
    
    //Shader ourShader("C:/Users/2015a/Documents/C++ Projects/OpenGLProjects/Include/headers/3.3.shader.vs",
    //    "C:/Users/2015a/Documents/C++ Projects/OpenGLProjects/Include/headers/3.3.shader.fs"); 
    
    // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesOld[] = {
        // positions         // colors
         0.9f, -0.9f, 0.0f,  1.0f, 0.0f, 1.0f,  // bottom right
        -0.9f, -0.9f, 0.0f,  0.0f, 1.0f, 1.0f,  // bottom left
         0.0f,  0.9f, 0.0f,  0.0f, 0.0f, 0.0f   // top 
    };
    
    float vertices2[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    float vertices3[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
         0.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //can we add another?-YES
    };
    float verticesCube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    
    Turtle turtle = Turtle(3); //create our turtle
    //tell turtle to generate vertices and index information
    //turtle.iterate3D(3);
    turtle.draw3D();
    
    cout << "turtle drew " << turtle.numVertices/3 << " triangles. \n";
    
    //get vertex info from the turtle and convert to CPP array
    vector<float> vertices_vec = turtle.getVertices();
   
    
    //dynamically created array of floats
    //int size = vertices_vec.size();
    int verticesSize = vertices_vec.size();
    float* vertices = new float[verticesSize];
    //populate the vertices
    for (int i = 0; i < verticesSize; i++) {
        vertices[i] = vertices_vec[i];

    }
    //repeat for indices
    
    
    
    //NEW Indices dynamic size
    vector<unsigned int> indices_vec = turtle.getIndices();
    //dynamically created array of floats
    //int size = vertices_vec.size();
    int indicesSize = indices_vec.size();
    unsigned int* indices = new unsigned int[indicesSize];
    //populate the vertices
    for (int i = 0; i < indicesSize; i++) {
        indices[i] = indices_vec[i];

    }


    cout << "ORIG tree has elmsToRender " << turtle.numVertices << " vertices\n";
    
    cout << "ORIG Indices size = " << indicesSize << endl;
    cout << "ORIG vertices size = " << verticesSize << endl;


    //TP
    bool dispVertexInfo = false;
    if (dispVertexInfo) {
        cout << "from turtle we received these coordinates" << endl;
        for (int i = 0; i < vertices_vec.size(); i++) {
            cout << vertices_vec[i] << ",";
            if (i % 3 == 2)
                cout << "\n";
        }
        cout << "from turtle we received these indices" << endl;
        for (int i = 0; i < indices_vec.size(); i++) {
            cout << indices[i] << ",";
            if (i % 3 == 2)
                cout << "\n";
        }

    }
    
    
    
    
    
    unsigned int elmsToRender = turtle.numVertices;
    unsigned int elmsToRenderMax = turtle.numVertices; //nonchanging

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])* verticesSize, &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indicesSize, &indices[0], GL_STATIC_DRAW); //works with small vertices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])* indicesSize * 1, &indices[0], GL_STATIC_DRAW);

    
    /*
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesB), indicesB, GL_STATIC_DRAW);
    */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT/*type*/, GL_FALSE, 3 * sizeof(float)/*stride was 3*/, (void*)0)/*offset*/;
    glEnableVertexAttribArray(0);
    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture coord attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    

    
    
   
    /*[TEXTURE]create a texture (id)*/
    unsigned int texture1, texture2;
    //texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    //wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*[TEXTURE]If using clamp border wrapping, then set border color this way*/
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /*[TEXTURE]If using clamp border wrapping, then set border color this way*/
    
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //load the image
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load awesomeface texture" << std::endl;
    }
    stbi_image_free(data);


    

    
    /*Tut: see max number of vertex attributes (aka inputs to the vertex shader) allowed on my hardware*/
    /*
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    */

    /*Transformations-basic vector transformation*/
    /*
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //glm vector of 3 4 floats
    glm::mat4 trans = glm::mat4(1.0f); //id matrix
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); //(id matrix, transformation vector) = translation matrix
    vec = trans * vec; //multiply our vector by the translation matrix
    std::cout << vec.x << vec.y << vec.z << std::endl;
    */
    /*Rotate and Scale an object*/
    /*
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));
    */
    

    bool moveTriangle = false;


    //[TEXTURE]
    ourShader.use(); // don't forget to activate the shader before setting uniforms!  
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
    ourShader.setInt1("texture2", 1); // or with shader class

    //[COORD SYS]
    //3D projection
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    //glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f)/*FOV*/, 1200.0f / 800.0f/*aspect ratio*/, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST); //enable depth testing

    //cube postions 
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    //tree postions 
    glm::vec3 treePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(15.0f,  0.0f,  -20.0f),
    glm::vec3(30.0f,  0.0f,  -40.0f),
    glm::vec3(-15.0f,  0.0f,  -20.0f),
    glm::vec3(-30.0f,  0.0f,  -40.0f),
    };

    


    numRectangles = turtle.numVertices / 6;
    numRectanglesMax = turtle.numVertices / 6;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        // set the texture mix value in the shader
        ourShader.setFloat1("mixValue", mixValue);
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // old teal: 0.2f, 0.3f, 0.3f, 1.0f
        //glClear(GL_COLOR_BUFFER_BIT); //old 2d version
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       
        /*Animation logic*/
        if (doAnimate) {
            
            if (scale < 0.999) {
                scale += 0.0001;
                
            }

            if (numRectangles < numRectanglesMax) {
                numRectangles += (max((int)numRectanglesMax/10000, 1));
            }
            
            if (scale > 0.999 && numRectangles >= numRectanglesMax) {
                numRectangles = numRectanglesMax; //correct for over draw
                
                doAnimate = false;
            }
        }


        /*Spawn New Tree (Wrap into a function later on, this is messy*/
        if (stretch == 2.0) {
            cout << "Making New Tree\n";
            stretch = 1.0;
            
            
            Turtle turtle = Turtle(3); //create our turtle
            //tell turtle to generate vertices and index information
            //turtle.iterate3D(3);
            turtle.draw3D();

            cout << "turtle drew " << turtle.numVertices / 3 << " triangles. \n";

            //get vertex info from the turtle and convert to CPP array
            vector<float> vertices_vec = turtle.getVertices();


            //dynamically created array of floats
            //int size = vertices_vec.size();
            float verticesSize = vertices_vec.size();
            float* vertices = new float[verticesSize];
            //populate the vertices
            for (int i = 0; i < verticesSize; i++) {
                vertices[i] = vertices_vec[i];

            }
            //repeat for indices



            //NEW Indices dynamic size
            vector<unsigned int> indices_vec = turtle.getIndices();
            //dynamically created array of floats
            //int size = vertices_vec.size();
            float indicesSize = indices_vec.size();
            unsigned int* indices = new unsigned int[indicesSize];
            //populate the vertices
            for (int i = 0; i < indicesSize; i++) {
                indices[i] = indices_vec[i];

            }

            elmsToRender = turtle.numVertices;
            elmsToRenderMax = turtle.numVertices; //nonchanging

            cout << "New tree has elmsToRender " << elmsToRender << " vertices\n";
            cout << "NEW Indices size = " << indicesSize << endl;
            cout << "NEW vertices size = " << verticesSize << endl;
            
            /*
            unsigned int VBO, VAO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            
            glBindVertexArray(VAO);
            */

            //glBindBuffer(GL_ARRAY_BUFFER, VBO);

            //try to clear out the buffers
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
            glBindVertexArray(VAO);
            

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * verticesSize, &vertices[0], GL_DYNAMIC_DRAW);

            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indicesSize, &indices[0], GL_STATIC_DRAW); //works with small vertices
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indicesSize, &indices[0], GL_DYNAMIC_DRAW);

            //glBindVertexArray(0);
            //TP
            //glBindVertexArray(VAO);
            numRectangles = turtle.numVertices / 6;
            numRectanglesMax = turtle.numVertices / 6;
        }
        

        float timeValue = glfwGetTime();
       

        //fractal stuff
        ourShader.setFloat1("systime", timeValue);

        // render the triangle
        //ourShader.use();

        // camera/view transformation
        glm::mat4 view = ourCam.GetViewMatrix();
        //ourShader.setMat4("view", view);


        //glBindVertexArray(VAO);

        //glBindTexture(GL_TEXTURE_2D, texture);
        //bind textures an texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        
        

        /*Controlable Camera*/
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // input
        // -----
        processInput(window);

        glm::mat4 projection = glm::perspective(glm::radians(ourCam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.001f, 100.0f);
        //ourShader.setMat4("projection", projection);
        view = ourCam.GetViewMatrix();
        //ourShader.setMat4("view", view);

        /*Rotating Camera*/
        if (rotateCamera) {
            radius;
            float spinSpeed = 0.33;
            float camX = sin(glfwGetTime()*spinSpeed) * radius;
            float camZ = cos(glfwGetTime()*spinSpeed) * radius;
            //glm::mat4 view;
            view = glm::lookAt(glm::vec3(camX, 40.0, camZ), glm::vec3(0.0, 19.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            //view = glm::lookAt(glm::vec3(10.0, 30.0, 30.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        }
        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        

        
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        
        

        // render container 10 times
        
        
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draws using indices in EBO
        float timeVar = 1.0f *  (float)glfwGetTime();
        for (unsigned int i = 0; i < 1; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, treePositions[i]);
            float angle = 0.0f;
            
            
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //ourShader.setMat4("model", model);

            //glDrawArrays(GL_TRIANGLES, 0, 3);
            ourShader.use();
            glBindVertexArray(VAO);
            //glDrawElements(GL_TRIANGLES, elmsToRender /*num vertices*/, GL_UNSIGNED_INT, 0);
            glDrawElements(GL_TRIANGLES, 6*numRectangles /*num vertices*/, GL_UNSIGNED_INT, 0);
            //glBindVertexArray(0);
        }
        
        if (colorSetChange == 1) {
            if (colorSetVal == 13) {
                colorSetVal = 0;
            }
            else {
                colorSetVal += 1;
            }
            ourShader.setInt1("colorSet", colorSetVal);
            cout << "colorSetVal set to " << colorSetVal << endl;
            colorSetChange = 0;
        }
        
        //numRectangles += 1;

        
        //ourShader.use();
        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, elmsToRender, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36); //draws vertices.  (How does it know where to connect triangles?)


        /*
        glm::mat4 trans = glm::mat4(1.0f); //transformation matrix
        //first triangle
        //trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, -(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        
        
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        */
        //second triangle
        /*
        trans = glm::mat4(1.0f); //reset trans mat
        //trans = glm::scale(trans, glm::vec3(2.0 * glfwGetTime(), 2.0 * glfwGetTime(), 2.0 * glfwGetTime())); //zooming
        //trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        */


        


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //clear dynamically allocated resources
    delete[] vertices; 
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    //CAMERA INPUT PROCESSING WITH CAMERA CLASS

    //"sprint" input logic
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        ourCam.MovementSpeed = 8.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        ourCam.MovementSpeed = SPEED;
    }
    /*
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        ourCam.MovementSpeed = 0.025f;
    }
    else if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)) {
        ourCam.MovementSpeed = SPEED;
    }
    */

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ourCam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ourCam.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ourCam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ourCam.ProcessKeyboard(RIGHT, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && gotKey == false) {
        numRectangles += 1;
        gotKey = true;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE) {
        //numRectangles += 1;
        gotKey = false;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        numRectangles += 1;

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        numRectangles = 0;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        numRectangles = numRectanglesMax;
    }
    
        


    //mouse input not handled in this function!!!
    
    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        colorSetChange = 1;
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        if (rotateCamera == true) {
            rotateCamera = false;
        }
        else {
            rotateCamera = true;
        }
        
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        radius += 1;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        radius -= 1;
    }
    //spawn new tree
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        stretch = 2.0f;
    }
    //animate tree growth by increase scale over time
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        scale = 0.000;
        doAnimate = true;
        numRectangles = 0;
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    ourCam.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ourCam.ProcessMouseScroll(static_cast<float>(yoffset));
}