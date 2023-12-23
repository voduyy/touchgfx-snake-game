#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/Drawable.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Utils.hpp>
#include <stdlib.h>
#include <images/BitmapDatabase.hpp>


//#include <cmsis_os.h>
typedef enum {
	GAME_OVER = 1,
	RETRY = 2,
	HOME = 3,
	TURN_ON_FLOAT = 4,
	TURN_OFF_FLOAT = 5,
	TURN_ON_COLLIDE = 6,
	TURN_OFF_COLLIDE = 7,
	COUNTER_CLOCK_WISE = 0,
	CLOCK_WISE = 1,
	ERROR_SCREEN = 8,
	SETTING_SCREEN = 9,
	DIAGONAL_LINE = 1,
	STRAIGHT_LINE = 0,
	BACK_SETTING = 10,

} state;
Screen1View::Screen1View() {

	isGameStart = false;
	isFirstPlay = true;
	isCollideMode = true;
	isFastMode = false;
	isFloatMode = false;
	isGameRunning = false;
	isAtMenuGame = false;

	isLeft = true;
	isRight = false;
	isUp = false;
	isDown = false;

	tick_counter = MAX_8BIT;
	box_progress_value = 0;
	score_value = 0;

	Speed = SLOW;

	random_direction = CLOCK_WISE;
	random_index = FIRST_DIRECTION;
	direction_movement = DIAGONAL_LINE;

	DirectionStraightX[FIRST_DIRECTION] = FOOD_LEFT;
	DirectionStraightX[SECOND_DIRECTION] = FOOD_RIGHT;
	DirectionStraightY[FIRST_DIRECTION] = FOOD_UP;
	DirectionStraightY[SECOND_DIRECTION] = FOOD_DOWN;
	// initialize food direction mode in diagonal mode

	PairOfDirection[FIRST_DIRECTION] = make_pair(FOOD_LEFT, FOOD_UP);
	PairOfDirection[SECOND_DIRECTION] = make_pair(FOOD_LEFT, FOOD_DOWN);
	PairOfDirection[THIRD_DIRECTION] = make_pair(FOOD_RIGHT, FOOD_DOWN);
	PairOfDirection[FOURTH_DIRECTION] = make_pair(FOOD_RIGHT, FOOD_UP);

	Vx = SPRITE_LEFT;
	Vy = SPRITE_STOP;
	Food_Vx = PairOfDirection[FIRST_DIRECTION].first;
	Food_Vy = PairOfDirection[FIRST_DIRECTION].second;

	Head = &head_left;

	body_length = INITIALIZE_SEGMENT;
	counterTick = 0;
	Box *SnakeHead = &snakebody[BODY_FIRST_SEGMENT];
	(*SnakeHead).setXY(SPAWN_COORDINATE_X, SPAWN_COORDINATE_Y);
	(*SnakeHead).setVisible(false);
	Snake.push_back(SnakeHead);
}

void Screen1View::setupScreen() {

	for (int i = 0; i < 32; i++) {
		snakebody[i].setPosition(302, 282, 16, 16);
		snakebody[i].setColor(touchgfx::Color::getColorFromRGB(72, 118, 236));
		snakebody[i].setVisible(false);
		add (snakebody[i]);
	}

	snaketail.setPosition(302, 282, 16, 16);
	snaketail.setColor(touchgfx::Color::getColorFromRGB(66, 111, 227));
	snaketail.setVisible(false);
	add (snaketail);

	Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}
