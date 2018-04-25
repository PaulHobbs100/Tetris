/*****************************************************************
* Simple TETRIS implemenation in C++ and SFML 2D graphic library *
*                                                                *
*    No Warranties given, this is NOT production quality code    *
*                       Paul Hobbs: 2018                         *
*****************************************************************/

#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

const int BOARD_HEIGHT=20;          //Height of the playing area
const int BOARD_WIDTH =10;          //Width of the playing area
const int BLOCK_SIZE =18;           //Size of block in pixels
const int FIGURE_COUNT =7;          //Number of possible figure shapes
const int FIGURE_SIZE =4;           //Size of each figure in blocks
const int WINDOW_WIDTH =320;        //Width of display window
const int WINDOW_HEIGHT = 480;      //Height of display window

int field[BOARD_HEIGHT][BOARD_WIDTH] ={0};

struct Point
    {int x,y;} a[FIGURE_SIZE],b[FIGURE_SIZE];

int figures[FIGURE_COUNT][FIGURE_SIZE] =
{
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};

bool bounds_check()
 {  /*************************************************************************************
    * For each block in the figure check if within the game area boundary, returns:
    * true  = within game boundary
    * false = invalid position
    **************************************************************************************/
     for (int i = 0;i < FIGURE_SIZE;i++)
         if (a[i].x < 0  || a[i].x >= BOARD_WIDTH || a[i].y >= BOARD_HEIGHT ) return false;
         else if (field[a[i].y] [a[i].x]) return false;

         return true;
 };

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),"Tetris!");

    Texture t1,t2,t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");


    Sprite s(t1),
    background(t2),
    frame(t3);

    s.setTextureRect((IntRect(0,0,BLOCK_SIZE,BLOCK_SIZE)));

    int dx=0;
    bool rotate=0;
    int colorNum=1;
    float timer=0;
    float delay=0.3;

    Clock clock;
 //Initialise first brick
        a[0].x=0, a[0].y=1;
        a[1].x=1, a[1].y=1;
        a[2].x=1, a[2].y=2;
        a[3].x=1, a[3].y=3;

    while(window.isOpen())
    {

        float time =clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;



        while (window.pollEvent(e))
        {





            if (e.type == Event::Closed)
                    window.close();

            if (e.type == Event::KeyPressed)

                if(e.key.code ==Keyboard::Up) rotate =true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
            }

            if (Keyboard::isKeyPressed(Keyboard::Down)) delay =0.05;





//move
            for(int i=0;i<FIGURE_SIZE;i++) { b[i]=a[i]; a[i].x +=dx;}
            if (!bounds_check()) for (int i=0;i<FIGURE_SIZE;i++) a[i] =b[i];

//rotate
            if(rotate)
            {
                Point p =a[1]; //the centre of rotation
                for (int i=0; i<FIGURE_SIZE;i++)
                {
                    int x = a[i].y -p.y;
                    int y = a[i].x -p.x;
                    a[i].x=p.x -x;
                    a[i].y =p.y+y;
                }
                if (!bounds_check()) for (int i=0;i<FIGURE_SIZE;i++) a[i] =b[i];
            }

    //tick
            if (timer >delay)
            {
                for (int i=0;i<FIGURE_SIZE;i++) { b[i]=a[i]; a[i].y +=1; }

                if (!bounds_check())
                {
                    for (int i=0;i<FIGURE_SIZE;i++) field[b[i].y][b[i].x] =colorNum;

                    colorNum =1+rand() % FIGURE_COUNT;
                    int n = rand() % FIGURE_COUNT;
                    for(int i=0;i<FIGURE_SIZE;i++)
                    {
                        a[i].x =figures[n][i] %2;
                        a[i].y = figures[n][i] /2;
                    }
                }
            timer =0;
            }

        //check lines

            int k =BOARD_HEIGHT-1;
            for (int i=BOARD_HEIGHT-1;i>0;i--)
            {
                int count =0;
                for (int j=0;j< BOARD_WIDTH;j++)
                {
                    if (field[i][j])  count++;
                    field[k][j] = field[i][j];
                }
            if(count < BOARD_WIDTH) k--;
            }

            dx=0;
            rotate=0;
            delay =1; //0.3;
//draw
            window.clear(Color::White);
            window.draw(background);

            for (int i = 0;i < BOARD_HEIGHT;i++)
                for(int j = 0;j < BOARD_WIDTH;j++)
                {
                    if (field[i][j]==0) continue;
                    s.setTextureRect(IntRect(field[i][j]*BLOCK_SIZE,0,BLOCK_SIZE,BLOCK_SIZE));
                    s.setPosition(j*BLOCK_SIZE,i*BLOCK_SIZE);
                    s.move(28,31);
                    window.draw(s);
                }
            for(int i = 0;i < FIGURE_SIZE;i++)
                {
                s.setTextureRect(IntRect(colorNum * BLOCK_SIZE,0,BLOCK_SIZE,BLOCK_SIZE));
                s.setPosition(a[i].x*BLOCK_SIZE,a[i].y * BLOCK_SIZE);
                s.move(28,31);
                window.draw(s);
                }
            window.draw(frame);
            window.display();

 // Test for end of game, this sort of works and needs to display a message and exit while
            std::cout << a[0].y  <<std::endl;    //@@
            if (a[0].y == 0)
            {
                int str;
                std::cout <<"Game Over!";

                std::cin >>str;
            };               //@@

            }





    return EXIT_SUCCESS;
}

