#include <Ps3Controller.h>

#define pinM1A 23
#define pinM1B 22
#define pinM2A 19
#define pinM2B 18
#define EN 21


#define M1A 1
#define M1B 2
#define M2A 3
#define M2B 4

#define M1 1 
#define M2 2
 
#define FREQUENCY 20000
#define RESOLUTION  10


#define lambda 5
#define d2r(x) x*(PI/180)

#define LengthAlpha 0.1 //diameter roda

int lx , ly , teta , rx , ry , atanVal , m1 , m2;



void setupMotor() {
  pinMode(EN , OUTPUT);
  ledcAttachPin(pinM1A, M1A);
  ledcAttachPin(pinM1B, M1B);
  ledcAttachPin(pinM2A, M2A);
  ledcAttachPin(pinM2B, M2B);
  ledcSetup(M1A, FREQUENCY, RESOLUTION);
  ledcSetup(M1B, FREQUENCY, RESOLUTION);
  ledcSetup(M2A, FREQUENCY, RESOLUTION);
  ledcSetup(M2B, FREQUENCY, RESOLUTION);
  ledcWrite(M1A, 0);
  ledcWrite(M1B, 0);
  ledcWrite(M2A, 0);
  ledcWrite(M2B, 0);

}



void notify()
{
  if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
    lx = Ps3.data.analog.stick.lx;
    ly = Ps3.data.analog.stick.ly;
  }

  if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
       rx = Ps3.data.analog.stick.rx;
       ry =  Ps3.data.analog.stick.ry;
        atanVal = atan2(rx,ry);
        atanVal = (atanVal * 180 / PI);
        teta = ((atanVal*-1) * 400);
    }

    kinematik(ly , -lx , teta);

    set_pwm(M1 , m1);
    set_pwm(M2 , m2);
    Serial.printf(" x : %3d | y : %3d | z : %3d | m1 : %3d | m2 : %3d \n" ,ly , -lx , teta, m1 , m2);
   
}

void onConnect(){
}

void setup()
{
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:1A:7D:DA:71:13");
    setupMotor();
}

void loop()
{
    if(!Ps3.isConnected())
        return;

}


void kinematik(int x , int y , int th){
  m1, m2  ;
  m1 = lambda * (cos(d2r(0))*x + sin(d2r(0))*y + LengthAlpha*d2r(th));
  m2 = lambda * (cos(d2r(180))*x + sin(d2r(180))*y + LengthAlpha*d2r(th));
 
  
  

  if(m1 > 1022) m1 = 1022;
  else if(m1 < -1022) m1 = -1022;
  
  if(m2 > 1022) m2 = 1022;
  else if(m2 < -1022) m2 = -1022;
 
  

}

void set_pwm(byte MTR , int val_pwm){
  switch(MTR){
    case M1 : 
      if(val_pwm > 0){
        digitalWrite(EN , HIGH);
        ledcWrite(M1A, val_pwm);
        ledcWrite(M1B, 0);
      }else if(val_pwm < 0){
        digitalWrite(EN , HIGH);
        ledcWrite(M1B, abs(val_pwm));
        ledcWrite(M1A, 0);
      }else{
        digitalWrite(EN , LOW);
        ledcWrite(M1A, 0);
        ledcWrite(M1B, 0);
      }
      break;

    case M2 : 
      if(val_pwm > 0){
        digitalWrite(EN , HIGH);
        ledcWrite(M2A, val_pwm);
        ledcWrite(M2B, 0);
      }else if(val_pwm < 0){
        digitalWrite(EN , HIGH);
        ledcWrite(M2B, abs(val_pwm));
        ledcWrite(M2A, 0);
      }else{
        digitalWrite(EN , LOW);
        ledcWrite(M2A, 0);
        ledcWrite(M2B, 0);
      }
      break;
  }}