void Screen1View::handleTickEvent() {		// recall after 16.6 ms ~ 60 Hz

	if (isGameStart == false && isAtMenuGame == false
			&& isGameRunning == false) {

		if (!on_button.isVisible()) {

			boxProgress1.setValue(box_progress_value);
			box_progress_value += 3;

			if (box_progress_value >= 127) {
				box_progress_value = 0;
				boxProgress1.setVisible(false);
				Screen1View::MenuGame();
			}
		} else if (on_button.isVisible()) {

			on_button.setAlpha(tick_counter);
			tick_counter -= 3;
			if (!tick_counter) {
				tick_counter = MAX_8BIT;
			}
		}
	}

	if (isGameStart == true && isGameRunning == true) {

		if (counterTick % (DELAY / Speed) == 0) {
			Box *SnakeHead = Snake[Snake.size() - 1];
			Snake.pop_back();
			(*SnakeHead).setXY((*Snake[BODY_FIRST_SEGMENT]).getX() + Vx,
					(*Snake[BODY_FIRST_SEGMENT]).getY() + Vy);
			//(*SnakeHead).setVisible(true);
			if (isUp) {
				Vx = SPRITE_STOP;
				Vy = SPRITE_UP;
			} else if (isDown) {
				Vx = SPRITE_STOP;
				Vy = SPRITE_DOWN;
			} else if (isLeft) {
				Vx = SPRITE_LEFT;
				Vy = SPRITE_STOP;
			} else if (isRight) {
				Vx = SPRITE_RIGHT;
				Vy = SPRITE_STOP;
			}
			if (!isCollideMode) {
				CheckEachElement();
			}
			Snake.insert(Snake.begin(), SnakeHead);
			snaketail.setXY((*Snake[Snake.size() - 1]).getX(),
					(*Snake[Snake.size() - 1]).getY());
			//snaketail.setVisible(true);
			ChangeSpriteDirection();
		}

		/**/
		if (isFloatMode) {
			food.setXY(food.getX() + Food_Vx, food.getY() + Food_Vy);
			if (direction_movement == DIAGONAL_LINE) {
				if (isFoodCollide()) {
					ChangeFoodDirectionOfDiagonal();
				}
			} else if (direction_movement == STRAIGHT_LINE) {
				if (isFoodCollide()) {
					ChangeFoodDirectionOfStraight();
				}
			}
		}
		if (isCollideMode) {
			if (isSpriteCollide()) {
				EndGame();
			}
		} else if (!isCollideMode) {

			if (isOutOfBox (BODY_FIRST_SEGMENT)) {
				if (isUp) {
					SpawnNewPosition((*Head).getX(), OUT_OF_AXIS_Y_DOWN);
				} else if (isDown) {
					SpawnNewPosition((*Head).getX(), OUT_OF_AXIS_Y_UP);
				} else if (isRight) {
					SpawnNewPosition(OUT_OF_AXIS_X_LEFT, (*Head).getY());
				} else if (isLeft) {
					SpawnNewPosition(OUT_OF_AXIS_X_RIGHT, (*Head).getY());
					//touchgfx_printf("hello");
				}
				CheckEachElement();
			}
			if(isHeadHitBodySnake())
			{
				EndGame();
			}
		}
		if (CheckEating()) {
			InsertBodySnake();
			if (isFloatMode) {
				GenerateFoodFloatMode();
			}
			score_value++;
			if (isFirstPlay) {
				high_score_value = score_value;
			} else if (isHighScore()) {
				high_score_value = score_value;
			}

			food.setXY(8 + 16 * (rand() % 13), 13 + 16 * (rand() % 13));
			food.setVisible(true);
			Unicode::snprintf(high_score_textBuffer, HIGH_SCORE_TEXT_SIZE, "%d",
					high_score_value);
			Unicode::snprintf(Score_textBuffer, SCORE_TEXT_SIZE, "%d",
					score_value);
		}
	}
	counterTick++;
	invalidate();
}
void Screen1View::EndGame() {
	Exit.setVisible(true);
	retry.setVisible(true);
	game_over.setVisible(true);
	game_background_over.setVisible(true);
	high_score_game_over.setVisible(true);
	high_score_game_over_text.setVisible(true);
	food_game_over.setVisible(true);
	score_game_over_text.setVisible(true);
	Unicode::snprintf(high_score_game_over_textBuffer,
			HIGH_SCORE_GAME_OVER_TEXT_SIZE, "%d", high_score_value);
	Unicode::snprintf(score_game_over_textBuffer, SCORE_GAME_OVER_TEXT_SIZE,
			"%d", score_value);
	TurnOffWidget(GAME_OVER);

	Vx = SPRITE_STOP;
	Vy = SPRITE_STOP;
	isFirstPlay = false;
	isGameRunning = false;

}
bool Screen1View::isHeadHitBodySnake() {
	for (int index_snake = 1; index_snake < body_length; index_snake++) {
		if ((*Head).getX() == (*Snake[index_snake]).getX()
				&& (*Head).getY() == (*Snake[index_snake]).getY()) {
			return true;
		}
	}
	return false;
}
void Screen1View::InsertBodySnake() {
	body_length++;
	Box *SnakeHead = &snakebody[body_length - 1];
	(*SnakeHead).setXY((*Snake[BODY_FIRST_SEGMENT]).getX() + Vx,
			(*Snake[BODY_FIRST_SEGMENT]).getY() + Vy);
	(*SnakeHead).setVisible(true);
	Snake.insert(Snake.begin(), SnakeHead);
}
void Screen1View::GetBoundingBox() {
	_HeadSnake.left = (*Head).getX();
	_HeadSnake.top = (*Head).getY();
	_HeadSnake.right = _HeadSnake.left + (*Head).getWidth();
	_HeadSnake.bottom = _HeadSnake.top + (*Head).getHeight();

	_food.left = food.getX();
	_food.top = food.getY();
	_food.right = _food.left + food.getWidth();
	_food.bottom = _food.top + food.getHeight();

	for (int index_snake = 0; index_snake < body_length; index_snake++) {

		_BodySnake[index_snake].left = (*Snake[index_snake]).getX();
		_BodySnake[index_snake].top = (*Snake[index_snake]).getY();
		_BodySnake[index_snake].right = _BodySnake[index_snake].left
				+ (*Snake[index_snake]).getWidth();
		_BodySnake[index_snake].bottom = _BodySnake[index_snake].top
				+ (*Snake[index_snake]).getHeight();
	}

	_TailSnake.left = snaketail.getX();
	_TailSnake.top = snaketail.getY();
	_TailSnake.right = _TailSnake.left + snaketail.getWidth();
	_TailSnake.bottom = _TailSnake.top + snaketail.getHeight();
}
void Screen1View::CheckEachElement() {
	int index_snake = 0;
	for (index_snake = 0; index_snake < body_length - 1; index_snake++) {
		if (isOutOfBox(index_snake)) {
			(*Snake[index_snake]).setVisible(false);
		} else {
			(*Snake[index_snake]).setVisible(true);
		}
	}
	if (isOutOfBox(index_snake)) {
		(*Snake[index_snake]).setVisible(false);
		snaketail.setVisible(false);
	} else {
		(*Snake[index_snake]).setVisible(true);
		snaketail.setVisible(true);
	}
}
void Screen1View::GenerateFoodFloatMode(void) {
	direction_movement = rand() % 2;
	if (direction_movement == DIAGONAL_LINE) {
		random_direction = rand() % 2;
		random_index = rand() % 4;
		Food_Vx = PairOfDirection[random_index].first;
		Food_Vy = PairOfDirection[random_index].second;

	} else if (direction_movement == STRAIGHT_LINE) {
		random_index = rand() % 2;
		if (count_straight_line % 2 != 0) {
			Food_Vy = DirectionStraightY[random_index];
			Food_Vx = FOOD_STOP;
		} else if (count_straight_line % 2 == 0) {
			Food_Vx = DirectionStraightX[random_index];
			Food_Vy = FOOD_STOP;
		}
		count_straight_line++;
	}
}
void Screen1View::ChangeFoodDirectionOfDiagonal() {
	if (random_direction == COUNTER_CLOCK_WISE) {
		random_index++;
		if (random_index >= MAX_LENGTH_PAIR) {
			random_index = FIRST_DIRECTION;
		}
		Food_Vx = PairOfDirection[random_index].first;
		Food_Vy = PairOfDirection[random_index].second;
		return;
	}
	if (random_direction == CLOCK_WISE) {
		random_index--;
		if (random_index < FIRST_DIRECTION) {
			random_index = FOURTH_DIRECTION;
		}
		Food_Vx = PairOfDirection[random_index].first;
		Food_Vy = PairOfDirection[random_index].second;
		return;
	}

}
void Screen1View::ChangeFoodDirectionOfStraight() {
	switch (Food_Vy) {
	case FOOD_UP:
		Food_Vy = FOOD_DOWN;
		break;
	case FOOD_DOWN:
		Food_Vy = FOOD_UP;
		break;
	default:
		break;
	}
	switch (Food_Vx) {
	case FOOD_RIGHT:
		Food_Vx = FOOD_LEFT;
		break;
	case FOOD_LEFT:
		Food_Vx = FOOD_RIGHT;
		break;
	default:
		break;
	}
}
bool Screen1View::isOutOfBox(int index_snake) {
	return ((*Snake[index_snake]).getX() < OUT_OF_AXIS_X_LEFT
			|| (*Snake[index_snake]).getX() > OUT_OF_AXIS_X_RIGHT
			|| (*Snake[index_snake]).getY() < OUT_OF_AXIS_Y_UP
			|| (*Snake[index_snake]).getY() > OUT_OF_AXIS_Y_DOWN-16);
}
void Screen1View::MenuGame() {
	start_game_snake.setVisible(true);
	setting_button.setVisible(true);
	start_button.setVisible(true);
	loading_image.setVisible(false);
	exit_home.setVisible(true);
	isAtMenuGame = true;
	isGameStart = true;

}
bool Screen1View::isHighScore() {
	return score_value > high_score_value;
}
void Screen1View::Retry_Function() {
	score_value = 0;
	Unicode::snprintf(Score_textBuffer, SCORE_TEXT_SIZE, "%d", score_value);
	Screen1View::BoxBorder();
	TurnOffWidget(RETRY);
}

