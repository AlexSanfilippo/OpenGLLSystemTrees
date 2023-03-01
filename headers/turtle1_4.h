#ifndef TURTLE_H
#define TURTLE_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include <tgmath.h> //for arcsin
#include <cstdlib>  // for rand()

//Lsystem
#include <headers/lsystem1_01.h>

const double PI = 3.14159265358979323846;



// An abstract turtle to use in OpenGL
using namespace std;
class Turtle
{
public:
    float angle = PI/2; //radians
    float d_angle = PI / 8; //how much we turn by
    
    //These starting 2 vertices define the base of the stem
    float width = 0.55;
    float height = 2.0;
    //float scale = 1.0f; //current scale
    float scale_decrease = 0.95; //how much to shrink(multiply) stems each draw
    float branch_angle = 45.0;
    float turn_angle = 15.0;
    float minScale = 0.05;  //do not draw fine details-wasteful
    int mode; //drawing mode
    vector<float> vertices = {-width, 0.0f, 0.0f,   width, 0.0, 0.0};
     
    vector<unsigned int> indices = {};

    vector<float> baseVertices;
    
    string instructions;
    int numVertices = 0; //pass to draw function
    int indicesIndex = 1;


    //LSystem Class
    Lsystem L = Lsystem(3,10);

    
    
    //2D Rotation Angles
    double radius = sqrt(powf(width, 2) + powf((height / 2), 2));
    double angle_tween = acos((2 * powf(radius, 2) - width*2) / 2 * powf(radius, 2));
    double angle_mod = angle_tween *width;
    float l_angle = angle + angle_mod;
    float r_angle = angle - angle_mod;
  
   
    // constructor 
    Turtle(float myAngle = PI/2, string myInstructions = "SS", int myMode = 3 /*2d or 3d*/)
    {
        mode = myMode;
        if (mode == 3) {
            float L = 2 * width;
            float R = L / sqrt(3);
            //An equilateral triangle centered at the origin
            vertices = { R, 0.0f, 0.0f, R*cos(4.0f*(float)PI/3.0f), 0.0, R * sin(4.0f * (float)PI / 3.0f), \
                R * cos(2.0f* (float)PI / 3.0f), 0.0f, R* sin(2.0f * (float)PI / 3.0f) };
            
            
            //vertices = { -width, 0.0f, 0.0f, width, 0.0, 0.0,  0.0, 0.0,  2 * width }; //old scalene triangle
            indicesIndex = 0;
            //cout << "Turtle created in 3d mode...\n";
            baseVertices = vertices;
        }
        else {
            //cout << "Turtle created in 2d mode...\n";
        }
        
        angle = myAngle;
        instructions = myInstructions;   

     
        
        L.readRules();
        L.iterate(3);
        instructions = L.getInstructions();
        cout << "Our Turtle found instructions " << instructions << endl;

    }

    /*Extends the instructions thru replacement*/
    /*This should be part of its own class, one for L-systems*/
    void iterate(unsigned int n){
        //srand(time(NULL));
        srand(1);
        for (int j = 0; j < n; j++) {
            int size = instructions.size();
            
            int rng = rand() % 100; //0 to 99, for random trees
            cout << "iter #" << j << " has rng = " << rng << endl;
            for (int i = 0; i < size; i++) {

                char curIns = instructions[i];
                string substr = instructions.substr(i, 2);
                if (curIns == 'S') {
                    //insert S->SRB
                    instructions.insert(i+1, "BS");
                    //instructions.replace(i, 3, "SBLS");
                    
                }
                /*
                if (rng < 50 && curIns == 'S') {
                    instructions.insert(i + 1, "B");
                }
                if (rng >= 50 && curIns == 'S') {
                    instructions.insert(i + 1, "S");
                }
                */
                /*
                else if (curIns == 'R') {
                    //insert S->SRB
                    //instructions.insert(i+1, "SSL");
                    instructions.replace(i, 4, "SLS");
                }
                else if (curIns == 'L') {
                    //insert S->SRB
                    //instructions.insert(i+1, "SSL");
                    instructions.replace(i+1, 2, "SL");
                }
                else if (curIns == 'R') {
                    //insert S->SRB
                    instructions.insert(i+1, "S");
                    //instructions.replace(i, 2, "S");
                }
                */
                /*
                else if (substr.compare("BS") == 0) {
                    instructions.replace(i, 2, "RSS");
                }
                */
            }
            cout << "after iterate " << j <<" instructions are " << instructions << endl;
        }

        cout << "after iterate() instructions are " << instructions << endl;
        
    }

