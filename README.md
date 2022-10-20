# Welcome to Custom Dashboard project documentation


In this text, I'll try to explain how I made my very own dashboard that works with various games.
In a nutshell, all of the components are connected to an STM32 Blue Pill microcontroller which communicates with SimHub software.
Base of the dashboard is a Fiat Bravo dashboard.
[Video](https://www.youtube.com/watch?v=OHZ0x-ckDA0&ab_channel=BornaSirovec)

All of the code is written in ArduinoIDE (I installed it separately, but you can also use the portable version that comes with SimHub.) In the first chapter, 
I will explain how to prepare ArduinoIDE to program BluePill via ST-LINK V2 programmer. Then, I will explain how to connect programmed Blue Pill to with SimHub as 
a Custom Serial Device. And lastly, I will go through the code and explain how it works.
Feel free to contact me if you stumble upon a problem.

 
If you were to use Arduino - please skip to last part where I cover the code because you won't need the first two chapters. 

#### Components:
<ul>
<li>1 x STM32 Blue Pill microcontroller
<li>1 x ST-LINK V2 programmer (clone)
<li>1 x Protoboard
<li>1 x LCD screen with I2C adapter
<li>3 x TowerPro SG90 servo motor
<li>2 x LEDs
<li>USB Power adapter
<li>Wires
</ul>

## 1. Setting Up BluePill & Arduino IDE
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
<ul>
 <li> Board - Generic STM32F1 series
 <li> Board part number - BluePill F103C8
</ul>


## 2. Connecting BluePill to SimHub as Custom Serial Device
#### 2.1. Settings
In case you don't have Custom Serial Device plugin enabled in SimHub, please enable it.
Here are the settings I used:
![41ba364a-6da8-47e6-96af-d52b7749189a](https://user-images.githubusercontent.com/49985928/196920132-ff8d844b-945f-45d5-9611-524ccc804e6a.jpg)
NOTE: You dont have to write Connect/Dissconnect messages.
Also, I enabled incoming data just for the purpose of debugging - I read the data SimHub sent to my BluePill, and then sent them back - just so I know if the data is acutally sent/read correctly.

#### 2.2. Custom Formula/Protocol
In custom formula, you choose the telemetry properties you want to extract from your game. Here is what my custom formula looks like:
![image](https://user-images.githubusercontent.com/49985928/196920998-f9a5c6d6-b097-4b2b-bb42-a7ef6da90d6d.png)


## 3. Code
#### 3.1. Calibration and mapping
Since servos weren't able to actually rotate 180˙, I had to calibrate them. For that purpose I wrote a script which helped me map Dashboard values(speed, rpms, fuel percentage) with to rotating degrees.
![kalibracija](https://user-images.githubusercontent.com/49985928/196921477-5f619077-1471-46f4-a335-a86a0dd9eff8.png)
![image](https://user-images.githubusercontent.com/49985928/196921584-99497472-143c-42b3-8ff3-64414562d110.png)

All those mapped values are stored in ``degArray``, ``speedArray``, ``rpmArray``, ``fuelArray`` and ``fuelDegArray``.


#### 3.2. Logic
**The rest of the code is documented and explained in the comments **



