//
//  main.cpp
//  Snake
//
//  Created by Santiago Vera on 04/07/20.
//  Copyright © 2020 Mistic_Lion. All rights reserved.
//

#include <iostream>
#include <vector>
#include "olcConsoleGameEngineSDL.h"
using namespace std;

class Game : public olcConsoleGameEngine{
    //----------------------------------------------------------------------------------------------------------
private:
    //Control integers
    int cont=0;
    int cont_2=0;
    
    int const SquareSize=4;
    
    //Establish default direction
    int n=3;
    
    //Structure of y and x positions among with it's speed
    struct Coord{
        
        float x=40;
        float y=40;
        
        int Speed=8;
    };
    
    //Vector of Coord structures
    vector <Coord> Body;
    //Principal cordinates of the apple and the head of the snake
    Coord XY;
    Coord AppleXY;
    
    //Default snake lenght
    int Snake_lenght=5;
    //Control integer to verify if the lenght has increased
    int templenght=Snake_lenght;
    //----------------------------------------------------------------------------------------------------------
public:
    //Constructor and destructor
    Game(){}
    ~Game(){}
    //----------------------------------------------------------------------------------------------------------
    //Changes the direction of the player avoiding changing it towards any body of the snake behind
    int Goes(){
        
        if(GetKey(VK_DOWN).bPressed and Body[1].x!=Body[0].x) n=0;
        else if (GetKey(VK_LEFT).bPressed and Body[1].y!=Body[0].y) n=1;
        else if (GetKey(VK_UP).bPressed and Body[1].x!=Body[0].x) n=2;
        else if (GetKey(VK_RIGHT).bPressed and Body[1].y!=Body[0].y) n=3;
        
        return n;
    }
    
    bool AppleEaten(){
        if(Body[0].x<(AppleXY.x+SquareSize) and (Body[0].x+SquareSize)>AppleXY.x){
            if(Body[0].y<(AppleXY.y+SquareSize) and (Body[0].y+SquareSize)>AppleXY.y){
                return true;
            }
        }
        return false;
    }
    
    void Apple(){
        if(AppleEaten()){
        AppleXY.x=(int)((rand()%ScreenWidth()/4)*4);
        AppleXY.y=(int)((rand()%ScreenHeight()/4)*4);
            
        Snake_lenght++;
        }
        
        Fill(AppleXY.x, AppleXY.y, AppleXY.x+SquareSize, AppleXY.y+SquareSize, PIXEL_SOLID, FG_RED);
    }

    void Snake(int lenght){
        cont++;
        
        Goes();
        
        float x=XY.x;
        float y=XY.y;
        
        if(cont_2<1){
            AppleXY.x=(int)((rand()%ScreenWidth()/4)*4);
            AppleXY.y=(int)((rand()%ScreenHeight()/4)*4);
            
            for(int i=0; i<lenght; i++){
                Body.push_back({x, y});
                x=x-SquareSize;
            }
        }
        
        cont_2++;
        
        if(cont>=XY.Speed){
            
        switch(Goes()){
            case 0:
                XY.y = XY.y+SquareSize;
                break;
            case 1:
                XY.x = XY.x-SquareSize;
                break;
            case 2:
                XY.y = XY.y-SquareSize;
                break;
            case 3:
                XY.x = XY.x+SquareSize;
                break;
        }
            
        if(cont_2>=1){
            
            for(int i=lenght-1; i>0; i--){
                Body[i].x=Body[i-1].x;
                Body[i].y=Body[i-1].y;
            }
            
            Body[0].x=XY.x;
            Body[0].y=XY.y;
            
            
            cont=0;
        }
            
        
        //Handle hitboxes
        if (XY.x+SquareSize>ScreenWidth() or XY.x-SquareSize<-4 or XY.y+SquareSize>ScreenHeight() or XY.y-SquareSize<-4){
            for(int i=0; i<lenght; i++){
                Body.pop_back();
            }
            XY.x=40;
            XY.y=40;
            Snake_lenght=5;
            cont_2=0;
            n=3;
        }
            
        for(int i=1; i<lenght; i++){
            if(Body[0].x<(Body[i].x+SquareSize) and (Body[0].x+SquareSize)>Body[i].x){
                if(Body[0].y<(Body[i].y+SquareSize) and (Body[0].y+SquareSize)>Body[i].y){
                    for(int i=0; i<lenght; i++){
                        Body.pop_back();
                    }
                    XY.x=40;
                    XY.y=40;
                    Snake_lenght=5;
                    cont_2=0;
                    n=3;
                }
            }
        }
            
        }
        
        //Render squares
        for(int i=0; i<Body.size(); i++){
            
        if(i==0) Fill((int)Body[0].x, (int)Body[0].y, (int)Body[0].x+SquareSize, (int)Body[0].y+SquareSize, PIXEL_SOLID, FG_GREEN);
            
        else Fill((int)Body[i].x, (int)Body[i].y, (int)Body[i].x+SquareSize, (int)Body[i].y+SquareSize, PIXEL_SOLID, FG_WHITE);
        }
        
        if(lenght>templenght){
            Body.push_back({XY.x, XY.y});
        }
        
        templenght=lenght;
    }
    //----------------------------------------------------------------------------------------------------------
    virtual bool OnUserCreate(){
        return true;
    }
    //----------------------------------------------------------------------------------------------------------
    virtual bool OnUserUpdate(float fElapsedTime){
        
        if(GetKey(VK_SPACE).bPressed) Snake_lenght++;
        
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
        
        Snake(Snake_lenght);
        
        Apple();
        
        cout << AppleXY.x << endl << AppleXY.y << endl << endl;
        
        return true;
    }
    //----------------------------------------------------------------------------------------------------------
};

int main() {
    Game Demo;
    if(Demo.ConstructConsole(400, 240, 4, 4)) Demo.Start();
    else cout << "Could not Construct Console\n";
}
