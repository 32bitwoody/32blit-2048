#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>

#include "2048.hpp"
#include "assets.hpp"

using namespace blit;
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/* setup */
void init() {
  // "asset_dingbads" is the asset name defined in assets.yml
  screen.sprites = SpriteSheet::load(asset_dingbads);
  if(read_save(score_data,0)) {
    // Loaded sucessfully!
  } else {
    // No save file or it failed to load, set up some defaults.
    score_data.highscore = 0;
  }
  if(read_save(game_data,1)) {
	  // success !
  } else {
	  game_data.score=0;
	  game_data.board[5]=0x02;
  }
}
/*--------------------------------------------------------------------*/
void render(uint32_t time_ms) {
  screen.pen = Pen(20, 30, 40);
  screen.clear();

  screen.alpha = 255;
  screen.mask = nullptr;

  // draw title box
  screen.alpha = 255;
  screen.pen = Pen(255, 255, 255);
  screen.rectangle(Rect(0, 0, 320, 14));

  screen.pen = Pen(0, 0, 0);
  screen.text("2048 demo", minimal_font, Point(5, 4));
  
  // draw score box
  screen.pen = Pen(48, 48, 96);
  screen.rectangle(Rect(120, 14, 320, 240));

  screen.pen = Pen(207, 207, 159);
  screen.text("Score", minimal_font, Point(125, 18));	
  std::string sc = std::to_string(game_data.score);
  screen.text(sc, minimal_font, Point(130, 28));
  screen.text("High", minimal_font, Point(125, 52));
    screen.text("score", minimal_font, Point(130, 60));	
	
  sc = std::to_string(score_data.highscore);
  screen.text(sc, minimal_font, Point(130, 70));		

  uint32_t ms_start = now();
  uint32_t t;
  // Draw a sprite using its numerical index in the sprite sheet
  // Treats the sprite sheet as a grid of 8x8 sprites numbered from 0 to 63
  // In this case sprite number 1 is the second sprite from the top row.
  // One should be an apple, two is a banana,.
  // Draw them according to a 16 slot array(4x4)
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (game_data.board[x+y*4]>0) {
		  t=game_data.board[x+y*4]*8;
//        screen.sprite(game_data.board[x+y*4], Point(25*x+15, 25*y+20));
          screen.stretch_blit(screen.sprites, Rect(t, 0,8, 8), Rect(22*x+14, 21*y+24, 20, 20));
//          screen.stretch_blit(screen.sprites, Rect(t, 0,8, 8), Rect(25, 25, 16, 16));

      }
   }
}

  uint32_t ms_end = now();

  // draw FPS meter
  screen.alpha = 255;
  screen.pen = Pen(255, 255, 255, 100);
  screen.rectangle(Rect(1, 120 - 10, 12, 9));
  screen.pen = Pen(255, 255, 255, 200);
  std::string fms = std::to_string(ms_end - ms_start);
  screen.text(fms, minimal_font, Rect(3, 120 - 9, 10, 16));
