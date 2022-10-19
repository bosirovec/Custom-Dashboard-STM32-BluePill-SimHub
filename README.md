# Welcome to Custom Dashboard project documentation

In this text, I'll try to explain how I made my very own dashboard that works with various games.
In a nutshell, all of the components are connected to an STM32 Blue Pill microcontroller which communicates with SimHub software.

All of the code is written in ArduinoIDE (I installed it separately, but you can also use the portable version that comes with SimHub.) In the first chapter, 
I will explain how to prepare ArduinoIDE to program BluePill via ST-LINK V2 programmer. Then, I will explain how to connect programmed Blue Pill to with SimHub as 
a Custom Serial Device. And lastly, I will go through the code and explain how it works.
Feel free to contact me if you stumble upon a problem.

 
If you were to use Arduino - please skip to last part where I cover the code because you won't need the first two chapters. 

#### Components:

1 x STM32 Blue Pill microcontroller

1 x ST-LINK V2 programmer (clone)

1 x Protoboard

1 x LCD screen with I2C adapter

3 x TowerPro SG90 servo motor

2 x LEDs

USB Power adapter

Wires


## 1. Setting Up
#### 1.1. Setting up Blue Pill
Since [@blekenbleu](https://github.com/blekenbleu) covered this part very good, for this step, please visit his [documentation](https://blekenbleu.github.io/Arduino/) and complete the steps up to "STM32Duino support"

#### 1.2. Setting up ArduinoIDE
Open ArduinoIDE and go to following: **File>Preferences** and in **Additional Board Manager URLs** add : 
<https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json>

Next, go to **Tools>Board>Board Manager**, search for STM and install the board package.
![image](https://user-images.githubusercontent.com/49985928/196671758-add1b725-af16-4b7e-b324-cd4fd8b897aa.png)

#### 1.3. I2C Library
Since I2C LCD is used, it's neccesarry to import I2C library for Blue Pill. The standard Arduino one will not work. 
You can find the download link under [**Programming STM32 for I2C LCD Display**](https://www.electronicshub.org/interfacing-i2c-lcd-with-stm32f103c8t6/)  section

#### 1.4. Upload Settings


## 2. Connecting BluePill to SimHub as Custom Serial Device

## 3. Code