void Screen1View::Exit_Function() {
	Exit.setVisible(false);
	retry.setVisible(false);
	game_over.setVisible(false);
	game_background_over.setVisible(false);
	high_score_game_over.setVisible(false);
	high_score_game_over_text.setVisible(false);
	food_game_over.setVisible(false);
	score_game_over_text.setVisible(false);

	isGameStart = false;
	isGameRunning = false;
	Screen1View::MenuGame();

}

void Screen1View::TurnOffWidget(int state) {
	switch (state) {
	case GAME_OVER:
		MyContainer.setVisible(false);
		Score_text.setVisible(false);
		up_button.setVisible(false);
		down_button.setVisible(false);
		right_button.setVisible(false);
		left_button.setVisible(false);
		food.setVisible(false);
		game_outside.setVisible(false);
		game_background.setVisible(false);
		food_score_image.setVisible(false);
		high_score_image.setVisible(false);
		Score_text.setVisible(false);
		high_score_text.setVisible(false);
		SetInvisiblePlayer();
		break;
	case RETRY:
		Exit.setVisible(false);
		retry.setVisible(false);
		game_over.setVisible(false);
		game_outside.setVisible(true);
		game_background_over.setVisible(false);
		high_score_game_over.setVisible(false);
		high_score_game_over_text.setVisible(false);
		food_game_over.setVisible(false);
		score_game_over_text.setVisible(false);
		break;
	default:
		break;
	}
}
void Screen1View::SetInvisiblePlayer() {
	(*Head).setVisible(false);
	for (int index_snake = 0; index_snake < body_length; index_snake++) {
		(*Snake[index_snake]).setVisible(false);
	}
	snaketail.setVisible(false);
}
bool Screen1View::CheckEating() {
	GetBoundingBox();
	if (!isFloatMode) {
		return ((*Head).getX() == food.getX() && (*Head).getY() == food.getY());
	} else if (isFloatMode) {

		return (_HeadSnake.left < _food.right && _food.left < _HeadSnake.right
				&& _HeadSnake.top < _food.bottom
				&& _HeadSnake.bottom > _food.top);
	}
	return false;
}

