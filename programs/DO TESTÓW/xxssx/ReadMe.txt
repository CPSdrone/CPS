  MAX7456.h - MAX7456 On Screen Display Library for Arduino 
  Copyright (c) 2013 F. Robert Honeyman.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Version 1.0 - 2013.01.31 - by F. Robert Honeyman <www.coldcoredesign.com>

    History :
      0.1 ~ Creation of MAX7456Demo from Max7456DisplayFont 1.0
            2012.11.01 - F. Robert Honeyman <www.coldcoredesign.com>
      0.2 ~ Rewrote begin() to be easier, added many new functions.
            2012.12.04 - F. Robert Honeyman 
      0.3 ~ Major bug fix for VM0 register; finished adding functions;
            improved write() performance through Auto-Increment support.
            2012.12.20 - F. Robert Honeyman 
      0.4 ~ Added cursor() and noCursor(); fixed readChar(), reset(), 
            resetIsBusy(), and begin().
            2013.01.28 - F. Robert Honeyman 
      1.0 ~ Major bug fix in createChar(); structured for distribution;
            added examples; other minor bug fixes.
            2013.01.31 - F. Robert Honeyman 



  To use this library, copy the MAX7456 directory into: 

    (path)\Arduino\libraries\


  To use the UploadFont example, a terminal client with XModem is required.
  Tera Term can be downloaded for free from:

    http://ttssh2.sourceforge.jp/index.html.en


  To create custom mcm font files, these resources are helpful:

    http://www.mylifesucks.de/oss/c-osd/
    http://www.mylifesucks.de/tools/max7456/


