#pragma once

#include <cstdint>

#include "32blit.hpp"

void init();
void update(uint32_t time);
void render(uint32_t time);
void move(int xc,int yc);
void add_piece(void);
void check_board(void);
void end_game(void);