bool Screen1View::isSpriteCollide() {
	if (isSpriteRunning()) {
		if ((*Head).getX() >= LIMIT_WALLX_RIGHT
				|| (*Head).getX() <= LIMIT_WALLX_LEFT) {
			Vx = SPRITE_STOP;
			return true;
		}
		if ((*Head).getY() >= LIMIT_WALLY_DOWN
				|| (*Head).getY() <= LIMIT_WALLY_UP) {
			Vy = SPRITE_STOP;
			return true;
		}
	}
	return false;
}
bool Screen1View::isFoodCollide() {
	GetBoundingBox();
	if (food.getX() >= LIMIT_WALLX_RIGHT || food.getX() <= LIMIT_WALLX_LEFT) {
		return true;
	}
	if (food.getY() >= LIMIT_WALLY_DOWN || food.getY() <= LIMIT_WALLY_UP) {
		return true;
	}
	for (int index_snake = 0; index_snake < body_length; index_snake++) {
		if ((_BodySnake[index_snake].left < _food.right
				&& _food.left < _BodySnake[index_snake].right
				&& _BodySnake[index_snake].top < _food.bottom
				&& _BodySnake[index_snake].bottom > _food.top)) {
			return true;
		}
	}
	if ((_TailSnake.left < _food.right && _food.left < _TailSnake.right
			&& _TailSnake.top < _food.bottom && _TailSnake.bottom > _food.top)) {
		return true;
	}
	return false;
}
bool Screen1View::isSpriteRunning() {
	return (Vx != SPRITE_STOP && Vy == SPRITE_STOP)
			|| (Vy != SPRITE_STOP && Vx == SPRITE_STOP);
}

