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
const int snake_array_size = width * height + 2;
int snake_array_x[snake_array_size];
int snake_array_y[snake_array_size];
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
  //init ncurses
  initscr();
  //place snake head in middle
  snake_array_x[0] = width / 2;
  snake_array_y[0] = height / 2;
  for(int i = 1; i < snake_array_size; i++)
  {
    snake_array_x[i] = -1;
    snake_array_y[i] = -1;
  }
  //make the first fruit
  SpawnFruit();
}
void End()
{
  endwin();
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
  int last_iteration_node_x = snake_array_x[0];
  int last_iteration_node_y = snake_array_y[0];
  //set new head pos
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
  //check if fruit was eaten
  if(snake_array_x[0] == fruit_x && snake_array_y[0] == fruit_y)
  {
    SpawnFruit();
    score++;
  }

  //update the tail
  for(int i = 1; i <= score; i++)
  {
    //swap principle:
    int temp_x, temp_y;
    
    temp_x = last_iteration_node_x;
    last_iteration_node_x = snake_array_x[i];
    snake_array_x[i] = temp_x;

    temp_y = last_iteration_node_y;
    last_iteration_node_y = snake_array_y[i];
    snake_array_y[i] = temp_y;    
  }
}
void Draw()
{
  system("clear");
  if(snake_array_x[0] < 0 || snake_array_x[0] >= width || snake_array_y[0] < 0 || snake_array_y[0] >= height)
  {
    isGameOver = true;
    printf("Nicht Bestanden!\n\rScore: %d\n\rPress enter to exit...", score);
    std::cin.get();
    return;
  }
  printf("Score: %d", score);
  //DEBUG
  /*printf("   POS_HEAD_X: %d", snake_array_x[0]);
  printf("   POS_HEAD_Y: %d", snake_array_y[0]);

  printf("   POS_o_X: %d", snake_array_x[1]);
  printf("   POS_o_Y: %d", snake_array_y[1]);*/
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
      bool should_print_o = false;
      for(int scr_cnt = 1; scr_cnt <= score; scr_cnt++)
      {
        if(j == snake_array_x[scr_cnt] && i == snake_array_y[scr_cnt])
        {
          should_print_o = true;
          break;
        }
      }
      if(j == snake_array_x[0] && i == snake_array_y[0])
      {
        printf("O");
      }
      else if(should_print_o)
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
  End();
  return 0;
}
