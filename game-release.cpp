//====================================================== =====================
// Name        : Tetris.cpp 
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Tetris...
//====================================================== =====================
#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <fstream>
#include <iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<string>
//#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sstream>
#include<cmath>      // for basic math functions such as cos, sin, sqrt
#define grid_x 65// snake takes jump small
#define grid_y 65
#define move_factor 1
using namespace std;

// function prototypes
void SetCanvasSize(int, int);
void find(int , int& , int& );
void update_snake_location();
void generate_powerfood();// generating the power food 
void check_power_food();// this function is to check that the snake eats the power food or not
void generate_food1();// generating a food
void generate_food2();
void generate_food3();
void generate_food4();
void generate_food5();
void checkFood1();
void checkFood2();
void checkFood3();
void checkFood4();
void checkFood5();
void Menu();
void Display();
void MouseClicked(int , int , int, int);
void NonPrintableKeys(int , int , int );
void PrintableKeys(unsigned char, int, int);
void no_key_check();// this function is called when no key is pressed from the keyboard
void Timer(int);// this is for the speed of the snake....
void set_grid();
void generate_hurdles();
void check_snake_collision_with_hurdle();
void check_snake_collision_with_self();
void store_score();
void gameover();
// variables
int snake_x=grid_x/2, snake_y=grid_y/2; // coordinates for snakes head
int tail_x, tail_y; // coordinates for snake tail
int food1x,food1y,food2x,food2y,food3x,food3y,food4x,food4y,food5x,food5y;// variable for the random food  with the x and y componets
int power_foodx,power_foody;
int maximum_value=0,minimum_value=65;
int last_key,length=3;
int scale_factor,temp1,temp2,temp3,temp4;
int grid[grid_x][grid_y];
bool check=false,printable_key_check=false;
bool snake_eaten_food = false,power_food_eaten=false;// flag to check if snake ate food or not 
string event="right",score="15";// initialy the score is 15 and the snake is moving in the right direction
string mode = "Menu"; // game mode
//bool snake_eaten_food1=false;
int temp_of_score=15;
int count1=115,count2=118;// these two are for the progress of the sanke
int food_timer[5] = {0,0,0,0,0}; // array of 5 integers representing timer of 5 foods on screen
int hurdle_x; // array to store x coordinates of 3 hurdles
int hurdle_y;
int hurdel_timer=0,power_food_timer=0;
char orientation;
string name;
char ch;

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}
void store_score()
{
    ofstream file;
    file.open("scores.txt", ios::app);

    file << name << "\t\t";
    file << score << endl;

    file.close();
}

// helper function
void find(int num, int& pos_x, int& pos_y) 
{
    // find num in the grid and return its position
    for (int i=0; i<grid_x; i++)
    {
        for (int j=0; j<grid_y; j++) 
        {
            if (grid[i][j] == num)
            {
                pos_x = i;
                pos_y = j;
                break;
            }
        }
    } 
}

void update_snake_location() 
{
    // function to update the body and head location of snake
    // on backend grid

    // start from the tail of the snake 
    // and move towards head
    // then move head to new position
    for (int i=length; i>1; i--)
    {
        // find position of tail in the grid
        int pos_x1, pos_y1;
        int pos_x2, pos_y2;
        find(i, pos_x1, pos_y1); // position of 3
        find(i-1, pos_x2, pos_y2); // position of 2

        // updating coordinates
        grid[pos_x1][pos_y1] = 0; // prev. position of 3 = 0
        grid[pos_x2][pos_y2] = i; // position of 3 = position of 2
    }

    // now update the head position to new position
    if(event=="left")
    {
        snake_x--; // snake head
    }
    else if(event=="right")
    {
        snake_x++;
    }
    else if(event=="up")
    {
        snake_y++;
    }
    else 
    {
        snake_y--;

    }

    //check_snake_collision_with_self();

    if (snake_x < 0) // snake head if moving to the left and move outside from the left side and then it come from the right side
    {
        snake_x = grid_x - 1; // the right most position of the grid
    }
    if (snake_x >= grid_x) 
    {
        snake_x = 0; // snake head if moving to the right and move outside from the right side and then it come from the left side
    }

    if (snake_y < 0)// snake head if moving to the down and move outside from the down side and then it come from the top side
    {
        snake_y = grid_y - 1;
    }
    if (snake_y >= grid_y)// snake head if moving to the up and move outside from the downside and then it come from the down side
    {
        snake_y = 0; 
    }

    grid[snake_x][snake_y] = 1;//head of the snake 

    if (snake_eaten_food)
    {
        snake_eaten_food = false;
        length++; // increase snake length
        grid[tail_x][tail_y] = length;//stores the tail of the snake 
        count2++;
        temp_of_score+=5;
        score=to_string(temp_of_score);// integer to string coversion
    }
    else if (power_food_eaten)
    {
        power_food_eaten=false;
        temp_of_score+=20;
        score=to_string(temp_of_score);
    }

    check_snake_collision_with_hurdle();
}