void Screen1View::BoxBorder() {
	MyContainer.setVisible(true);
	Score_text.setVisible(true);
	up_button.setVisible(true);
	down_button.setVisible(true);
	right_button.setVisible(true);
	left_button.setVisible(true);
	food.setVisible(true);
	game_background.setVisible(true);
	food_score_image.setVisible(true);
	high_score_image.setVisible(true);
	Score_text.setVisible(true);
	high_score_text.setVisible(true);
	game_outside.setVisible(true);

	start_game_snake.setVisible(false);
	setting_button.setVisible(false);
	exit_home.setVisible(false);

	Vx = SPRITE_LEFT;
	Vy = SPRITE_STOP;

	score_value = 0;
	Unicode::snprintf(Score_textBuffer, SCORE_TEXT_SIZE, "%d", score_value);

	food.setXY(8 + 16 * (rand() % 13), 13 + 16 * (rand() % 13));
	isRight = false;
	Screen1View::Left_Button_Click();
	isGameRunning = true;
	isGameStart = true;
	SpawnPlayer();
}
void Screen1View::SpawnNewPosition(int CoordinateX, int CoordinateY) {
	Box *SnakeHead = Snake[Snake.size() - 1];
	Snake.pop_back();
	(*SnakeHead).setXY(CoordinateX, CoordinateY);
	Snake.insert(Snake.begin(), SnakeHead);
	(*Head).setXY((*Snake[BODY_FIRST_SEGMENT]).getX() + Vx,
			(*Snake[BODY_FIRST_SEGMENT]).getY() + Vy);
	(*Head).setVisible(true);
	snaketail.setXY((*Snake[Snake.size() - 1]).getX(),
			(*Snake[Snake.size() - 1]).getY());
}
void Screen1View::SpawnPlayer() {
	body_length = INITIALIZE_SEGMENT;
	(*Head).setVisible(false);
	Head = &head_left;
	Snake.clear();
	for (int index_snake = 0; index_snake < body_length; index_snake++) {
		Box *SnakeHead = &snakebody[index_snake];
		(*SnakeHead).setXY(SPAWN_COORDINATE_X + WIDTH_SEGMENT * index_snake,
		SPAWN_COORDINATE_Y);
		(*SnakeHead).setVisible(true);
		Snake.push_back(SnakeHead);
	}
	Vx = SPRITE_LEFT;
	Vy = SPRITE_STOP;
	(*Head).setXY((*Snake[BODY_FIRST_SEGMENT]).getX() + Vx,
			(*Snake[BODY_FIRST_SEGMENT]).getY() + Vy);
	(*Head).setVisible(true);
	snaketail.setXY((*Snake[Snake.size() - 1]).getX(),
			(*Snake[Snake.size() - 1]).getY());
	snaketail.setVisible(true);
}
void Screen1View::ChangeSpriteDirection() {

	(*Head).setVisible(false);
	if (isUp) {
		Head = &head_up;
	} else if (isDown) {
		Head = &head_down;
	} else if (isLeft) {
		Head = &head_left;
	} else if (isRight) {
		Head = &head_right;
	}
	(*Head).setVisible(true);
	(*Head).setXY((*Snake[BODY_FIRST_SEGMENT]).getX() + Vx,
			(*Snake[BODY_FIRST_SEGMENT]).getY() + Vy);

}