//  printf("FMS %s",fms);
	
  int block_size = 4;
  for (uint32_t i = 0; i < (ms_end - ms_start); i++) {
    screen.pen = Pen(i * 5, 255 - (i * 5), 0);
    screen.rectangle(Rect(i * (block_size + 1) + 1 + 13, screen.bounds.h - block_size - 1, block_size, block_size));
  }
}
/*--------------------------------------------------------------------*/
void update(uint32_t time) {
// Check for dpad pressed and start move
  if (pressed(Button::DPAD_UP) && moveup==false) {
	moveup=true;
	move(0,1);
	printf("Board moved = %d",boardmoved);
  }
  if (pressed(Button::DPAD_DOWN) && movedown==false) {
	movedown=true;
    move(0,-1);
  }
  if (pressed(Button::DPAD_LEFT) && moveleft==false) {
	moveleft=true;
	move(1,0);
  }
  if (pressed(Button::MENU) && menupress==false) {
  	menupress=true;
  }
  if (pressed(Button::HOME) && homepress==false) {
	homepress=true;
  }
  if (pressed(Button::DPAD_RIGHT) && moveright==false) {
	moveright=true;
    move(-1,0);
  }
//On release place new piece
  if (pressed(Button::DPAD_UP)==false && moveup==true) {
//    printf("Release: Up\n");
    moveup=false;
    add_piece();
  }
  if (pressed(Button::DPAD_DOWN)==false && movedown==true) {
//    printf("Release: Down\n");
    movedown=false;
    add_piece();

  }
  if (pressed(Button::DPAD_LEFT)==false && moveleft==true) {
//    printf("Release: Left\n");
    moveleft=false;
    add_piece();

  }
  if (pressed(Button::DPAD_RIGHT)==false && moveright==true) {
//    printf("Release: Right\n");
    moveright=false;
    add_piece();
  }
  if (pressed(Button::MENU)==false && menupress==true) {
	end_game();
	menupress=false;  
  }
  if (pressed(Button::HOME)==false && homepress==true) {
	
    homepress=false;
  }

}
/*--------------------------------------------------------------------*/
// Look at board and act on pieces above/below, or left/right
void move(int xc,int yc) {
//  printf("Move: \n");
  unsigned int xl=0;
  unsigned int xu=4;
  unsigned int yl=0;
  unsigned int yu=4;
  int xs=1;
  int ys=1;
// Work out how much of board to check
//  and what order
  if(xc==1) {
	xu=3;
  }
  if(xc==-1) {
	xu=0;
	xl=3;
	xs=-1;
  }
  if(yc==1) {
	yu=3;
  }
  if(yc==-1) {
	yu=0;
	yl=3;
	ys=-1;
  }
  
//  printf("X range :%d : %d : %d\n",xl,xu,xs);
//  printf("Y range :%d : %d : %d\n",yl,yu,ys);
  unsigned int i=0;
  unsigned int x=xl;
  unsigned int y=yl;
  while(i<5) {
//	printf("%d,%d ",x,y);
    if (game_data.board[x+y*4]==0x00) {
	  game_data.board[x+y*4]=game_data.board[x+xc+(y+yc)*4];
	  game_data.board[x+xc+(y+yc)*4]=0x00;
	  if(game_data.board[x+y*4]!=0x00) {
	    boardmoved=true;
      }
	}
    if(i>3) {
	  if (game_data.board[x+y*4]==game_data.board[x+xc+(y+yc)*4] && game_data.board[x+y*4]!=0x00) {
	    game_data.board[x+y*4]=game_data.board[x+y*4]+1;
	    game_data.board[x+xc+(y+yc)*4]=0x00;
	    game_data.score=game_data.score+pof2[game_data.board[x+y*4]];
	    if(game_data.score>score_data.highscore) {
		  score_data.highscore=game_data.score;
		}
	  }
    }
    x=x+xs;
    if(x==xu) {
	  y=y+ys;
	  x=xl;
    }
    if(y==yu){
	  i++;
	  y=yl;
    }
  }
}
/*--------------------------------------------------------------------*/
// add new piece to board
void add_piece() {
	write_save(score_data,0);
    write_save(game_data,1);
// only add a new piece if the board moved	
  if(boardmoved==false){
	  return;
  }
  bool place=false;
// add (2) either apple or banana, 3 would add apricots as well
  unsigned int v = (rand() % 2 ) + 1;
//  unsigned int v=1;
  unsigned int lc=0;
  // Good old loop counter to stop us if needed
  while(place==false) {
	lc++;
	if(lc>7){
		break;
	}
	unsigned int x = (rand() % 4);
	unsigned int y = (rand() % 4);
//    printf("Picked %d,%d - %d (%d)\n",x,y,v,lc);
    if (game_data.board[x+y*4]==0x00) {
	  game_data.board[x+y*4]=v;
      place=true;	
      boardmoved=false;
    }
  }

  if(place==true) {
	  return;
  }
//  printf("Cant add by random\n");
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
//	  printf("a%d,%d ",x,y);
	  if (game_data.board[x+y*4]==0x00) {
	  game_data.board[x+y*4]=v;
      boardmoved=false;
      check_board();
	  return;
      }
    }
  }

//  printf("Cant add by system\n");
  check_board();
}
/*--------------------------------------------------------------------*/
void check_board() {
  printf("Check board for end of game\n");
// End of game is a bit scrappy, need to use flags to change render to a 
//pop-up with score and play again.
  endflag=true;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
//	  printf("c%d,%d ",x,y);
	  if (game_data.board[x+y*4]==0x00) {
		endflag=false;
		printf("~1");
	  }
	  if (game_data.board[x+y*4]!=0x00) {
		emptyboard=false;
	  }
	  if(x<3) {
	    if (game_data.board[x+y*4]==game_data.board[x+1+y*4]) {
		  endflag=false;
		  printf("~2");
	    }
	  }
	  if (game_data.board[x+y*4]==game_data.board[x-1+y*4] && x>1) {
		endflag=false;
		printf("~3");
	  }
	  if (game_data.board[x+y*4]==game_data.board[x+(y+1)*4] && y<3) {
		endflag=false;
		printf("~4");
	  }
	  if (game_data.board[x+y*4]==game_data.board[x+(y-1)*4] && y>1) {
		endflag=false;
		printf("~5");
	  }
    }
  }
  if(endflag==false) {
  	printf("Moves avail\n");
  }
  if(endflag==true) {
  	printf("No more moves\n");
    end_game();
    return;
  }
  if(emptyboard==true) {
	boardmoved=true;
	add_piece();
	return;
  }   
}
/*--------------------------------------------------------------------*/
void end_game() {
	printf("give score and clear\n");
// only has one high score and it does not persist. add filestream to 
// store highscores.
	if(game_data.score>score_data.highscore) {
	  score_data.highscore=game_data.score;
	}
	game_data.score=0;
	for (int x = 0; x < 4; x++) {
	for (int y = 0; y < 4; y++) {
		game_data.board[x+y*4]=0x00;
	}
// addpiece will only work if the board has just moved.	
	boardmoved=true;
	add_piece();
  }
}
/*--------------------------------------------------------------------*/