void check_snake_collision_with_hurdle() {
    // check snake collision with hurdle
    for (int i=0; i<=20; i++)
    {
        if (orientation == 'h')
        { 
            if ((snake_x == hurdle_x + i )&& (snake_y == hurdle_y) && (mode == "Start Game"))
            {
                    glClearColor(0.5, 0.5,
            0.2, 0.1);
            glClear(GL_COLOR_BUFFER_BIT);   //Update the colors
                DrawString(365,365,"Game Over",colors[RED]);
                // store_score();
                exit(0);
                glutSwapBuffers();
            }
        }
        else
        {
            if ((snake_x == hurdle_x) && (snake_y == hurdle_y + i) && (mode == "Start Game"))
            {
                // end game
                cout << "Game end";
                    glClearColor(0.5/*Red Component*/, 0.5/*Green Component*/,
            0.2/*Blue Component*/, 0.1/*Alpha component*/);// Red==Green==Blue==1 --> White Colour
    glClear(GL_COLOR_BUFFER_BIT);   //Update the colors

                DrawString(grid_x * scale_factor/2, grid_y * scale_factor / 2, "Game Over", colors[RED]);
                glutPostRedisplay();
                store_score();
                exit(0);
                glutSwapBuffers(); // do not modify this line..
            }            
        }
    }
}

/*
 * Main Canvas drawing function.
 * */
double startx=320,starty=400;
void generate_powerfood()// generating the power food 
{
    while (true)
    {   srand(time(0));
        power_foodx=rand()%grid_x;
        power_foody=rand()%grid_y;

        if((grid[power_foodx][power_foody]==0))// check that the generating power function is not on the body
        {
            break;
        }
    }
    
}
void check_power_food()// this function is to check that the snake eats the power food or not
{   power_food_timer++;
    if(( snake_x==power_foodx) && (snake_y==power_foody || snake_y+1==power_foody))
    {// to check that the snake has eaten food or not 
       power_food_eaten=true;
        power_foodx=700;
        power_foody=700;
        // power_food_eaten=false;
    }

    else if(power_food_timer==400)// if the timer is zero than reset this and generate the food again
       {
            power_food_timer=0;
            generate_powerfood();
       }
    else if(power_food_timer==150 && power_food_eaten!=true)// if the snakes not eat food in that time
       {
            power_foodx=700;
            power_foody=700;
       }
}
void generate_food1()// generating a food
{
    food_timer[0] = 0;
    srand(time(0));
     while(true)
    {   
        
        while(true)
        {      
            food1x=rand()%grid_x;                                                                            // for the diagonal of the food i.e primary   
            if((food1x!=food2x && food1x!= food3x && food1x!=food4x && food1x!=food5x )&& (food1x!=food2x+1 && food1x!=food3x+1 && food1x!=food4x+1 && food1x!=food5x+1) && (food1x!=food2x-1 && food1x!=food3x-1 && food1x!=food4x-1 && food1x!=food5x-1))
            {   
                break;
            
            }
        }
        while (true)
        {
            food1y=rand()%grid_y;
            if((food1y != food2y && food1y!=food3y && food1y!=food4y && food1y!=food5y)&& (food1y!=food2y+1 && food1y!=food3y+1 && food1y!=food4y+1 && food1y!=food5y+1) && (food1y!=food2y-1 && food1y!=food3y-1 && food1y!=food4y-1 && food1y!=food5y-1) )
            {
                break;
            }
        }
        if ((grid[food1x][food1x] == 0))
        {
            break;
        }
    }
}

void checkFood1()
{
    if (food_timer[0] == 100)
    {
        generate_food1();
        return;
    }
    else if(snake_x==food1x && snake_y==food1y )// if the snake has eaten the food
    {
        food_timer[0] = 0;
        int pos_x, pos_y; // find the position of tail in grid
        find(length, pos_x, pos_y);
        tail_x = pos_x;
        tail_y = pos_y;
        snake_eaten_food = true;
         generate_food1();
    }
    else // increase food not eaten timer
    {
        food_timer[0]++;
    }
}

