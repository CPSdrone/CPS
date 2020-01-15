void getTransVectors() { //derive desired angle, speed, from RC
    Vd = 0; theta = 0; Vtwist = 0;
        // first check if stick is arming/disarming
    if (rcUD < lowUD + rcSlack) {  //depth stick very far down
      if ((rcTW > cwTW - rcSlack) ||(rcTW < ccwTW + rcSlack)) {  // twist stick is arm or disarm
        Serial.println("arming or disarming");
        return;  //leave at 0s
      }
    }
        //  now see if any translation wanted
    if (abs(rcFB-neutFB) > rcSlack || abs(rcLR-neutLR) > rcSlack) {  
          //derive translation speed and angle from rc ctls
        int wkFB = map(rcFB, lowFB, highFB, -100, 100);    // map rc to calc factor
        int wkLR = map(rcLR, leftLR, rightLR, -100, 100);
        Vd = (sqrt(wkFB * wkFB + wkLR * wkLR))/100.00; // trans speed -1 to 1
        theta = atan2(wkLR, wkFB);  // trans angle 0 to 2PI

/*          // make sure we stay pointed the same direction during move
        int diffHdg = getHeading()-setHeading;  // how far off are we (deg)
        diffHdg = normalizeDiff(diffHdg);
        if (abs(diffHdg) > 7) {  //drifted too far CW or CCW, need twist
          int wkTW = map(diffHdg, -45, 45, 65, -65); //proportional resp
          Vtwist = wkTW/100.00; // twist speed -1 to 1
          Serial.print("Off heading by ");Serial.print(diffHdg);
        }  */
    }
}

void runVectThrusters() {  // calculate four thrusters speeds using vectors
      if (!armAuto) return;
      int motorSpeedWk;
      float vMotor[] = {0,0,0,0};
      vMotor[0] = Vd * sin(theta + PI/4) + Vtwist; // thruster layout (diff from drone)
      vMotor[1] = Vd * cos(theta + PI/4) - Vtwist; //    1    2
      vMotor[2] = Vd * cos(theta + PI/4) + Vtwist; //
      vMotor[3] = Vd * sin(theta + PI/4) - Vtwist; //    3    4
        
        //run the motors 
      for (int x=0; x<4; x++) {
        if (vMotor[x] < -1) vMotor[x] = -100; //normalize
        else if (vMotor[x] > 1) vMotor[x] = 100;
        else vMotor[x] = vMotor[x] * 100;
        motorSpeedWk = map(abs(vMotor[x]), 0, 100, 0, 255);
        if (vMotor[x] < 0) {  //set motor direction
          analogWrite(motorFwdPwmPin[x], motorSpeedWk); //make it go
        } else {
          analogWrite(motorRevPwmPin[x], motorSpeedWk); //make it go
        }    
      }  
}

void runUpDnThrusters(int ctlUD) {
  int motorSpeedWk;
        if (!armAuto) return;
        if (ctlUD < neutUD - rcSlack) {  //set motor dir for Down
            // speed proportional to stick dist from neutral
          motorSpeedWk = map(ctlUD, neutUD, lowUD, 0, 255);
          if (motorSpeedWk>255) motorSpeedWk=255;  //over is bad
          analogWrite(motorFwdPwmPin[4], motorSpeedWk); //make it go or stop
        } else if (ctlUD > neutUD + rcSlack) { //set motor dir for Up
          motorSpeedWk = map(ctlUD, neutUD, highUD, 0, 255);
          if (motorSpeedWk>255) motorSpeedWk=255;  //over is bad
          analogWrite(motorRevPwmPin[4], motorSpeedWk); //make it go or stop
       } else { 
          analogWrite(motorFwdPwmPin[4], 0); //make it go or stop
          analogWrite(motorRevPwmPin[4], 0); //make it go or stop
       }
  
}

void stopVectMotors() {
      for (int x=0; x<numMotors-1; x++) {
        analogWrite(motorFwdPwmPin[x], 0); //make it stop
        analogWrite(motorRevPwmPin[x], 0); //make it stop
      }
}
void stopAllMotors() {
      for (int x=0; x<numMotors; x++) {
        analogWrite(motorFwdPwmPin[x], 0); //make it stop 
        analogWrite(motorRevPwmPin[x], 0); //make it stop
      }
}

void testMotors() {
  int motorSpeedWk = 250;
  if (rcFB-neutFB > rcSlack) {
    analogWrite(motorFwdPwmPin[0], motorSpeedWk); //make it go or stop
    analogWrite(motorRevPwmPin[0], 0); //make it go or stop
    Serial.println(" 1 fwd");
  } else if (neutFB - rcFB > rcSlack)  {
    analogWrite(motorRevPwmPin[0], motorSpeedWk); //make it go or stop
    analogWrite(motorFwdPwmPin[0], 0); //make it go or stop
    Serial.println(" 1 rev");
  } else {
    analogWrite(motorRevPwmPin[0], 0); //make it go or stop
    analogWrite(motorFwdPwmPin[0], 0); //make it go or stop
  }
  if (rcLR-neutLR > rcSlack) {
    analogWrite(motorFwdPwmPin[1], motorSpeedWk); //make it go or stop
    analogWrite(motorRevPwmPin[1], 0); //make it go or stop
    Serial.println(" 2 fwd");
  } else if (neutLR - rcLR > rcSlack)  {
    analogWrite(motorRevPwmPin[1], motorSpeedWk); //make it go or stop
    analogWrite(motorFwdPwmPin[1], 0); //make it go or stop
    Serial.println(" 2 rev");
  } else {
    analogWrite(motorRevPwmPin[1], 0); //make it go or stop
    analogWrite(motorFwdPwmPin[1], 0); //make it go or stop
  }
  if (rcTW-neutTW > rcSlack) {
    analogWrite(motorFwdPwmPin[2], motorSpeedWk); //make it go or stop
    analogWrite(motorRevPwmPin[2], 0); //make it go or stop
    Serial.println(" 3 fwd");
  } else if (neutTW - rcTW > rcSlack)  {
    analogWrite(motorRevPwmPin[2], motorSpeedWk); //make it go or stop
    analogWrite(motorFwdPwmPin[2], 0); //make it go or stop
    Serial.println(" 3 rev");
  } else {
    analogWrite(motorRevPwmPin[2], 0); //make it go or stop
    analogWrite(motorFwdPwmPin[2], 0); //make it go or stop
  }  
  if (rcUD-neutUD > rcSlack) {
    analogWrite(motorFwdPwmPin[3], motorSpeedWk); //make it go or stop
    analogWrite(motorRevPwmPin[3], 0); //make it go or stop
    Serial.println(" 4 fwd");
  } else if (neutUD - rcUD > rcSlack)  {
    analogWrite(motorRevPwmPin[3], motorSpeedWk); //make it go or stop
    analogWrite(motorFwdPwmPin[3], 0); //make it go or stop
    Serial.println(" 4 rev");
  } else {
    analogWrite(motorRevPwmPin[3], 0); //make it go or stop
    analogWrite(motorFwdPwmPin[3], 0); //make it go or stop
  } 
  if (offx6 - rcx6 > rcSlack) {
    analogWrite(motorRevPwmPin[4], motorSpeedWk); //make it go or stop
    analogWrite(motorFwdPwmPin[4], 0); //make it go or stop
    Serial.println(" 5 fwd");
  } else {
    analogWrite(motorRevPwmPin[4], 0); //make it go or stop
    analogWrite(motorFwdPwmPin[4], 0); //make it go or stop
  } 
}
