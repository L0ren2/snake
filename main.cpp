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
bool is_game_over = false;
enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;

//ncurses does not have this natively
//kbhit checks whether the user pressed any button
bool kbhit()
{
  int ch = getch();

  if (ch != ERR) {
    ungetch(ch);
    return true;
  } else {
    return false;
  }
}

void spawn_fruit()
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
  spawn_fruit();
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
      is_game_over = true;
      break;
    }
  }
}
void Update() 
{
  //for updating tail:
  //old_node starts at pos of head but moves along the length of the tail
  int old_node_pos_x = snake_array_x[0];
  int old_node_pos_y = snake_array_y[0];

  //move head
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
    spawn_fruit();
    score++;
  }

  //update the tail
  for(int i = 1; i <= score; i++)
  {
    //swap principle:
    int temp_x, temp_y;
    
    temp_x = old_node_pos_x;
    old_node_pos_x = snake_array_x[i];
    snake_array_x[i] = temp_x;

    temp_y = old_node_pos_y;
    old_node_pos_y = snake_array_y[i];
    snake_array_y[i] = temp_y;    
  }
}
bool is_snake_out_of_bounds()
{
  return snake_array_x[0] < 0 || snake_array_x[0] >= width || snake_array_y[0] < 0 || snake_array_y[0] >= height;
}
bool is_snake_tail_hit() 
{
  //check if head_x == node_x[i] for each node < score
  for(int i = 1; i < score; i++)
  {
    if(snake_array_x[0] == snake_array_x[i] && snake_array_y[0] == snake_array_y[i])
      return true;
  }
  return false;
}
void Draw()
{
  system("clear");

  if(is_snake_out_of_bounds() || is_snake_tail_hit())
  {
    is_game_over = true;
    printf("Nicht Bestanden!\n\rScore: %d\n\rPress enter to exit...", score);
    do { /*wait for enter to be pressed*/ } while (getchar() != '\r');
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
      bool should_print_o = false;
      //check if there is a node at [j , i], that needs to be printed
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
  while(!is_game_over)
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
