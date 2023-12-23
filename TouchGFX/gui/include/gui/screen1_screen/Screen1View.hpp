#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <vector>
using namespace std;
//
#define MAX_8BIT 255
// LIMIT FOR BOX BORDER
#define LIMIT_WALLX_RIGHT 220
#define LIMIT_WALLX_LEFT 4
#define LIMIT_WALLY_DOWN 226
#define LIMIT_WALLY_UP 8

// COORDINATE FOR RESPAWN SPRITE AT MIDDLE OF MAP
#define SPAWN_COORDINATE_X 120
#define SPAWN_COORDINATE_Y 125
// DIRECTION FOR SPRITE
#define SPRITE_UP 		-16
#define SPRITE_DOWN 	16
#define SPRITE_LEFT 	-16
#define SPRITE_RIGHT 	16
#define SPRITE_STOP  	0
// DIRECITON FOR FOOD
#define FOOD_UP 	-1
#define FOOD_DOWN 	1
#define FOOD_LEFT 	-1
#define FOOD_RIGHT 	1
#define FOOD_STOP 	0
// SPEED
#define SLOW 1
#define FAST 2
#define DELAY 10
//	RESPAWN
#define NUMBER_PIXEL 16
// ADJUST ALPHA
#define TWENTY_PERCENT_ALPHA 50
#define MAX_ALPHA 255
// CHECK IF OUT OF BOX BORDER
#define OUT_OF_AXIS_X_LEFT 8
#define OUT_OF_AXIS_X_RIGHT 232
#define OUT_OF_AXIS_Y_UP 13
#define OUT_OF_AXIS_Y_DOWN 237
#define MAX_LENGTH_PAIR 4
#define FIRST_DIRECTION 0
#define SECOND_DIRECTION 1
#define THIRD_DIRECTION 2
#define FOURTH_DIRECTION 3
#define BODY_FIRST_SEGMENT 0
#define WIDTH_SEGMENT	16
#define INITIALIZE_SEGMENT 3

struct BoundingBox {
	int16_t left = 0;
	int16_t top = 0;
	int16_t right = 0;
	int16_t bottom = 0;
};
class Screen1View: public Screen1ViewBase {

protected:
	bool isGameStart :0x1;
	bool isFirstPlay :0x1;
	bool isCollideMode :0x1;
	bool isFastMode :0x1;
	bool isFloatMode :0x1;
	bool direction_movement :0x1;
	bool clock_direction :0x1;
	bool isGameRunning :0x1;
	bool isAtMenuGame :0x1;
	int tick_counter;
	int random_direction, random_index;
	int box_progress_value, index, count_segment;
	int Speed, count_straight_line;
	pair<int, int> PairOfDirection[MAX_LENGTH_PAIR];

	int DirectionStraightX[MAX_LENGTH_PAIR / 2];
	int DirectionStraightY[MAX_LENGTH_PAIR / 2];
	uint16_t score_value, high_score_value;
	int8_t Vx, Vy;
	int8_t Food_Vx, Food_Vy;


	Box snakebody[32];
	Box snaketail;
	BoundingBox _food;
	BoundingBox _HeadSnake;
	BoundingBox _BodySnake[32];
	BoundingBox _TailSnake;
	vector<Box*> Snake;

	ScalableImage *Head;

	uint32_t counterTick;

	int body_length;

	bool continue1 = true;
public:

	bool isLeft :0x1;
	bool isRight :0x1;
	bool isUp :0x1;
	bool isDown :0x1;


	Screen1View();
	virtual ~Screen1View() {
	}
	virtual void setupScreen();
	virtual void tearDownScreen();
	virtual void handleTickEvent();
	virtual void BoxBorder();
	virtual void Up_Button_Click();
	virtual void Down_Button_Click();
	virtual void Left_Button_Click();
	virtual void Right_Button_Click();
	virtual void Setting_Button_Click();
	virtual void Float_Button_Click();
	virtual void Fast_Button_Click();
	virtual void Collide_Button_Click();
	virtual void Home_Button_Click();
	virtual void Block_Float_Button_Click();
	virtual void Block_Collide_Button_Click();
	virtual void Retry_Function();
	virtual void Exit_Function();
	virtual void Exit_Home_Click();
	bool isSpriteCollide();
	bool isSpriteRunning();
	bool isFoodCollide();
	bool isHighScore();
	bool CheckEating();
	bool isOutOfBox(int);
	bool isHeadHitBodySnake();
	void TurnOffWidget(int);
	void ChangeSpriteDirection();
	void ChangeFoodDirectionOfStraight();
	void ChangeFoodDirectionOfDiagonal();
	void FadeWidget(int);
	void MenuGame();
	void SpawnPlayer();
	void SetInvisiblePlayer();
	void GenerateFoodFloatMode();
	void GetBoundingBox();
	void InsertBodySnake();
	void SpawnNewPosition(int,int);
	void CheckEachElement();
	void EndGame();
};

#endif // SCREEN1VIEW_HPP
