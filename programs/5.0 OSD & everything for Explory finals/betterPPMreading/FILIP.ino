#define RECEIVE_PIN 2
#define CHANNEL_AMOUNT 6
#define DETECTION_SPACE 2
#define METHOD RISING

int ch[CHANNEL_AMOUNT + 1];

void setup()
{
Serial.begin(115200);
pinMode(RECEIVE_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);
}

void loop()
{
ppm_write();
}

void ppm_write()
{
static unsigned long int t;
if (millis() - t < 100)
return 0;
for (byte i = 0; i < CHANNEL_AMOUNT + 1; i++)
{
Serial.print(ch[i]);
Serial.print("\t");
}
Serial.print("\n");
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
}
