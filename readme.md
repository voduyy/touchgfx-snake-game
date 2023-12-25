# STM32F429I_DISCO_REV_D01 TBS
This TBS is configured for 320 x 240 pixels 16bpp screen resolution.  <br />

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ  - Pin PE2, RENDER_TIME - Pin PE3, FRAME_RATE  - Pin PE4, MCU_ACTIVE  - Pin PE5
<br />
# DETAIL PROJECT
To open file, first open STM32CubeIDE then import project by clicking file -> import -> choose directory -> include all file -> finish.  <br />
This game is include 4 control direction buttons: up, down, right, left. <br />
<p align="center">
  <img width="25%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/16e6a28c-31bb-4b25-9325-603d1d813cfe" /> <br />
   Screen while playing game. <br />
</p>
We also design a menu game is include 3 buttons: setting, exit, play  <br />

<p align="center">
  <img width="23%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/8b27476a-31e1-4480-bd24-51ff59aefba6" /> <br />
   Screen menu game. <br />
</p>
When you open the setting, you can see 3 modes: float, fast, collide 
  1. `Float`: which means food can be float or not. If food is floating, when it collides with wall, it change direction immediately (Default `Float` is off).
  2. `Fast`: which means snake can go faster or not. Faster is double speed when it goes normally.(Default `Fast` is off).
  3. `Collide`: which means snake can collide with wall or not.  (Default `Collide` is on).
<br />
<p align="center">
  <img width="25%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/84b1a673-8725-44e4-b013-6d9170b9f8ae" /> <br />
   Screen setting game. <br />
</p>
<br />
If you turn off collide, you can't turn on float because float is enable food floating and when collides with wall, change direction, and off collide means wall doesn't exists. We think its kind of non-physical so we block user from doing that, vice versa. <br />

# EXPLAIN FLOAT MODE
We design food will go randomly 2 directions: <br />
 1. `Straight line`: So with straight line, food will go up or down with axis Y, and go right or left with axis X.
 <p align="center">
  <img width="40%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/cf73a6d6-c692-4b99-967c-0310fb26cf14" /> <br />
   Simulation moving direction with straight line. <br />
</p>

 2. `Diagonal line`: So with diagonal line, food will go with 2 direction of clock (CCW,CW), default is CCW.  <br />
 <p align="center">
  <img width="25%" src="https://github.com/dythuiiiii/touchgfx_snake_game/assets/134894679/9ca139b7-0510-427e-817c-14dac2806908" /> <br />
   Simulation moving direction with diagonal line. <br />
</p>
<br />

# WHERE YOU CAN FIND CODE
If you can file where code is, so you find this directory Code: <br />
 1. Header is in file header.txt <br />
 2. File Screen1View.cpp is in Screen1View.txt <br />
 3. File generate screen is in generate.txt  <br />


 
