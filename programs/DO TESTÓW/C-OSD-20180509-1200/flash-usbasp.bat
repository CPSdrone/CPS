@echo off
:: /****************************************************************************
::  *   Copyright (C) 2014-2018 by Claas Anders "CaScAdE" Rathje               *
::  *   admiralcascade@gmail.com                                               *
::  *   Project-URL: http://www.mylifesucks.de/oss/c-osd/                      *
::  *                                                                          *
::  *   This program is free software; you can redistribute it and/or modify   *
::  *   it under the terms of the GNU General Public License as published by   *
::  *   the Free Software Foundation; either version 2 of the License.         *
::  *                                                                          *
::  *   This program is distributed in the hope that it will be useful,        *
::  *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
::  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
::  *   GNU General Public License for more details.                           *
::  *                                                                          *
::  *   You should have received a copy of the GNU General Public License      *
::  *   along with this program; if not, write to the                          *
::  *   Free Software Foundation, Inc.,                                        *
::  *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
::  ****************************************************************************/

:: set FIRMWAREFILE to current version
set FIRMWAREFILE=C-OSD-20180509-1200-pal.hex

:: Read fuses as test
avrdude -c usbasp -p m162 -n -v

IF ERRORLEVEL 1 GOTO COMERR

:: uncomment next command if it is the first time you flash the OSD and want to set the fuses
:: avrdude -c usbasp -p m162 -u -v -U lfuse:w:0xff:m -U hfuse:w:0xd7:m -U efuse:w:0xfb:m

:: uncomment out next command if you want to flash the characters file in PAL
:: avrdude -c usbasp -p m162 -u -U flash:w:C-OSD-characters-pal.hex

:: uncomment out next command if you want to flash the characters file in NTSC
:: avrdude -c usbasp -p m162 -u -U flash:w:C-OSD-characters-ntsc.hex



:: programm firmware
avrdude -c usbasp -p m162 -u -U flash:w:%FIRMWAREFILE%


IF ERRORLEVEL 1 GOTO FLASHERR

echo Firmware flashed successfully! Yay!
echo Got outside and fly - enjoy the view!

GOTO END

:COMERR
echo ERROR: Could not read fuses to check connection
echo * Check connection of USBASP
echo * Check driver installation for USBASP
echo * Check connection of USBASP to OSD
echo * Post in Forum
GOTO END


:FLASHERR
echo ERROR: Could not flash firmware %FIRMWAREFILE%
echo * Check if file exists
echo * Check if firmware is too big for AVR
echo * Post in Forum
GOTO END

:END


echo.
echo.

::EOF