/****************************************************************************
 *   Copyright (C) 2009-2018 by Claas Anders "CaScAdE" Rathje               *
 *   admiralcascade@gmail.com                                               *
 *   Project-URL: http://www.mylifesucks.de/oss/c-osd/                      *
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License.         *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program; if not, write to the                          *
 *   Free Software Foundation, Inc.,                                        *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
 ****************************************************************************/

Look at
http://www.mylifesucks.de/oss/c-osd/
to see what this is all about.

Simple way:
	- Flash C-OSD-characters-(pal|ntsc).hex
		- the 3rd LED (green one) should blink now
		- wait till all chars appear on screen and ALL 4 LEDs are on
	- Flash C-OSD-*-(pal|ntsc).hex acording to your needs

	If you already have flashed the character file and you just want to
	update the software itself, you do not need to flash the character files again.

Instructions:
	Connect the "To FC" port from EPi-OSD to the Debug port of the NaviCtrl.
	You will need at least NaviCtrl 2.16a soft to fully enjoy the software.

	S1 enters a menu where you can cycle through using S2 and toggle/accept choice with S1

	You can connect a spare RC-Channel to the PPM connector and enable/disable the HUD remotely with it, hopefully.

Known Problems:
	When using the EPi-OSD, NaviCtrl and another Pc-Connection (direct or via wireless connections like bluetooth, WI232, WiFi and such) on the same wire there might be data collisions causing laggy screen updates, and in some hypothetical cases that are not scientifically proven, harm to the Atmega and other attached Hardware. So it is not recommended to use the stuff like that, altough some people are and not experiencing any problems at all.
	Software may fry your cat when flashing the micro-oven with it, or even when using the software near a micro-oven.

Donations:
	http://www.mylifesucks.de/oss/c-osd/#PayPal

FAQ:
	http://www.mylifesucks.de/oss/c-osd/#FAQ

How to build the software from source:
	http://youtu.be/BgOAicjeNC8
	
How to flash the software (the MK-user way):
	Requirements:
		- Windows PC with a real SerialPort
		- PonyProg (http://www.LancOS.com)
		- SerCon
	HowTo:
		- Connect the ISP1 from SerCon to the ISP connector of the EPi-OSD board using a
		  normal 1:1 cable
		- Set the Jumper on the SerCon
		- SetUp PonyProg
			Setup -> Interfac Setup
			and select the correct Serial Port and "SI Prog API"
		- Select Chip manufacturer "AVR micro" and Chip "ATMega162"
		- If this is the first time using you board check the fuses (only BOD1LEVEL and EESAVE are checked)
		- Open the .hex file and Writa All (Ctrl+W)
		- Hopefully done :)

How to flash the software with avrdude:
        Requirements:
                - ISP-Programmer which is supported by avrdude (SerCon, usbasp, AVRISP-MKII and so on...)
                - avrdude
        HowTo:
                - assuming your ISP programmer is a usbasp compatible connected via usb
				- adjust supplied flash-usbasp.bat (according to your programmer and recent firmware)
				- OR use the following commands
					- fuses:
						avrdude -c usbasp -p m162 -u -v -U lfuse:w:0xff:m -U hfuse:w:0xd7:m -U efuse:w:0xfb:m
					- firmware:
						avrdude -c usbasp -P usb -p m162 -u -U flash:w:C-OSD-######.hex
					- for serial port programming using the SerCon
						avrdude -c siprog -P COM1 -p m162 *****************look above*****************


#EOF
