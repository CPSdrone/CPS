void maintainDepth() {
      int UDctl;  // this is set to pulse width value. chgd to motor value later
          
      if (abs(rcUD - neutUD) < rcSlack) { // no up down ctl, hold depth
        int diffDepth = waterDepth - setDepth;  // how far off in cm
        if (diffDepth > 5) {  //too deep, come up
          UDctl = map(diffDepth, 0, 30, neutUD, lowUD);
        } else if (diffDepth < -5) {  //too shallow, go down
          UDctl = map(abs(diffDepth), 0, 30, neutUD, highUD);
        } else { 
          UDctl = neutUD;  // within limits, don't run motors
        }
        runUpDnThrusters(UDctl);   // use computed power
      } else {  // user wants to go up or down
        runUpDnThrusters(rcUD);   // use power from RC
        setDepth = waterDepth;    // save this depth as desired depth
      }
}

void maintainHeading() {
      if (abs(rcTW-neutTW) > rcSlack)  {   // any twist wanted?
         // get speed for direction change
        int wkTW = map(rcTW, ccwTW, cwTW, -35, 35);  //5/9/18 less abrupt
        Vtwist = wkTW/100.00; // twist speed -1 to 1
        rcTwist = true; autoTwist = false;
        setTwistTime = millis() + 1000;  //updt heading later - after it stops moving
        
      } else if (!autoTwist && !rcTwist) {  // use compass to maintain current heading
        int diffHdg = getHeading()-setHeading;  // how far off are we (deg)
        diffHdg = normalizeDiff(diffHdg);
        if (abs(diffHdg) > 10) {  //drifted too far CW or CCW, need twist
          int wkTW = map(diffHdg, -45, 45, 65, -65); //proportional resp
          Vtwist = wkTW/100.00; // twist speed -1 to 1
          Serial.print("Off heading by ");Serial.print(diffHdg);
          Serial.print(" Vtwist=");Serial.println(Vtwist);
          autoTwist = true;
           // don't correct again until after it stops moving
          setTwistTime = millis() + 1000;  
        } else Vtwist = 0;  
      }
  
}

void checkArming() {
        // arm or disarm depth hold and heading hold
     if (rcUD < lowUD + rcSlack) {  //depth stick very far down
        if (rcTW > cwTW - rcSlack && !armAuto) {  // twist stick is in arm position - right
          setHeading = getHeading();  //set current heading
          setDepth = waterDepth;    // save this depth as desired depth
          armAuto = true; 
          Serial.println("arming");
          delay(2000);  // let them relax the stick
          stopAllMotors();
          Vd = 0; Vtwist = 0;
        } else if (rcTW < ccwTW + rcSlack && armAuto) {  // stick is in disarm position - left
          stopAllMotors();
          Vd = 0; Vtwist = 0;
          armAuto = false; 
          rcTwist = false; autoTwist = false;
          Serial.println("disarming");
          delay(300);  // let them relax the stick
        }  
      }
}




int getHeading() { //check compass to see current heading
  Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
  Wire.write(ANGLE_16);                     //Sends the register we wish to start reading from
  Wire.endTransmission();
 
  // Request 2 bytes from the CMPS12
  // this will give us both bytes of the 16 bit bearing
  Wire.requestFrom(CMPS12_ADDRESS, 2);       
  while(Wire.available() < 2);        // Wait for all bytes to come back
  high_byte = Wire.read();
  low_byte = Wire.read();
  angle16 = high_byte;                 // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;
  angle16 += 1800;                      //mounted backwards
  if (angle16 > 3600) angle16 -= 3600;  //normalize
  return angle16/10;  //compass returns 0-3599. ignore tenth
}

int normalizeDiff(int rawDegree) {  //make sure degrees diff are between -180 and 180
    if (rawDegree > 180) return rawDegree - 360;
    else if (rawDegree < -180) return rawDegree + 360;
    else return rawDegree;
}    