void generate_food2()
{  
    food_timer[1] = 0;
    while(true)// this loop is run until it finds a freee space
    {   
        while(true)// loop will run untill it has  no this food has no food on it x coridnate
        {   srand(time(0));
            food2x=rand()%grid_x;
            if((food2x!=food1x && food2x!=food3x && food2x!=food4x && food2x!=food5x ) && (food2x!=food1x+1 && food2x!=food3x+1 && food2x!=food4x+1 && food2x!=food5x+1)&& ( food2x!=food1x-1 && food2x!=food3x-1 && food2x!=food4x-1 && food2x!=food5x-1))// the x cordinate of food doesnot matches with the other food x cordinates
            {
                break;
            }
        }
        while (true)// loop will run untill it has  no this food has no food on it x coridnate
        {
            srand(time(0));
            food2y=rand()%grid_y;
            if((food2y != food1y && food2y != food3y && food2y != food4y && food2y != food5y) && (food2y!=food1y+1 && food2y!=food3y+1 && food2y!=food4y+1 && food2y!=food5y+1)&& ( food2y!=food1y-1 && food2y!=food3y-1 && food2y!=food4y-1 && food2y!=food5y-1))// the y cordinate of food doesnot matches with the other food y cordinates
            {
                break;
            }

        }
        
        if (grid[food2x][food2x] == 0)// to find that ther is free location at that location
        {
            break;
        }
    }   
    
}
void checkFood2()
{
    if (food_timer[1] == 100) // regenerate food if timer is up
    {
        generate_food2();
        return;
    }
    if(snake_x==food2x && snake_y==food2y )// if the snake has eaten the food
    {
        int pos_x, pos_y; // find the position of tail in grid
        find(length, pos_x, pos_y);
        tail_x = pos_x;
        tail_y = pos_y;
        snake_eaten_food = true;

         // assign a new position for food
         generate_food2();
    }
    else // increase food not eaten timer
    {
        food_timer[1]++;
    }
}
void generate_food3()
{
    food_timer[2] = 0;
    while(true)// to check that the generated food is on the free location
    {  
        srand(time(0));
        
        while(true)//it will run unitl it find no food in its x cordinate
        {   
            srand(time(0));
            food3x=rand()%grid_x;
            if((food3x!=food1x && food3x!=food2x  &&  food3x!=food4x && food3x!=food5x )&& (food3x!=food2x+1 && food3x!=food1x+1 && food3x!=food4x+1 && food3x!=food5x+1) && (food3x!=food2x-1 && food3x!=food1x-1 && food3x!=food4x-1 && food3x!=food5x-1))
            {
                break;
            }
        }
        while (true)//it will run unitl it find no food in its y cordinate
        {   
            srand(time(0));
            food3y=rand()%grid_y;
            if((food3y != food1y && food3y != food2y && food3y != food4y && food3y != food5y)&& (food3y!=food2y+1 && food3y!=food1y+1 && food3y!=food4y+1 && food3y!=food5y+1) && (food3y!=food2y-1 && food3y!=food1y-1 && food3y!=food4y-1 && food3y!=food5y-1))
            {
                break;
            }

        }
        
        if ((grid[food3x][food3x] == 0))// to check that the generated food is on the free location
        {
            break;
        }
    }   
    
}

void checkFood3()
{
    if (food_timer[2] == 100)
    {
        generate_food3();
        return;
    }
    if(snake_x==food3x && snake_y==food3y )// if the snake has eaten the food
    {
        food_timer[2] = 0;
        int pos_x, pos_y; // find the position of tail in grid
        find(length, pos_x, pos_y);
        tail_x = pos_x;
        tail_y = pos_y;
        snake_eaten_food = true;

         // assign a new position for food
         generate_food3();
    }
    else // increase food not eaten timer
    {
        food_timer[2]++;
    }
}

