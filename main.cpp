#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include <chrono>
#include <thread>
//#########################
//#                       #
//#                       #
//#             S         #
//#           O           #
//#           o           #
//#         ooo           #
//#                       #
//#                       #
//#                       #
//#########################

const int width = 32;
const int height = width/2;
int snake_x = width / 2;
int snake_y = height / 2;
int fruit_x;
int fruit_y;
int score = 0;
bool isGameOver = false;
enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;
int kbhit()
{
  int ch = getch();

  if (ch != ERR) {
    ungetch(ch);
    return 1;
  } else {
    return 0;
  }
}
void SpawnFruit()
{
  fruit_x = rand() % (width - 1);
  fruit_y = rand() % (height -1);
}
void Setup()
{
  initscr();
  SpawnFruit();
}
void Input()
{
  if(kbhit())
  {
    int ch = getch();
    switch(ch)
    {
      case 'w': //wasd
      dir = eDirection::UP;
      break;
      case 'a':
      dir = eDirection::LEFT;
      break;
      case 's':
      dir = eDirection::DOWN;
      break;
      case 'd':
      dir = eDirection::RIGHT;
      break;
      case 'x':
      isGameOver = true;
      break;
    }
  }
}
void Update() 
{
  switch(dir)
  {
    case eDirection::UP:
    snake_y--;
    break;
    case eDirection::LEFT:
    snake_x--;
    break;
    case eDirection::DOWN:
    snake_y++;
    break;
    case eDirection::RIGHT:
    snake_x++;
    break;
  }
  if(snake_x == fruit_x && snake_y == fruit_y)
  {
    SpawnFruit();
    score++;
  }
}
void Draw()
{
  system("clear");
  if(snake_x < 0 || snake_x >= width || snake_y < 0 || snake_y >= height)
  {
    isGameOver = true;
    printf("Nicht Bestanden!\n\rScore: %d\n\rPress enter to exit...", score);
    std::cin.get();
    return;
  }
  printf("Score: %d", score);
  printf("\n\r");
  for(int i = 0; i < width + 2; i++)
  {
    printf("#");
  }
  printf("\n\r");


  for(int i = 0; i < height; i++)
  {
    printf("#");
    for(int j = 0; j < width; j++)
    {
      if(j == snake_x && i == snake_y)
      {
        printf("O");
      }
      else if(j == fruit_x && i == fruit_y)
      {
        printf("F");
      }
      else 
      {
        printf(" ");
      }
    }
    printf("#");
    printf("\n\r");
  }

  for(int i = 0; i < width + 2; i++)
  {
    printf("#");
  }
  printf("\n\r");
}

std::chrono::milliseconds pause_time(145);
int main()
{
  //used to initialize ncurses
  Setup();
  while(!isGameOver)
  {
    //time for getch() to block (ms)
    timeout(5);
    Input();
    Update();
    Draw();
    //limit framerate
    std::this_thread::sleep_for(pause_time);
  }
  endwin();
  return 0;
}
