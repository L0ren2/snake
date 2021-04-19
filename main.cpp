#include <iostream>
#include <windows.h>  
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

char game_board_buffer[height+1][width + 2];
char prev_board_buffer[height+1][width + 2];

int snake_array_x[snake_array_size];
int snake_array_y[snake_array_size];

int fruit_x;
int fruit_y;

int score = 0;
bool isGameOver = false;

enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;

// Functions for managing output //
void setCursorPosition(int x, int y)
{
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  std::cout.flush();
  COORD coord = { (SHORT)x, (SHORT)y };
  SetConsoleCursorPosition(hOut, coord);
}
void cls()
{
  //some extra padding to ensure that the screen is empty afterwards
  for(int y = 0; y < height + 10; y++)
  {
    for(int x = 0; x < width + 10; x++)
    {
      setCursorPosition(x, y);
      std::cout << " ";
    }
  }
  setCursorPosition(0, 0);
}
void mk_empty_board()
{
  for(int x = 0; x < width+1; x++)
  {
    game_board_buffer[0][x] = '#';
    game_board_buffer[height][x] = '#';
  }
  for(int y = 1; y < height; y++)
  {
    game_board_buffer[y][0] = '#';
    game_board_buffer[y][width] = '#';
    for(int x = 1; x < width; x++)
    {
      game_board_buffer[y][x] = ' ';
    }
  }
}
// Functions for managing output //


bool wall_hit(){ return snake_array_x[0] <= 0 || snake_array_x[0] >= width || snake_array_y[0] <= 0 || snake_array_y[0] >= height; }
bool tail_hit()
{
  for(int i = 1; i <= score; i++)
  {
    if(snake_array_x[0] == snake_array_x[i] && snake_array_y[0] == snake_array_y[i])
      return true;
  }
  return false;
}
void SpawnFruit()
{
  fruit_x = (rand() % (width - 1)) + 1;
  fruit_y = (rand() % (height -1)) + 1;
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
  memset((char*)game_board_buffer, 0, (height+1) * (width + 2));
  memset((char*)prev_board_buffer, 0, (height+1) * (width + 2));
  mk_empty_board();
  system("cls");
  SpawnFruit();
}
void Cleanup()
{
  //empty the whole terminal. everything.
  system("cls");
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
void Draw()
{
  if(wall_hit() || tail_hit())
  {
    isGameOver = true;
    cls();
    setCursorPosition(0, 0);
    std::cout << "Nicht Bestanden!\nScore: " << score << "\nPress enter to quit...\n";
    do { } while (std::cin.get() != '\n');
    return;
  }
  setCursorPosition(0, 0);
  std::cout << "Score: " << score << "\n";
  setCursorPosition(0, 0);
  int start_x = 0;
  int start_y = 3;
  
  for(int y = 0; y < height+1; y++)
  {
    for(int x = 0; x < width+2; x++)
    {

      bool print_o = false;
      for(int scr_cnt = 1; scr_cnt <= score; scr_cnt++)
      {
        if(y == snake_array_y[scr_cnt] && x == snake_array_x[scr_cnt])
        {
          print_o = true;
          break;
        }
      }
      if(x == snake_array_x[0] && y == snake_array_y[0])
      {
        game_board_buffer[y][x] = 'O';
      }
      else if(print_o)
      {
        game_board_buffer[y][x] = 'o';
      }
      else if(x == fruit_x && y == fruit_y)
      {
        game_board_buffer[y][x] = 'F';
      }


      if(game_board_buffer[y][x] == prev_board_buffer[y][x])
      {
        continue;
      }
      setCursorPosition(x + start_x, y + start_y);
      std::cout << game_board_buffer[y][x];
      setCursorPosition(0, 0);
    }
  }
  std::cout.flush();
  memcpy(prev_board_buffer, game_board_buffer, height*(width+2));
  mk_empty_board();
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
  Cleanup();
  return 0;
}
