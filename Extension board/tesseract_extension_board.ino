#include <Stepper.h>

int thermistorPin1 = A0;
int thermistorPin2 = A1;
int thermistorPin3 = A2;

int relay1=6;
int relay2=4;
int relay3=7;
int relay4=8;

int input1=10;
int input2=11;
int input3=12;

float Vo1; 
float Vo2; 
float Vo3; 

int stepperpin1=3;
int stepperpin2=5;
int stepperpin3=9;
int stepperpin4=13;

int act=0;
String receivedCommand;
int recieved_status = 0;
int commands[8];
int speed;
int distance;
char data[20];

const int stepsPerRevolution = 2048;  
int command=0;


Stepper myStepper(stepsPerRevolution, 3, 9, 5, 13);

void setup() {
  pinMode(thermistorPin1,INPUT);
  pinMode(thermistorPin2,INPUT);
  pinMode(thermistorPin3,INPUT);
  pinMode(input1,INPUT_PULLUP);
  pinMode(input2,INPUT_PULLUP);
  pinMode(input3,INPUT_PULLUP);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
  pinMode(stepperpin1,OUTPUT);
  pinMode(stepperpin2,OUTPUT);
  pinMode(stepperpin3,OUTPUT);
  pinMode(stepperpin4,OUTPUT);
  Serial.begin(9600);
  delay(25);
  Serial.println("Ready");
}

void loop() {
   readSerialPort();
     
      if(recieved_status==1){
        if(act==1){
          delay(15);
          Serial.print("Moving at ");
          Serial.print(distance);
          Serial.print(" at a speed of ");
          Serial.print(speed);
          myStepper.setSpeed(speed);
          myStepper.step(distance);
          }
         if(act==2){
          delay(15);
          Serial.print("Moving at ");
          Serial.print(distance);
          Serial.print(" at a speed of ");
          Serial.print(speed);          myStepper.setSpeed(speed);
          myStepper.step(-distance);
          } 

         if(act==3){
          delay(15);
          Serial.print("Turning relay one on");
          if(speed==1){
            digitalWrite(relay1,HIGH);
            }else if(speed==0){
            digitalWrite(relay1,LOW);  
            }
          }
         
         if(act==4){
          delay(15);
          Serial.print("Turning relay two on");
          if(speed==1){
            digitalWrite(relay2,HIGH);
            }else if(speed==0){
            digitalWrite(relay2,LOW);  
            }
          }  

         if(act==5){
          delay(15);
          Serial.print("Turning relay three on");
          if(speed==1){
            digitalWrite(relay3,HIGH);
            }else if(speed==0){
            digitalWrite(relay3,LOW);  
            }
          } 

          
         if(act==6){
          delay(15);
          Serial.print("Turning relay four on");
          if(speed==1){
            digitalWrite(relay4,HIGH);
            }else if(speed==0){
            digitalWrite(relay4,LOW);  
            }
          }  
         if(act==7){
          delay(15);
          Serial.print("Turning all relays on");
          if(speed==1){
            digitalWrite(relay1,HIGH);
            digitalWrite(relay2,HIGH);
            digitalWrite(relay3,HIGH);
            digitalWrite(relay4,HIGH);

            }else if(speed==0){
            digitalWrite(relay1,LOW);  
            digitalWrite(relay2,LOW);  
            digitalWrite(relay3,LOW);  
            digitalWrite(relay4,LOW);  

            }
          }    
        
        delay(20); 
        Serial.println("Done");   
        stopMotor();
        act=-1;
        recieved_status = 0;
        Serial.flush();
        delay(20);
      }
/*
  if(digitalRead(input1)==LOW){
    digitalWrite(relay1,HIGH);
  }else{
  digitalWrite(relay1,LOW);
  }
  
  if(digitalRead(input2)==LOW){
    digitalWrite(relay2,HIGH);
   }else{
  digitalWrite(relay2,LOW);
   }
  if(digitalRead(input3)==LOW){
    digitalWrite(relay3,HIGH);
   }else{
  digitalWrite(relay3,LOW);
   }
*/

   if (millis() %1000 <500){
  
  
  Vo1 = analogRead(thermistorPin1);
  Vo2 = analogRead(thermistorPin2);
  Vo3 = analogRead(thermistorPin3);
  /*
  Serial.print("Temperature1 : ");
  Serial.print(thermistorCalculate(Vo1));
  Serial.print(" ");
  Serial.print("Temperature2 : ");
  Serial.print(thermistorCalculate(Vo2));
  Serial.print(" ");
  Serial.print("Temperature3 : ");
  Serial.print(thermistorCalculate(Vo3));
  Serial.println(" ");   
  */
}
}

float thermistorCalculate(float input){
  float R2, tKelvin, tCelsius;
  const float Beta     = 3974.0;
  const float roomTemp = 298.15;   // room temperature in Kelvin
  const float Ro = 10000.0; // Resistance of the thermistor at roomTemp
  const float R1 = 9710.0;  // Resistnce of the known resistor  
  R2 = R1 * (1023.0 / (float)input - 1.0); // Resistance of the Thermistor
  tKelvin = (Beta * roomTemp) /
            (Beta + (roomTemp * log(R2 / Ro)));
  tCelsius = tKelvin - 273.15;
  return tCelsius;
  }


void readSerialPort(){
    int i=0;
    int counter=0;
    int k=0;
    int command=0;
 
    
    if (Serial.available()) {
        for(int j=0; j<3; j++){
            commands[j]=-1;
        }
       
        delay(10);
        receivedCommand = "";
        while (Serial.available() > 0) {
            data[i] = (char)Serial.read();
            // mporei na einai megalo to mnm mexri to \n
            i++;   
            if (i>=20){
                break;
            } 
            delay(1);   
        }   
        recieved_status = 1;
        Serial.flush();
        delay(20);
        for(i=0; i<20; i++){
            if(data[i]=='/' || data[i]==','){
                switch(counter){
                    case 1:
                        command=data[i-counter]-'0';
                        commands[k]=command;
                        break;
                    case 2:
                        command=(data[i-counter]-'0')*10 + (data[i-counter+1]-'0');
                        commands[k]=command;
                        break;
                    case 3:
                        command=(data[i-counter]-'0')*100 + (data[i-counter+1]-'0')*10 + (data[i-counter+2]-'0');
                        commands[k]=command;
                        break;
                    case 4:
                        command=(data[i-counter]-'0')*1000 + (data[i-counter+1]-'0')*100 + (data[i-counter+2]-'0')*10 + (data[i-counter+3]-'0');
                        commands[k]=command;
                        break;    
                    case 5:
                        command=(data[i-counter]-'0')*10000+(data[i-counter+1]-'0')*1000 + (data[i-counter+2]-'0')*100 + (data[i-counter+3]-'0')*10 + (data[i-counter+4]-'0');
                        commands[k]=command;
                        break;  
                    }
                counter=0;
                command=0;
                k++;
            }
            else{
                counter++;      
            }
        }
     
        act = commands[0];
        speed = commands[1];
        distance = commands[2];

    } 
}

void stopMotor(){
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(9,LOW);
  digitalWrite(13,LOW);
  
  }  