    void iterate3D(unsigned int n) {
        //srand(time(NULL));
        srand(1);
        for (int j = 0; j < n; j++) {
            int size = instructions.size();

            int rng = rand() % 100; //0 to 99, for random trees
            //cout << "iter #" << j << " has rng = " << rng << endl;
            for (int i = 0; i < size; i++) {

                char curIns = instructions[i];
                string substr = instructions.substr(i, 2);
               
                if (substr.compare("SS") == 0) {
                    instructions.insert(i+2, "BX");
                }
                else if (substr.compare("BX") == 0) {
                    //instructions.replace(i+1, 2, "SLX");
                    instructions.insert(i + 2, "BZ");
                }
                else if (substr.compare("BZ") == 0) {
                    instructions.insert(i + 2, "RXSRZ]");
                    //instructions.replace(i, 2, "RSS");
                }
                else if (substr.compare("B3") == 0) {
                    instructions.insert(i + 2, "SSBX");
                    //instructions.replace(i, 2, "RSS");
                }
                else if (substr.compare("B4") == 0) {
                    instructions.insert(i + 2, "SS");
                    //instructions.replace(i, 2, "RSS");
                }
                else {
                    //instructions.insert(i + 1, "S");
                }
                
            }
            cout << "after iterate " << j << " instructions are " << instructions << endl;
        }

        cout << "after iterate() instructions are " << instructions << endl;

    }