void generate_food4()
{
    food_timer[3] = 0;
    while(true)// this loop is will run until it finds no free location i.e where there is no body of snake
    {
        
        while(true)// this will run unitl no  food x  cordinate is same in of it 
        {   srand(time(0));
            food4x=(rand()%grid_x);
            if((food4x!=food1x && food4x!=food2x  &&  food3x!=food4x && food4x!=food5x ) && (food4x!=food2x+1 && food4x!=food3x+1 && food4x!=food1x-1 && food4x!=food5x+1)&& (food4x!=food2x-1 && food4x!=food3x-1 && food4x!=food1x-1 && food4x!=food5x-1))
            {
                break;
            }
        }
        while (true)// this will run unitl no  food y  cordinate is same in of it 
        {    
            srand(time(0));
            food4y=(rand()%grid_y);
            if((food4y != food1y && food4y != food2y && food3y != food4y && food4y != food5y) && (food4y!=food2y+1 && food4y!=food3y+1 && food4y!=food1y-1 && food4y!=food5y+1)&& (food4y!=food2y-1 && food4y!=food3y-1 && food4y!=food1y-1 && food4y!=food5y-1) )
            {
                break;
            }

        }
        
        if ((grid[food4x][food4x] == 0))// if is to check that there at that point there is no body od snake
        {
            break;
        }
    }   
    
}

void checkFood4()
{
    if (food_timer[3] == 100)
    {
        generate_food4();
        return;
    }
    if(snake_x==food4x && snake_y==food4y )// if the snake has eaten the food
    {
        food_timer[3] = 0;
        int pos_x, pos_y; // find the position of tail in grid
        find(length, pos_x, pos_y);
        tail_x = pos_x;
        tail_y = pos_y;
        snake_eaten_food = true;

         // assign a new position for food
         generate_food4();
    }
    else // increase food not eaten timer
    {
        food_timer[3]++;
    }
}
void generate_food5()
{   
    food_timer[4] = 0;
    while(true)// this loop is will run until it finds no free location i.e where there is no body of snake
    {
        
        while(true)//this will run unitl no  food x  cordinate is same in of it 
        {   srand(time(0));
            food5x=(rand()%grid_x);
            if((food5x!=food1x && food5x!=food2x  &&  food3x!=food5x && food4x!=food5x ) && (food5x!=food2x+1 && food5x!=food3x+1 && food5x!=food4x+1 && food5x!=food1x+1) && food5x!=food2x-1 && food5x!=food3x-1 && food5x!=food4x-1 && food5x!=food1x-1)
            {
                break;
            }
        }
        while (true)//this will run unitl no  food Y  cordinate is same in of it 
        {
            srand(time(0));
            food5y=(rand()%grid_y);
            if((food5y != food1y && food5y != food2y && food5y != food4y && food5y != food3y)&& (food5y!=food2y+1 && food5y!=food3y+1 && food5y!=food4y+1 && food5y!=food1y+1) && food5y!=food2y-1 && food5y!=food3y-1 && food5y!=food4y-1 && food5y!=food1y-1)
            {
                break;
            }
        }
        
        if ((grid[food5x][food5x] == 0))
        {
            break;
        }
    }   
}

void checkFood5()
{
    if (food_timer[4] == 100)
    {
        generate_food5();
        return;
    }
    if(snake_x==food5x && snake_y==food5y )// if the snake has eaten the food
    {
        food_timer[4] = 0;//sets the timer of food 5 to zero
        int pos_x, pos_y; // find the position of tail in grid
        find(length, pos_x, pos_y);
        tail_x = pos_x;
        tail_y = pos_y;
        snake_eaten_food = true;

         // assign a new position for food
         generate_food5();
    }
    else
    {
        food_timer[4]++;
    }
}

void Menu() {
    mode = "Start Game";
    if (mode == "Start Game") // if the user clicks the start game then the game will start
    {
        glutDisplayFunc(Display);
        return;
    }
    glClearColor(0.5/*Red Component*/, 0.5/*Green Component*/,
            0.2/*Blue Component*/, 0.1/*Alpha component*/);// Red==Green==Blue==1 --> White Colour
    glClear(GL_COLOR_BUFFER_BIT);   //Update the colors
     
    DrawString(264, 507, "Start Game", colors[WHITE]);
    DrawString(200, 407, "The black color is of the Hurdles", colors[WHITE]);
    DrawString(200, 357, "The small square are the food ", colors[WHITE]);
    DrawString(200, 307, "Round Orange Food is Power Food", colors[WHITE]);
    DrawString(200, 257, "Press Esc to Exit Game", colors[WHITE]);
    DrawString(200, 207, "Click the Start Game to Start", colors[WHITE]);
    glutSwapBuffers(); // do not modify this line..
    // return;
}

