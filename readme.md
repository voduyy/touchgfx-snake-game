# STM32F429I_DISCO_REV_D01 TBS#
This TBS is configured for 320 x 240 pixels 16bpp screen resolution.  <br />
Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ  - Pin PE2, RENDER_TIME - Pin PE3, FRAME_RATE  - Pin PE4, MCU_ACTIVE  - Pin PE5
<br />
# Detail project game
This game is called **SNAKE++** <br />
To open file, first open STM32CubeIDE then import project by clicking file -> import -> choose directory -> include all file -> finish.  <br />

Requirement of project
---------------
<pre>
   - Must have STM32CubeIDE and version of project is 1.14.0, if your version below this project, please update. 
   - Must have TouchGFX Designer version 4.22.1 and STM32 Programmer must be installed at its directory.
   - Must have a board STM32F429ZIT6 like figure below.
</pre>
<p align="center">
  <img width="15%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/c5de6e94-fd2f-4521-a726-bd85c3714f20" /> <br />
   Board STM32F429ZIT6 <br />
</p>


Explain the project
---------------

When you upload finishing file, the LCD will display  a on/off button and it will be fading after 16.66ms 
<p align="center">
  <img width="10%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/a3ea124c-d99b-45c6-a61e-d41aa6124570" /> <br />
   Button on/off <br />
</p>

If you click it, it will open the menu game. Menu game includes 3 buttons: `setting`, `exit`, `play`  <br />
<p align="center">
  <img width="23%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/8b27476a-31e1-4480-bd24-51ff59aefba6" /> <br />
   Screen menu game <br />
</p>

### Setting screen ###


When you open the setting, you can see 3 modes: float, fast, collide 
  1. `Float`: which means food can be float or not. If food is floating, when it collides with wall, it change direction immediately (Default `Float` is off).
  2. `Fast`: which means snake can go faster or not. Faster is double speed when it goes normally.(Default `Fast` is off).
  3. `Collide`: which means snake can collide with wall or not.  (Default `Collide` is on).
<br />

<p align="center">
  <img width="25%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/84b1a673-8725-44e4-b013-6d9170b9f8ae" /> <br />
   Screen setting game <br />
</p>
<br />

#### Explain setting error ####

If you turn off collide, you can't turn on float because float is enable food floating and when collides with wall, change direction, and off collide means wall doesn't exists. We think its kind of non-physical so we block user from doing that, vice versa.  <br />
If user still wants to click, there is screen of `ERROR` for announcing user that action is being blocked, and after that it set state of button like before. <br />

### Playing screen ###

This game includes 4 control direction buttons: `up`, `down`, `right`, `left`.  <br />

You can't change direction immediately, if you want to up and snake is down, you need to turn right or left first then turn up for snake. <br />
If you can't play this game because of the speed of snake, you go to file header and change value of `DELAY`. Increasing `DELAY` is faster and slower by decreasing <br />
<p align="center">
  <img width="25%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/16e6a28c-31bb-4b25-9325-603d1d813cfe" /> <br />
   Screen while playing game <br />
</p>

#### Explain how snake run ####
First, you need to initialize a body of snake with lenght is 3, then spawn at the middle map but snake must fit with pixel of map so that it can't make our game have bug. <br />
After that, to make snake move, you need take the last element of snake put it at the position of first body then plus for a mount of speed to move.  <br />
Head is plus 16 (16 is the width of pixel) with first body, tail is the last element.

#### Explain Float mode ####

We design food will go randomly 2 directions: <br />
 1. `Straight line`: So with straight line, food will go up or down with axis Y, and go right or left with axis X.
 <p align="center">
  <img width="40%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/cf73a6d6-c692-4b99-967c-0310fb26cf14" /> <br />
  Direction of moving with straight line <br />
</p>

 2. `Diagonal line`: So with diagonal line, food will go with 2 direction of clock (CCW,CW), default is CCW.  <br />
 <p align="center">
  <img width="25%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/9ca139b7-0510-427e-817c-14dac2806908" /> <br />
   Direction of moving with diagonal line <br />
</p>

#### Explain Collide mode ####

In this mode, if collide mode is on, if snake is collide with wall so game is over, right?. So if collide mode is off, snake will go through wall and spawn opposite. You can easily understand that if our box is (left box is at 0, right box is at 240, in this situation that snake componets have the same coordinate for Y axis) , so if snake collide wall at 0, it will check if first of body snake is at 0 or not, yes, it will respawn body at 240. 

# Where you can find code
If you can't find where code is, so you find this directory Code:
 1. Header where we modify class or define is in file `header.txt` <br />
 2. File Screen1View.cpp which is main file we code is in `Screen1View.txt` <br />
 3. File generate screen or widget is in `generated.txt`  <br />

Thanks to my friend who helps me at this project, QB ♥

 