    /*Simple 3D plant drawing-mode must be set to "3D" to use*/
    int draw3D(int insInd = 0, int vertPos = 9, glm::vec3 rad_angle = {0.0, 0.0, 0.0}, float scale = 1.0f) {

       
        //TP-->Custom Turtle Instruction Manipulation
        
        //some fun ones
        //SSSSB3SSB4SSBZSSBXSSBZSRXSRZSRXSRZSRXSRZS
        //LXLXSSSSTXSSSSBZSSSSSSB3SSSSSB4SSSSB]B3SSRXRXSRXRXSSB3SLXSLXSB4SBXSTZSTXS
        
        //instructions.clear();
        //string instructions = "SSSSSB4SSSSB3SSSPZSSB]SSPXSSB]SSPZSSB]SSPXSSB]SSPZSSB]SSPXSSB]SSPZSSB]"; 
        

        
        
        int i = 0;
        for (i = insInd; i < instructions.size(); i++) {
            char curIns = instructions[i];
            string substr = instructions.substr(i, 2);
            int insSize = instructions.size();
            //cout << "[draw3d] found instruction [" << curIns <<"], and pair ["<<substr << "]\n"; //tp
            if (scale < minScale) {
                continue;
            }
            if (curIns == 'S') {
                
                int size = vertices.size();

                
                if (vertPos == size) { //default rules of placement for stems
                    //fetch old vertex triplet - condense into immediate below block later
                    float oldXc = vertices[vertPos - 3];
                    float oldYc = vertices[vertPos - 2];
                    float oldZc = vertices[vertPos - 1];
                    float oldXb = vertices[vertPos - 6];
                    float oldYb = vertices[vertPos - 5];
                    float oldZb = vertices[vertPos - 4];
                    float oldXa = vertices[vertPos - 9];
                    float oldYa = vertices[vertPos - 8];
                    float oldZa = vertices[vertPos - 7];
                    
                    //the 3 old vertices - ie, current turtle postion
                    glm::vec4 ptA = glm::vec4(oldXa, oldYa, oldZa, 1.0f);
                    glm::vec4 ptB = glm::vec4(oldXb, oldYb, oldZb, 1.0f);
                    glm::vec4 ptC = glm::vec4(oldXc, oldYc, oldZc, 1.0f);
                    // base triplet
                    glm::vec4 baseA = glm::vec4(vertices[0], vertices[1], vertices[2], 1.0f);
                    glm::vec4 baseB = glm::vec4(vertices[3], vertices[4], vertices[5], 1.0f);
                    glm::vec4 baseC = glm::vec4(vertices[6], vertices[7], vertices[8], 1.0f);
                    //Midpoints
                    glm::vec4 baseMid = baseA + baseB + baseC;
                    baseMid /= 3;
                    glm::vec4 curTripletMid = ptA+ptB+ptC;
                    curTripletMid /= 3;
                    //new points triplet
                    glm::vec4 ptA2 = glm::vec4(vertices[0], vertices[1] + height, vertices[2], 1.0f);
                    glm::vec4 ptB2 = glm::vec4(vertices[3], vertices[4] + height, vertices[5], 1.0f);
                    glm::vec4 ptC2 = glm::vec4(vertices[6], vertices[7] + height, vertices[8], 1.0f);
                    
                    
                    //NEW translation method (23 Jan, 20:00)
                        //IF a turn is ordered, rotate at base then use midpoint
                        //ELSE we translate each triplet point by the SAME vector (not rotating the triplet)
                    //the 3 2nd oldest coordinates

                    //translation matrices
                    glm::mat4 transA = glm::mat4(1.0f);
                    glm::mat4 transB = glm::mat4(1.0f);
                    glm::mat4 transC = glm::mat4(1.0f);
                    glm::mat4 transS = glm::mat4(1.0f);
                    //scale
                    transS = glm::scale(transS, glm::vec3(scale, scale, scale));
                    scale *= scale_decrease;
                    ptA2 = transS * ptA2;
                    ptB2 = transS * ptB2;
                    ptC2 = transS * ptC2;
                    //rotate
                    transA = glm::rotate(transA, glm::radians(rad_angle[0]), glm::vec3(1.0, 0.0, 0.0)); //X
                    transA = glm::rotate(transA, glm::radians(rad_angle[1]), glm::vec3(0.0, 1.0, 0.0)); //Y
                    transA = glm::rotate(transA, glm::radians(rad_angle[2]), glm::vec3(0.0, 0.0, 1.0)); //Z
                    transB = glm::rotate(transB, glm::radians(rad_angle[0]), glm::vec3(1.0, 0.0, 0.0)); //X
                    transB = glm::rotate(transB, glm::radians(rad_angle[1]), glm::vec3(0.0, 1.0, 0.0)); //Y
                    transB = glm::rotate(transB, glm::radians(rad_angle[2]), glm::vec3(0.0, 0.0, 1.0)); //Z
                    transC = glm::rotate(transC, glm::radians(rad_angle[0]), glm::vec3(1.0, 0.0, 0.0)); //X
                    transC = glm::rotate(transC, glm::radians(rad_angle[1]), glm::vec3(0.0, 1.0, 0.0)); //Y
                    transC = glm::rotate(transC, glm::radians(rad_angle[2]), glm::vec3(0.0, 0.0, 1.0)); //Z
                        
                    //apply rotation to new triplet
                    ptA2 = transA * ptA2;
                    ptB2 = transB * ptB2;
                    ptC2 = transC * ptC2;
                    //rest translation matrices
                    transA = glm::mat4(1.0f);
                    transB = glm::mat4(1.0f);
                    transC = glm::mat4(1.0f);
                        
                    //translate
                    transA = glm::translate(transA, glm::vec3(ptA2.x , ptA2.y , ptA2.z  ));
                    transB = glm::translate(transB, glm::vec3(ptB2.x , ptB2.y , ptB2.z  ));
                    transC = glm::translate(transC, glm::vec3(ptC2.x , ptC2.y , ptC2.z  ));
                    //apply trans mat to cur Triplet's mid point to get 3 new points
                    ptA2 = transA * curTripletMid;
                    ptB2 = transB * curTripletMid;
                    ptC2 = transC * curTripletMid;

                    
                    //Add new set of 3 points to the vertices array
                    vertices.push_back(ptA2.x);
                    vertices.push_back(ptA2.y);
                    vertices.push_back(ptA2.z);
                    vertices.push_back(ptB2.x);
                    vertices.push_back(ptB2.y);
                    vertices.push_back(ptB2.z);
                    vertices.push_back(ptC2.x);
                    vertices.push_back(ptC2.y);
                    vertices.push_back(ptC2.z);
                    
                    

                    
                    numVertices += 3 * 3;//increase Vertices count
                    
                    vertPos += 3 * 3; //Increase our position in the  vertices array (For tracking branching)

                    //add indices to list so OpenGL knows where to draw triangles
                    //see page 4-front (22 Jan,2023)
                    //GENERATE COORDINATE INDICES
                    indicesIndex = (size / 3)-3;
                    for (int j = 0; j < 3; j++) {
                        //T1
                        indices.push_back(indicesIndex + j);
                        indices.push_back(indicesIndex + 3 + j);
                        //indices.push_back(indicesIndex + 1 + j);
                        if (j == 0)
                            indices.push_back(indicesIndex + 1);
                        if (j == 1)
                            indices.push_back(indicesIndex + 2);
                        if (j == 2)
                            indices.push_back(indicesIndex + 0);
                        //T2
                        indices.push_back(indicesIndex + j);
                        indices.push_back(indicesIndex + 3 + j);
                        //indices.push_back(indices.back()-1);
                        if (j == 0)
                            indices.push_back(indicesIndex + 5);
                        if (j == 1)
                            indices.push_back(indicesIndex + 3);
                        if (j == 2)
                            indices.push_back(indicesIndex + 4);   
                    }
                    vertPos = vertices.size();
                }
                else { //If we are doing the not-first branch of a branching case 
                    /*Vertices based on old postion for first after new branch*/
     
                    //fetch old vertex triplet - condense into immediate below block later
                    float oldXc = vertices[vertPos - 3];
                    float oldYc = vertices[vertPos - 2];
                    float oldZc = vertices[vertPos - 1];
                    float oldXb = vertices[vertPos - 6];
                    float oldYb = vertices[vertPos - 5];
                    float oldZb = vertices[vertPos - 4];
                    float oldXa = vertices[vertPos - 9];
                    float oldYa = vertices[vertPos - 8];
                    float oldZa = vertices[vertPos - 7];

                    //the 3 old vertices - ie, current turtle postion
                    glm::vec4 ptA = glm::vec4(oldXa, oldYa, oldZa, 1.0f);
                    glm::vec4 ptB = glm::vec4(oldXb, oldYb, oldZb, 1.0f);
                    glm::vec4 ptC = glm::vec4(oldXc, oldYc, oldZc, 1.0f);
                    // base triplet
                    glm::vec4 baseA = glm::vec4(vertices[0], vertices[1], vertices[2], 1.0f);
                    glm::vec4 baseB = glm::vec4(vertices[3], vertices[4], vertices[5], 1.0f);
                    glm::vec4 baseC = glm::vec4(vertices[6], vertices[7], vertices[8], 1.0f);
                    //Midpoints
                    glm::vec4 baseMid = baseA + baseB + baseC;
                    baseMid /= 3;
                    glm::vec4 curTripletMid = ptA + ptB + ptC;
                    curTripletMid /= 3;
                    //new points triplet
                    glm::vec4 ptA2 = glm::vec4(vertices[0], vertices[1] + height, vertices[2], 1.0f);
                    glm::vec4 ptB2 = glm::vec4(vertices[3], vertices[4] + height, vertices[5], 1.0f);
                    glm::vec4 ptC2 = glm::vec4(vertices[6], vertices[7] + height, vertices[8], 1.0f);


                    //NEW translation method (23 Jan, 20:00)
                        //IF a turn is ordered, rotate at base then use midpoint
                        //ELSE we translate each triplet point by the SAME vector (not rotating the triplet)
                    //the 3 2nd oldest coordinates

                    //translation matrices
                    glm::mat4 transA = glm::mat4(1.0f);
                    glm::mat4 transB = glm::mat4(1.0f);
                    glm::mat4 transC = glm::mat4(1.0f);
                    glm::mat4 transS = glm::mat4(1.0f);
                    //scale
                    transS = glm::scale(transS, glm::vec3(scale, scale, scale));
                    scale *= scale_decrease;
                    ptA2 = transS * ptA2;
                    ptB2 = transS * ptB2;
                    ptC2 = transS * ptC2;
                    //rotate
                    transA = glm::rotate(transA, glm::radians(rad_angle[0]), glm::vec3(1.0, 0.0, 0.0)); //X
                    transA = glm::rotate(transA, glm::radians(rad_angle[1]), glm::vec3(0.0, 1.0, 0.0)); //Y
                    transA = glm::rotate(transA, glm::radians(rad_angle[2]), glm::vec3(0.0, 0.0, 1.0)); //Z
                    transB = glm::rotate(transB, glm::radians(rad_angle[0]), glm::vec3(1.0, 0.0, 0.0)); //X
                    transB = glm::rotate(transB, glm::radians(rad_angle[1]), glm::vec3(0.0, 1.0, 0.0)); //Y
                    transB = glm::rotate(transB, glm::radians(rad_angle[2]), glm::vec3(0.0, 0.0, 1.0)); //Z
                    transC = glm::rotate(transC, glm::radians(rad_angle[0]), glm::vec3(1.0, 0.0, 0.0)); //X
                    transC = glm::rotate(transC, glm::radians(rad_angle[1]), glm::vec3(0.0, 1.0, 0.0)); //Y
                    transC = glm::rotate(transC, glm::radians(rad_angle[2]), glm::vec3(0.0, 0.0, 1.0)); //Z

                    //apply rotation to new triplet
                    ptA2 = transA * ptA2;
                    ptB2 = transB * ptB2;
                    ptC2 = transC * ptC2;
                    //rest translation matrices
                    transA = glm::mat4(1.0f);
                    transB = glm::mat4(1.0f);
                    transC = glm::mat4(1.0f);

                    //translate
                    transA = glm::translate(transA, glm::vec3(ptA2.x, ptA2.y, ptA2.z));
                    transB = glm::translate(transB, glm::vec3(ptB2.x, ptB2.y, ptB2.z));
                    transC = glm::translate(transC, glm::vec3(ptC2.x, ptC2.y, ptC2.z));
                    //apply trans mat to cur Triplet's mid point to get 3 new points
                    ptA2 = transA * curTripletMid;
                    ptB2 = transB * curTripletMid;
                    ptC2 = transC * curTripletMid;


                    //Add new set of 3 points to the vertices array                   
                    vertices.push_back(ptA2.x);
                    vertices.push_back(ptA2.y);
                    vertices.push_back(ptA2.z);
                    vertices.push_back(ptB2.x);
                    vertices.push_back(ptB2.y);
                    vertices.push_back(ptB2.z);
                    vertices.push_back(ptC2.x);
                    vertices.push_back(ptC2.y);
                    vertices.push_back(ptC2.z);
                                        
                    numVertices += 3 * 3;//increase Vertice count
                    

                    /*GENERATE INDICES*/
                    //new logic for 3d brancing...
                    int oldII = (vertPos / 3)-3;
                    indicesIndex =( size/3);
                    for (int j = 0; j < 3; j++) {
                        //T1
                        indices.push_back(oldII + j); //done
                        indices.push_back(indicesIndex + j); //done
                        //indices.push_back(indicesIndex + 1 + j);
                        if (j == 0)
                            indices.push_back(oldII + 1);
                        if (j == 1)
                            indices.push_back(oldII + 2);
                        if (j == 2)
                            indices.push_back(oldII + 0);
                        //T2
                        indices.push_back(oldII + j); //done
                        indices.push_back(indicesIndex + j); //done
                        if (j == 0)
                            indices.push_back(indicesIndex + 2);
                        if (j == 1)
                            indices.push_back(indicesIndex + 0);
                        if (j == 2)
                            indices.push_back(indicesIndex + 1);
                    }
                    vertPos = vertices.size();
                    indicesIndex = (vertPos/3)-3; //yes
                }
               
                indicesIndex += 3; //we added 3 new points=3 new rows of vertices
                //indices.push_back(indicesIndex); //?
                numVertices += 3 * 3;
            }
            /*6 different types of Rotation*/
            else if (substr.compare("RZ") == 0) { //change X angle Right
                rad_angle -= glm::vec3(0.0, 0.0, turn_angle);
                i += 1;
            }
            else if (substr.compare("LZ") == 0) { //change X angle left
                rad_angle += glm::vec3(0.0, 0.0, turn_angle);
                i += 1;
            }
            else if (substr.compare("RX") == 0) { //change Z angle Right
                rad_angle += glm::vec3(turn_angle, 0.0, 0.0);
                i += 1;
            }
            else if (substr.compare("LX") == 0) { //change Z angle left
                rad_angle -= glm::vec3(turn_angle, 0.0, 0.0);
                i += 1;
            }
            else if (substr.compare("RY") == 0) { //change Y angle Right
            rad_angle += glm::vec3( 0.0, turn_angle , 0.0);
            i += 1;
            }
            else if (substr.compare("LY") == 0) { //change Y angle left
            rad_angle -= glm::vec3(0.0, turn_angle,  0.0);
            i += 1;
            }
            //increase scale
            else if (substr.compare("MG") == 0) { //change Y angle left
                //rad_angle -= glm::vec3(0.0, turn_angle, 0.0);
                scale *= 2;
                i += 1;
            }
            //2-way "fork" branch
            else if (substr.compare("BX") == 0) {
                glm::vec3 BAngle = rad_angle + glm::vec3(30.0, 0.0, 0.0);
                int moveIndex;
                moveIndex = draw3D(i + 1, vertPos/*vertPos*/, BAngle, scale);
                rad_angle -= glm::vec3(30.0, 0.0, 0.0);
                if (moveIndex == insSize) {
                    moveIndex = i;
                }
                i = moveIndex + 1;
            }
            
            else if (substr.compare("BZ") == 0) {
                glm::vec3 BAngle = rad_angle + glm::vec3(0.0, 0.0, branch_angle);
                int moveIndex = draw3D(i + 1, vertPos/*vertPos*/, BAngle/**/, scale);
                rad_angle -= glm::vec3(0.0, 0.0, 30.0);
                if (moveIndex == insSize) {
                    moveIndex = i;
                }
                i = moveIndex + 1;
            }
            //2-way "straight" branches
            else if (substr.compare("TX") == 0) {
            glm::vec3 BAngle = rad_angle + glm::vec3(branch_angle, 0.0, 0.0);
            int moveIndex;
            moveIndex = draw3D(i + 1, vertPos/*vertPos*/, BAngle, scale);
            if (insSize == moveIndex)
                moveIndex = i;
            //rad_angle -= glm::vec3(30.0, 0.0, 0.0);
            i = moveIndex + 1;
            }
            else if (substr.compare("TZ") == 0) {
            glm::vec3 BAngle = rad_angle + glm::vec3(0.0, 0.0, branch_angle);
            int moveIndex;
            moveIndex = draw3D(i + 1, vertPos/*vertPos*/, BAngle, scale);
            //rad_angle -= glm::vec3(30.0, 0.0, 0.0);
            if (moveIndex == insSize)
                moveIndex = i;
            i = moveIndex + 1;
            }
            else if (substr.compare("PZ") == 0) {
                glm::vec3 BAngle = rad_angle + glm::vec3(0.0, 0.0, branch_angle);
                glm::vec3 CAngle = rad_angle - glm::vec3(0.0, 0.0, branch_angle);
                int moveIndex;
                moveIndex = draw3D(i + 1, vertPos/*vertPos*/, BAngle, scale);
                moveIndex = draw3D(i + 1, vertPos/*vertPos*/, CAngle, scale);
                //rad_angle -= glm::vec3(30.0, 0.0, 0.0);
                if (moveIndex == insSize)
                    moveIndex = i;
                i = moveIndex + 1;
            }
            else if (substr.compare("PX") == 0) {
                glm::vec3 BAngle = rad_angle + glm::vec3(branch_angle, 0.0, 0.0);
                glm::vec3 CAngle = rad_angle - glm::vec3(branch_angle, 0.0, 0.0);
                int moveIndex;
                moveIndex = draw3D(i + 1, vertPos/*vertPos*/, BAngle, scale);
                moveIndex = draw3D(i + 1, vertPos/*vertPos*/, CAngle, scale);
                //rad_angle -= glm::vec3(30.0, 0.0, 0.0);
                if (moveIndex == insSize)
                    moveIndex = i;
                i = moveIndex + 1;
            }

            //3-way branch
            else if (substr.compare("B3") == 0) {
            
                glm::vec3 BAngle = rad_angle + glm::vec3(branch_angle*cos(4.0f*(float)PI/3.0f), 0.0, branch_angle * sin(4.0f * (float)PI / 3.0f));
                glm::vec3 CAngle = rad_angle + glm::vec3(branch_angle* cos(2.0f * (float)PI / 3.0f), 0.0, branch_angle* sin(2.0f * (float)PI / 3.0f));
                int moveIndex;
                moveIndex = draw3D(i + 2, vertPos/*vertPos*/, BAngle, scale);
                moveIndex = draw3D(i + 2, vertPos/*vertPos*/, CAngle, scale);
                rad_angle += glm::vec3(branch_angle, 0.0, 0.0);
                if (moveIndex == insSize) {
                    moveIndex = i;
                }
                i = moveIndex + 1;
            }
            //4-way branch
            else if (substr.compare("B4") == 0) {
                glm::vec3 BAngle = rad_angle + glm::vec3(30.0, 0.0, 0.0);
                glm::vec3 CAngle = rad_angle + glm::vec3(0.0, 0.0, 30.0);
                glm::vec3 DAngle = rad_angle + glm::vec3(-30.0, 0.0, 0.0);
                int moveIndex;
                moveIndex = draw3D(i + 2, vertPos/*vertPos*/, BAngle, scale);
                moveIndex = draw3D(i + 2, vertPos/*vertPos*/, CAngle, scale);
                moveIndex = draw3D(i + 2, vertPos/*vertPos*/, DAngle, scale);
                rad_angle -= glm::vec3(0.0, 0.0, 30.0);
                if (moveIndex == insSize) {
                    moveIndex = i;
                }  
                i = moveIndex + 1;       
            }
            else if (substr.compare("B]") == 0) { //end the drawing loop-breaks out of recursion
                break;
            }  
        }
        return i;
    } //end of draw3D