void Display()
{   
    glClearColor(0.5/*Red Component*/, 0.5/*Green Component*/,
            0.2/*Blue Component*/, 0.1/*Alpha component*/);// Red==Green==Blue==1 --> White Colour
    glClear(GL_COLOR_BUFFER_BIT);   //Update the colors
   DrawString( 500, 600, "Score:", colors[MISTY_ROSE]); // this will print given string at x=500 , y=600
    DrawString( 570, 600, score, colors[MISTY_ROSE]);
    DrawLine( 115 , 608 ,295 , 608 , 2 ,colors[WHITE] ); 
    DrawString( 50, 600, "Snake", colors[MISTY_ROSE]);
    DrawLine( count1, 608 , count2, 608 , 4 , colors[RED]);// the progress of the snake
    int pos_x, pos_y;
    for (int i=0; i<grid_x; i++) // maping snake body to screen size
    {
        for (int j=0; j<grid_y; j++) 
        {
            if (grid[i][j] != 0) 
            {   if(grid[i][j]==1)
                {
                    pos_x = i * scale_factor; // mapping to window size
                     pos_y= j * scale_factor; // mapping to window size
                    // setting the coordinates of the head
                    if(event=="right")
                    {
                        pos_x+=6;
                        pos_y+=4;
                    }
                    else if(event=="left")
                    {
                        pos_x+=3;
                        pos_y+=4;
                    }
                    else if(event=="up")
                    {
                        pos_x+=5;
                        pos_y+=5;
                    }
                    else if(event=="down")
                    {
                        pos_x+=5;
                        pos_y+=4;
                    }
                    DrawCircle(pos_x,pos_y ,6,colors[RED]);
                }
                
                else if(grid[i][j]>1)
                {
                    pos_x= i * scale_factor; // mapping to window size
                    pos_y = j * scale_factor; // mapping to window size
                    DrawSquare(pos_x,pos_y ,10,colors[10]);// to draw the square before the head 
                }

            }
            DrawSquare(food1x*scale_factor,food1y*scale_factor,10,colors[YELLOW_GREEN]); // food
            DrawSquare(food2x*scale_factor,food2y*scale_factor,10,colors[YELLOW]);
            DrawSquare(food3x*scale_factor,food3y*scale_factor,10,colors[BLUE]);
            DrawSquare(food4x*scale_factor,food4y*scale_factor,10,colors[CYAN]);
            DrawSquare(food5x*scale_factor,food5y*scale_factor,10,colors[ORANGE_RED]);
            DrawCircle(power_foodx*scale_factor,power_foody*scale_factor,5,colors[ORANGE]);// this is to draw the power food in the game 
    
        }
    } 	

    if(orientation == 'h')
        for (int i=0; i<=20; i++)// to print the hurdle 
            DrawSquare((hurdle_x + i) * scale_factor, hurdle_y * scale_factor, 10, colors[BLACK]);
    else
        for (int i=0; i<=20; i++)/// to print the hurdle
            DrawSquare((hurdle_x) * scale_factor, (hurdle_y + i) * scale_factor, 10, colors[BLACK]);
    check=false,printable_key_check=false;
   glutSwapBuffers(); // do not modify this line..
}


// function for the click of the mouse
void MouseClicked(int button, int state, int x, int y) {


	if (mode == "Menu" && button == GLUT_LEFT_BUTTON) //when the left button is clicked
	{
        if (x > 264 && x < 379 && y > 126 && y < 146)// it is to start the game
        {
            mode = "Start Game";   
            cout << "mode changed to " << mode << endl;
            return;
            // glutDisplayFunc(Display);
        }

	} 

    glutPostRedisplay();
}

