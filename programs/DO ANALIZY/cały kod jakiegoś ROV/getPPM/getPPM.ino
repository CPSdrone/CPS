unsigned long int falling,prevFall,pulseGap;
int i=1;
volatile int ch[11];


void readPPM()  { //ISR triggered by falling signal on int pin
  falling=micros(); //store time value a when pin value falling
  pulseGap=falling-prevFall;      //calculating time between two edges
  prevFall=falling;        // 
  ch[i]=pulseGap;     //store gap values in shared array
      //see if it's the boundary marker
  if(pulseGap>3000) {
    i=1; //reset for next interrupt
  } else {
    i++;       
  }
}

void getPPM(){  //assign 8 channel values
  for(int k=1;k<11;k++){
//    Serial.print(" ");Serial.print(k);
//    Serial.print("=");Serial.print(ch[k]);
#ifdef PPMviaRC  // PPM coming in via rc receiver
    if (k==1) {
      medLR.in(int(ch[1]));  //give new value to median calc
      rcLR = medLR.out();  //get median to elim noise
    } else if (k==2) {
      medFB.in(int(ch[2]));  //give new value to median calc
      rcFB = medFB.out();  //get median to elim noise
    } else if (k==3) { 
      medUD.in(int(ch[3]));  //give new value to median calc
      rcUD = medUD.out();  //get median to elim noise
    } else if (k==4) {
      medTW.in(int(ch[4]));  //give new value to median calc
      rcTW = medTW.out();  //get median to elim noise
    } else if (k==5) rcLI = ch[5];  //switches are bound to leap
    else if (k==6) rcx6 = ch[6];
    else if (k==7) rcCA = ch[7];
  }
#else   // PPM coming in via hard wire to xmitter buddy box port
    if (k==1)      rcUD = ch[1]; //only get the ones we're using
    else if (k==2) rcLR = ch[2];
    else if (k==3) rcFB = ch[3];
    else if (k==4) rcTW = ch[4];
    else if (k==5) rcLI = ch[5];
    else if (k==6) rcx6 = ch[6];
    else if (k==7) rcCA = ch[7];
    else if (k==8) rcx8 = ch[8];
  }
#endif  
//  Serial.println("");                       
}     

