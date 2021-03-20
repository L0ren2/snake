#include <iostream>
#include <conio.h>
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
bool isGameOver = false;
enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;
//schlange: länge n, head_pos_x, head_pos_y, -> liste (kopf hat pos x und pos y) -> wenn kopf sich bewegt, kommt die alte pos in die nächste node
//-> warum überhaupt so was kompliziertes? erstmal nur x und y als int speichern
int snake_x = width / 2;
int snake_y = height / 2;
void Input()
{
  if(_kbhit())
  {
    switch(_getch())
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
}
void Draw()
{
  system("cls");
  printf("\n");
  for(int i = 0; i < width; i++)
  {
    printf("#");
  }
  printf("\n");


  for(int i = 0; i < height; i++)
  {
    printf("#");
    for(int j = 1; j < width - 1; j++)
    {
      if(j == snake_x && i == snake_y)
      {
        printf("O");
      }
      else 
      {
        printf(" ");
      }
    }
    printf("#");
    printf("\n");
  }

  for(int i = 0; i < width; i++)
  {
    printf("#");
  }
  printf("\n");
}

std::chrono::milliseconds pause_time(150);
int main()
{
  while(!isGameOver)
  {
    Input();
    Update();
    Draw();
    std::this_thread::sleep_for(pause_time);
  }
  return 0;
}
