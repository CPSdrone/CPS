void ppm_write()
{
static unsigned long int t;
if (millis() - t < 100)
return 0;
//ppmprint();
  t = millis();
}

void ppm_interrupt()
{
static byte i;
static unsigned long int t_old;
unsigned long int t = micros(); //store time value a when pin value falling/rising
unsigned long int dt = t - t_old; //calculating time inbetween two peaks
t_old = t;

if ((dt > DETECTION_SPACE) || (i > CHANNEL_AMOUNT))
{
i = 0;
}
ch[i++] = dt;

  /*Serial.println(t);
  if(100-t<100) {DISCONNECT= 0;
    for (byte i = 0; i < CHANNEL_AMOUNT + 1; i++){
      ch[i]=1500;
    }
  }else{ DISCONNECT= 1;}*/
}

void ppmprint(){
  for (byte i = 0; i < CHANNEL_AMOUNT + 1; i++){
  Serial.print(ch[i]);
  Serial.print("\t");
  }
  Serial.print(DISCONNECT);
  Serial.print("\n");
}
