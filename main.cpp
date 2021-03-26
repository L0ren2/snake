#include <iostream>
#include <stdio.h>
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
const int height = width / 2;
int snake_x = width / 2;
int snake_y = height / 2;
int fruit_x;
int fruit_y;
int score = 0;
bool isGameOver = false;
bool isFruitEaten = true;
enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;

void SpawnFruitIf(bool& fruitEaten)
{
  if(fruitEaten)
  {
    fruit_x = rand() % (width - 1);
    fruit_y = rand() % (height -1);
  }
  fruitEaten = false;
}
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
  if(snake_x == fruit_x && snake_y == fruit_y)
  {
    isFruitEaten = true;
    score++;
  }
}
void Draw()
{
  system("cls");
  if(snake_x < 0 || snake_x >= width || snake_y < 0 || snake_y >= height)
  {
    isGameOver = true;
    printf("Nicht Bestanden!\nScore: %d\nPress enter to quit...\n", score);
    std::cin.get();
    return;
  }
  printf("Score: %d", score);
  //DEBUG:
  //printf("  F_X: %d, F_Y: %d", fruit_x, fruit_y);
  printf("\n");
  for(int i = 0; i < width + 2; i++)
  {
    printf("#");
  }
  printf("\n");


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
    printf("\n");
  }

  for(int i = 0; i < width + 2; i++)
  {
    printf("#");
  }
  printf("\n");
}

std::chrono::milliseconds pause_time(145);
int main()
{
  while(!isGameOver)
  {
    Input();
    Update();
    SpawnFruitIf(isFruitEaten);
    Draw();
    std::this_thread::sleep_for(pause_time);
  }
  return 0;
}