void Screen1View::Up_Button_Click() {

	if (isOutOfBox (BODY_FIRST_SEGMENT)) {
		return;
	}

	if (isDown)
		return;

	isUp = true;
	isDown = false;
	isLeft = false;
	isRight = false;

}
void Screen1View::Down_Button_Click() {

	if (isOutOfBox (BODY_FIRST_SEGMENT)) {
		return;
	}
	if (isUp)
		return;

	isUp = false;
	isDown = true;
	isLeft = false;
	isRight = false;
}

void Screen1View::Left_Button_Click() {

	if (isOutOfBox (BODY_FIRST_SEGMENT)) {
		return;
	}
	if (isRight)
		return;

	isUp = false;
	isDown = false;
	isLeft = true;
	isRight = false;

}
void Screen1View::Right_Button_Click() {

	if (isOutOfBox (BODY_FIRST_SEGMENT)) {
		return;
	}
	if (isLeft)
		return;

	isUp = false;
	isDown = false;
	isLeft = false;
	isRight = true;

}
void Screen1View::Setting_Button_Click() {
	Screen1View::FadeWidget(SETTING_SCREEN);
}
void Screen1View::Float_Button_Click() {
	if (!isCollideMode) {
		Block_Float_Container.setVisible(true);
		Block_Float_Button.setVisible(true);
		error_label_image.setVisible(true);
		float_button.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_OFF_USE_ID),
				touchgfx::Bitmap(BITMAP_BUTTON_ON_REMOVEBG_PREVIEW_ID));
		Screen1View::FadeWidget(ERROR_SCREEN);
		return;
	}
	if (!isFloatMode) {
		isFloatMode = true;
		Screen1View::FadeWidget(TURN_OFF_COLLIDE);
	} else if (isFloatMode) {
		isFloatMode = false;
		Screen1View::FadeWidget(TURN_ON_COLLIDE);
	}
}
void Screen1View::Fast_Button_Click() {
	if (!isFastMode) {
		isFastMode = true;
		Speed = FAST;
	} else if (isFastMode) {
		isFastMode = false;
		Speed = SLOW;
	}
}
void Screen1View::Collide_Button_Click() {
	if (isFloatMode) {
		Block_Collide_Container.setVisible(true);
		Block_Collide_Button.setVisible(true);
		error_label_image.setVisible(true);
		collide_button.setBitmaps(
				touchgfx::Bitmap(BITMAP_BUTTON_ON_REMOVEBG_PREVIEW_ID),
				touchgfx::Bitmap(BITMAP_BUTTON_OFF_USE_ID));
		Screen1View::FadeWidget(ERROR_SCREEN);
		return;
	}
	if (isCollideMode) {
		isCollideMode = false;
		Screen1View::FadeWidget(TURN_OFF_FLOAT);
	} else if (!isCollideMode) {
		isCollideMode = true;
		Screen1View::FadeWidget(TURN_ON_FLOAT);
	}
}
void Screen1View::Home_Button_Click() {
	Screen1View::FadeWidget(HOME);

}
void Screen1View::Block_Float_Button_Click() {
	Block_Float_Container.setVisible(false);
	Block_Float_Button.setVisible(false);

	Screen1View::FadeWidget(BACK_SETTING);
}
void Screen1View::Block_Collide_Button_Click() {
	Block_Collide_Container.setVisible(false);
	Block_Collide_Button.setVisible(false);

	Screen1View::FadeWidget(BACK_SETTING);

}
void Screen1View::FadeWidget(int state) {
	switch (state) {
	case SETTING_SCREEN:
		start_button.setVisible(false);
		start_game_snake.setAlpha(TWENTY_PERCENT_ALPHA);
		setting_button.setVisible(false);
		exit_home.setVisible(false);
		Setting_Container.setVisible(true);
		home_button.setVisible(true);
		setting_screen.setVisible(true);
		setting_label_image.setVisible(true);
		break;
	case ERROR_SCREEN:
		setting_screen.setAlpha(TWENTY_PERCENT_ALPHA * 2);
		setting_label_image.setAlpha(TWENTY_PERCENT_ALPHA * 2);
		home_button.setVisible(false);
		float_button.setVisible(false);
		fast_button.setVisible(false);
		collide_button.setVisible(false);
		break;
	case TURN_OFF_COLLIDE:
		collide_text.setAlpha(TWENTY_PERCENT_ALPHA);
		collide_button.setAlpha(TWENTY_PERCENT_ALPHA);
		break;
	case TURN_ON_COLLIDE:
		collide_text.setAlpha(MAX_ALPHA);
		collide_button.setAlpha(MAX_ALPHA);
		break;
	case TURN_OFF_FLOAT:
		float_text.setAlpha(TWENTY_PERCENT_ALPHA);
		float_button.setAlpha(TWENTY_PERCENT_ALPHA);
		break;
	case TURN_ON_FLOAT:
		float_text.setAlpha(MAX_ALPHA);
		float_button.setAlpha(MAX_ALPHA);
		break;
	case HOME:
		start_button.setVisible(true);
		start_button.setAlpha(MAX_ALPHA);
		start_game_snake.setAlpha(MAX_ALPHA);
		setting_button.setAlpha(MAX_ALPHA);
		setting_label_image.setVisible(false);
		setting_button.setVisible(true);
		Setting_Container.setVisible(false);
		home_button.setVisible(false);
		setting_screen.setVisible(false);
		exit_home.setVisible(true);
		break;
	case BACK_SETTING:
		home_button.setVisible(true);
		float_button.setVisible(true);
		fast_button.setVisible(true);
		collide_button.setVisible(true);
		exit_home.setVisible(false);
		setting_label_image.setVisible(true);
		setting_screen.setAlpha(MAX_ALPHA);
		setting_label_image.setAlpha(MAX_ALPHA);
		error_label_image.setVisible(false);
		break;
	}
}
void Screen1View::Exit_Home_Click() {
	high_score_value = 0;
	exit_home.setVisible(false);
	start_game_snake.setVisible(false);
	setting_button.setVisible(false);
	start_button.setVisible(false);
	exit_home.setVisible(false);
	isAtMenuGame = false;
	isGameStart = false;
	isGameRunning = false;
	on_button.setVisible(true);
}
// snake++ by VD & QB
