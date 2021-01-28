#pragma once

#include <cstdint>

#include "32blit.hpp"

//  Dialog dialog;

  struct ScoreSave {
    char name[10];
    uint16_t highscore;
  };
  struct GameSave {
    char name[10];
    uint16_t score;
    uint8_t board[16]= {0};
  };

  ScoreSave score_data;
  GameSave game_data;
  
  const uint16_t screen_width = 320;
  const uint16_t screen_height = 240;

// Hold array of scores for each fruit. 
//  Saves generating powers of 2 on the fly
  std::vector<uint16_t> pof2 = {
	0x00, 0x01, 0x02, 0x04, 0x08, 
	      0x10, 0x20, 0x40, 0x80, 
	      0x100, 0x200, 0x400, 0x800, 
	      0x1000
  };
// Set up some flags
  bool emptyboard=true;
  bool endflag=true;
// Flags for stick movement, so we can track release  
  bool moveup=false;
  bool movedown=false;
  bool moveleft=false;
  bool moveright=false;
  bool boardmoved=false;
  int tick_count = 0;
// score and single highscore
//  unsigned int score=0;
//  unsigned int score_data.highscore=0;

void init();
void update(uint32_t time);
void render(uint32_t time);
void move(int xc,int yc);
void add_piece(void);
void check_board(void);
void end_game(void);