void NonPrintableKeys(int key, int x, int y)
{  
    if((key ==last_key) || ( key!= GLUT_KEY_DOWN &&  key!=GLUT_KEY_UP &&  key!=GLUT_KEY_RIGHT &&  key!=GLUT_KEY_LEFT))
    {
       // this check is for sometimes the user press two keys at a time and it disturbs the movement of the snake
       //i.e either stops the snake or disturbs it body 
    }
    else
    {
    grid[snake_x][snake_y] = 0; 
    if(event=="up" || event =="down")// the snake can move left or right when he is moving either up or dowm
    {
        if (key == GLUT_KEY_LEFT   /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
            event="left";						// what to do when left key is pressed...
            snake_x--;

        } 
        else if (key == GLUT_KEY_RIGHT  /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) 
        {
            event="right";
            snake_x++;
        }
    
    }
    else // the snake can up left or down when he is moving either left or right
    {
        if (key == GLUT_KEY_UP) /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/ {
            event="up";
            snake_y++;

        }
        else if (key == GLUT_KEY_DOWN)   /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/ {		
            event="down";
            snake_y--;
        }
    }

    if (snake_x < 0) // snake head if moving to  left and move outside from the left and then it come from the right 
    {
        snake_x = grid_x - 1; 
    }
    if (snake_x >= grid_x)// snake head if moving to  right and move outside from the right  and then it come from the left 
    {
        snake_x = 0;
    }

    if (snake_y < 0) // snake head if moving to  down and move outside from the down and then it come from the top
    {
        snake_y = grid_y - 1;
    }
    if (snake_y >= grid_y)// snake head if moving to up and move outside from the up  and then it come from the down
    {
        snake_y = 0; 
    }
    grid[snake_x][snake_y] = 1; 
    check=true;// it will tell that the key is pressed

     glutPostRedisplay();

    }
    last_key =key;
}

void PrintableKeys(unsigned char key, int x, int y) 
{
    if (key == KEY_ESC/* Escape key ASCII*/) {
        exit(1); // exit the program when escape key is pressed.
    }
    printable_key_check=true;
    glutPostRedisplay();
}


void no_key_check()// this function is called when no key is pressed from the keyboard
{
    if(check==false && printable_key_check==false) // when there is no key is pressed from the bottom then it will make the snake moveable
    {   
        checkFood1();
        checkFood2();
        checkFood3();
        checkFood4();
        checkFood5();
        check_power_food();
        update_snake_location();
    }
}

void Timer(int m)// this is for the speed of the snake....
{
    no_key_check();

    if (hurdel_timer == 200)
    {
        generate_hurdles();
    }
    hurdel_timer++;
	glutPostRedisplay();
    glutTimerFunc(100.0 / FPS, Timer, 0);
}

void set_grid() 
{
    // cout << "Enter your name : ";
    // cin >> name;

    for (int i=0; i<grid_x; i++) 
    {
        for (int j=0; j<grid_y; j++) 
        {
            grid[i][j] = 0;// storing the empty spaces as 0
        }
    }
    grid[snake_x][snake_y] = 1; // head
    grid[snake_x-1][snake_y-5] = 2; // body 1
    grid[snake_x-2][snake_y-5] = 3; // body 2
    // Initially generate the food in the window 
    generate_food1();
    generate_food2();
    generate_food3();
    generate_food4();
    generate_food5();
    generate_powerfood();

    generate_hurdles();
}

// helper function to generate hurdle at a specific index if collision detected
// with food or snake
void generate_hurdles() 
{
    hurdel_timer = 0;
    hurdle_x = rand() % grid_x;
    hurdle_y = rand() % grid_y;  

    // check collission
    if(rand() % 2 == 0) // horizontal hurdle
    {
        if((hurdle_x + 20 )> grid_x)
            generate_hurdles();
        orientation = 'h';// generate the hurdle as a horizontal
        for (int i=0; i<=20; i++)     
            if (grid[hurdle_x + i][hurdle_y] != 0) // snake body collision detection
            {
                generate_hurdles();
            }
    }
    else // vertical hurdle
    {
        if(hurdle_y + 20 > grid_y)
        {
            generate_hurdles();
        }
        orientation = 'v';// the orientation of the hurdle will be vertical
        for(int i=0; i<=20; i++)     
            if (grid[hurdle_x][hurdle_y + i] != 0) // snake body collision detection
            {
                generate_hurdles();
            }        
    }
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {
    set_grid();
    int width = 650, height = 650; // i have set my window size to be 800 x 600
    scale_factor = width / grid_x; // because width and height are same and this will be helpfull to map on the window
    InitRandomizer(); // seed the random number generator...
    glutInit(&argc, argv); // initialize the graphics library...
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
    glutInitWindowPosition(50, 50); // set the initial position of our window
    glutInitWindowSize(width, height); // set the size of our window
    glutCreateWindow("PF's Snake Game"); // set the title of our game window
    SetCanvasSize(width, height); // set the number of pixels...
    glutDisplayFunc(Menu);// this function is to display the menu
    glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
    glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
    // This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
    glutTimerFunc(5.0 / FPS, Timer, 0);

	glutMouseFunc(MouseClicked);
    glutMainLoop();
    return 1;
}
#endif /* Snake Game */

