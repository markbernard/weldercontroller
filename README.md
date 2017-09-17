# Spot Welder Controller

# DISCLAIMER: Use at your own risk. Working with electricity can be dangerous. I assume no responsibily for your use of anything found in this repository. You have been warned.

This is a replacement for the control circuit found in Kerry Wong's spot welder video. You can find the video on [You Tube](https://www.youtube.com/watch?v=_oRsMGGNdf0). He also has a write up on his [web site](http://www.kerrywong.com/2017/06/18/dual-purpose-spot-welder-with-pulse-duration-control/). This only replaces the 555 timer control circuit. The high voltage side remains the same.

The code for this controller makes it easy to tell which state the controller is in. When you first power it on the green LED will light up. This means it is ready to weld. When you press the button to weld the red LED will light up indicating that the welder is currently pushing current. Once the weld completes the welder will turn off and both LEDs will be off. After a delay of 2 seconds the welder will be ready again.

This circuit requires minimal parts to go along with the controller. The code is written for a Node MCU v1.0 board but should port easily to any Arduino or ESP board.

As the code is written the green LED connects from ground to GPIO pin 14. The red LED connects to GPIO pin 4. The switch connects to to GPIO pin 5. These pins are defined at the top of the code and can be changed to your liking.

You also need to connect GPIO 4 to pin 1 on the optocoupler ([MOC3061](http://www.jameco.com/Jameco/Products/ProdDS/133874.pdf)) and ground to pin 2 of the optocoupler.

Make sure the switch you use is a momentary switch and to use a pull down resistor between the switch and the GPIO pin.

**Welder is ready**
![Alt text](/WelderReady.jpg?raw=true "Welder is ready for operation.")
**Welder is operating**
![Alt text](/WelderOn.jpg?raw=true "Welder is operaterating.")
**Welder is off**
![Alt text](/WelderOff.jpg?raw=true "Welder is off.")
