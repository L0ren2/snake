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
const int snake_array_size = width * height + 2;
int snake_array_x[snake_array_size];
int snake_array_y[snake_array_size];
int fruit_x;
int fruit_y;
int score = 0;
bool isGameOver = false;
enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;
void SpawnFruit()
{
  fruit_x = rand() % (width - 1);
  fruit_y = rand() % (height -1);
}
void Setup()
{
  snake_array_x[0] = width / 2;
  snake_array_y[0] = height / 2;
  for(int i = 1; i < snake_array_size; i++)
  {
    snake_array_x[i] = -1;
    snake_array_y[i] = -1;
  }
  SpawnFruit();
}
void Input()
{
  if(_kbhit())
  {
    switch(_getch())
    {
      case 'w': //wasd
        if(dir != eDirection::DOWN)
          dir = eDirection::UP;
        break;
      case 'a':
        if(dir != eDirection::RIGHT)
          dir = eDirection::LEFT;
        break;
      case 's':
        if(dir != eDirection::UP)
          dir = eDirection::DOWN;
        break;
      case 'd':
        if(dir != eDirection::LEFT)
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
  int old_pos_x = snake_array_x[0];
  int old_pos_y = snake_array_y[0];
  switch(dir)
  {
    case eDirection::UP:
      snake_array_y[0]--;
      break;
    case eDirection::LEFT:
      snake_array_x[0]--;
      break;
    case eDirection::DOWN:
      snake_array_y[0]++;
      break;
    case eDirection::RIGHT:
      snake_array_x[0]++;
      break;
  }
  if(snake_array_x[0] == fruit_x && snake_array_y[0] == fruit_y)
  {
    SpawnFruit();
    score++;
  }
  //update the tail
  for(int i = 1; i <= score; i++)
  {
    int temp_x, temp_y;

    temp_x = snake_array_x[i];
    snake_array_x[i] = old_pos_x;
    old_pos_x = temp_x;

    temp_y = snake_array_y[i];
    snake_array_y[i] = old_pos_y;
    old_pos_y = temp_y;
  }
}
bool wall_hit(){ return snake_array_x[0] < 0 || snake_array_x[0] >= width || snake_array_y[0] < 0 || snake_array_y[0] >= height; }
bool tail_hit()
{
  for(int i = 1; i <= score; i++)
  {
    if(snake_array_x[0] == snake_array_x[i] && snake_array_y[0] == snake_array_y[i])
      return true;
  }
  return false;
}
void Draw()
{
  system("cls");
  if(wall_hit() || tail_hit())
  {
    isGameOver = true;
    printf("Nicht Bestanden!\nScore: %d\nPress enter to quit...\n", score);
    do { } while (std::cin.get() != '\n');
    return;
  }
  printf("Score: %d", score);
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
      bool print_o = false;
      for(int scr_cnt = 1; scr_cnt <= score; scr_cnt++)
      {
        if(i == snake_array_y[scr_cnt] && j == snake_array_x[scr_cnt])
        {
          print_o = true;
          break;
        }
      }
      if(j == snake_array_x[0] && i == snake_array_y[0])
      {
        printf("O");
      }
      else if(print_o)
      {
        printf("o");
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
  Setup();
  while(!isGameOver)
  {
    Input();
    Update();
    Draw();
    std::this_thread::sleep_for(pause_time);
  }
  return 0;
}