    /**
    Drawing Instuctions
        This function parses the string of instructions and returns
        OLD 2D turtle.
    */
    void draw(int insInd = 0, int vertPos = 6, float cur_angle = PI/2) {
        l_angle = cur_angle + angle_mod;
        r_angle = cur_angle - angle_mod;
        //cout << "in draw() instructions are " << instructions << endl;
        //float old_angle = cur_angle; //tp
        for (int i = insInd; i < instructions.size(); i++) {

            //cout << "drawing instruction #" << i << endl;
            char curIns = instructions[i];
            //cout << "current instruction: -" << curIns << "-\n";
            if (curIns == 'S') {
                
                //add 2 new vertices
                //old vertex pair
                float oldXa = vertices[vertPos - 6];
                float oldYa = vertices[vertPos - 5];
                float oldXb = vertices[vertPos - 3];
                float oldYb = vertices[vertPos - 2];
                
               
                //cout << "angle = " << angle << endl;
                //cout << "cos(angle) = " << cos(cur_angle) << endl;
                //cout << "sin(angle) = " << sin(cur_angle) << endl;

                //midpoint of last pair's midpoint
                float oldMidpointX = (oldXa + oldXb) / 2;
                float oldMidpointY = (oldYa + oldYb) / 2;
                //new vertex 1 (left)
                //vertices.push_back(oldXa + height * cos(cur_angle));
                //vertices.push_back(oldYa +  height * sin(cur_angle));
                vertices.push_back(oldMidpointX + height * cos(l_angle));
                vertices.push_back(oldMidpointY + height * sin(l_angle));
                vertices.push_back(0.0f);
                //new vertex 2 (right)
                //vertices.push_back(oldXb + height * cos(cur_angle));
                //vertices.push_back(oldYb + height * sin(cur_angle));
                vertices.push_back(oldMidpointX + height * cos(r_angle));
                vertices.push_back(oldMidpointY + height * sin(r_angle));
                vertices.push_back(0.0f);
                
                
                indicesIndex += 2;
                //add to indices - 3 points of the triangle -for non-branching bits
                int size = vertices.size();
                if (vertPos == size) {
                    for (int j = 0; j < 2; j++) {
                        indices.push_back(indicesIndex);
                        indices.push_back(indicesIndex + 1);
                        indices.push_back(indicesIndex + 2);
                        indicesIndex += 1;
                    }
                }
                else {
                    /*NEW triange indexing logic*/

                    int old_coorda = (vertPos / 3) - 2; //0
                    int old_coordb = (vertPos / 3) - 1; //1
                    int new_coorda = indicesIndex - 1;
                    int new_coordb = indicesIndex;
                    indices.push_back(old_coorda);
                    indices.push_back(old_coordb);
                    indices.push_back(new_coorda);
                    indices.push_back(old_coordb);
                    indices.push_back(new_coorda);
                    indices.push_back(new_coordb);
                    
                    
                    //vertPos += 6;
                    vertPos = vertices.size();
                }

                //increase triangle count
                numVertices += 2 * 3;
   
            }
            
            else if (curIns == (char)'R') {
                cur_angle -= d_angle;
                l_angle -= d_angle;
                r_angle -= d_angle;
            }
            else if (curIns == (char)'L') {
                cur_angle += d_angle;
                l_angle += d_angle;
                r_angle += d_angle;
            }
            else if (curIns == (char)'B') {
                draw(i+1, vertPos/*vertPos*/, cur_angle - d_angle);
                cur_angle += d_angle;  
                l_angle = cur_angle + angle_mod;
                r_angle = cur_angle - angle_mod;
            }
            else if (curIns == (char)'A') { //simple triangle leaf - proof of concept
                //old vertex pair
                //tp
                //cout << "drawing leaf" << endl;
                int size = vertices.size();
                float oldXa = vertices[size - 3];
                float oldYa = vertices[size - 2];
                float oldXb = vertices[size - 8];
                float oldYb = vertices[size - 9];

                //place leaf at right side midpoint
                vertices.push_back(oldXa + 0.1f);
                vertices.push_back(oldYa + 0.1f);
                vertices.push_back(0.0f);
                vertices.push_back(oldXa + 0.1f);
                vertices.push_back(oldYa - 0.1f);
                vertices.push_back(0.0f);

                //add 3 points of leaf triangle to indices
                indices.push_back(indicesIndex);
                indices.push_back(indicesIndex + 1);
                indices.push_back(indicesIndex + 2);
                indicesIndex += 2;

                //increase triangle count
                numVertices += 2 * 3;
            }
            

        }
        //TP
        /*
        cout << "In draw we added these coords" << endl;
        cout << vertices.size() << " = vertices.size()" << endl;
        for (int i = 0; i < vertices.size(); i++) {
            cout << vertices[i] << ",";
            if (i % 3 == 2)
                cout << "\n";
        }
        cout << "and these indices" << endl;
        for (int i = 0; i < indices.size(); i++) {
            cout << indices[i] << ",";
            if (i % 3 == 2)
                cout << "\n";
        }
        */

        //TP
        //cout << "done drawing!\n";
    }

    /*Returns a pointer to the array of vertices*/
    vector <float> getVertices() {   
        return vertices;
    }
    /*Returns a pointer to the array of vertices*/
    vector <unsigned int> getIndices() {
        return indices;
    }
    
    


private:
    
};
#endif
