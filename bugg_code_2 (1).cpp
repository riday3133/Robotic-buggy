#include <NewPing.h>
#define TRIGGER_PIN 13
#define ECHO_PIN 12
#define MAX_DISTANCE 200
NewPing Sonar(TRIGGER_PIN,ECHO_PIN,MAX_DISTANCE);
static long start_time_g=0,curr_time_g=0;
unsigned long elapsed_time_g=0;
int irpin=4;
int flag=0;
static int rounds=0;
unsigned long d=0;
int l,r;
unsigned int dist=0;

void setup() {
	pinMode(5,OUTPUT);
	pinMode(6,OUTPUT);
	pinMode(7,OUTPUT);
	pinMode(8,OUTPUT);

	pinMode(A0,INPUT);//left input pin
	pinMode(A1,INPUT);//right input pin

	Serial.begin(9600);
	/// Some additional zigbee code
	/// comment out if things dont work well
	Serial.print("+++");  // Enter xbee AT command mode, NB no carriage return here
	delay(1500);          // Guard time
	Serial.println("ATID 3333, CH C, CN");
}
void forward()
{
	digitalWrite(5,HIGH);
	digitalWrite(6,LOW);
	digitalWrite(7,LOW);
	digitalWrite(8,HIGH);
}

void backward()
{
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(8,LOW);
  digitalWrite(7,HIGH);
}
void left()
{
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
}

void clockwise()
{
	digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
}

void anti_clockwise()
{
	digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
}

void right()
{
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
}
void stopp()
{
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(8,LOW);
  digitalWrite(7,LOW);
}

void normal()
{
   if(l==0 && r==0)
      {
           forward();
      }
      else if(l==0 && r==1)
      {
          left();
		  //anti_clockwise();
      }
      else if(l==1 && r==0)
      {
          right();
		  //clockwise();
       }  
      else if(l==1 && r==1)
      {
           forward();
      }
}

void leftblind()
{
   if(l==0 && r==0)
      {
           forward();
      }
      else if(l==1 && r==0)
      {
          right();
		  //clockwise();
      }  
      else if(l==1 && r==1)
      {
           forward();
      }
}    
void gantry()
{
   dist=Sonar.ping_cm();
    l=digitalRead(A0);
    r=digitalRead(A1);
    if((dist<15) && (dist>0))
    {
      stopp();
    }
    else{
      if((l==0) && (r==0))
      {
           forward();

      }
      else if((l==0) && (r==1))
      {
          left();
		  //anti_clockwise();
      }
      else if((l==1) && (r==0))
      {
          right();
		  //clockwise();
       }  
      else if((l==1) && (r==1))
      {
           forward();
      }
      if(digitalRead(irpin)==HIGH)
      {
        d=pulseIn(irpin,HIGH);
        if(d>500 && d<1500)
        {
          stopp();
          Serial.println("GANTRY NUM: 1");
          delay(1000);
        }
        else if(d>1500 && d<2500)
        {
          stopp();
          Serial.println("GANTRY NUM: 2");
          delay(1000);
        }
        else if(d>2500 && d<3500)
        {
          stopp();
          Serial.println("GANTRY NUM: 3");
          rounds=rounds+1;
          Serial.print("THE GANTRY COUNTER IS: ");
          Serial.println(rounds);
          delay(1000);
        }

        else
        {
          Serial.println(d);
          Serial.println("GANTRY UNKNOWN");
        }

        if(rounds>=2) // was >= 3 earlier
        {
          flag=3;
          start_time_g=millis();
        }
      }
    }
}
void loop() {

if(flag==0)
{
  if(Serial.read()=='L')
  {
    flag=1;
  }
}  
if(flag==1)
  {
    gantry();
  }
//PARKING
if(flag==3)
{
  curr_time_g=millis();
  elapsed_time_g=curr_time_g-start_time_g;

  if(elapsed_time_g<2500)
  {
    flag=3;
    leftblind();
  }
  if(elapsed_time_g>2500 && elapsed_time_g<6500)
  {
    flag=3;
    normal();
  }
  if(elapsed_time_g>6500)
  {
    stopp();
    Serial.print("BUGGY PARKED");
    delay(200);
    flag=-1;
  }
}
}