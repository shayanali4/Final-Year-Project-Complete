#include <SoftwareSerial.h>
#include<string.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>


#include <max6675.h>

SoftwareSerial RS485 (D4, D8);  //RX, TX

#define FIREBASE_HOST "finalyear-17dc5.firebaseio.com"
#define FIREBASE_AUTH "JX8NZUIbovnfYFMhSnE6Zcv0VeKAmD7CpOtUHlAt"

#define WIFI_SSID "Doctors" 
#define WIFI_PASSWORD "noman1122" 


#define SW1 D2

#define DIR D3               //Enable Pin (LOW for Reciver, HIGH for transmitter)



char str[20];                 //String to Store Complete ASCII value from PLC
char data[5];                 //String to Store Data Values     (0x0000-0xFFFF)
char reg[5];                  //String to Store Register Number (0x0000-0xFFFF)
char address;                 //String to Store Master Address  (0x01-0xFF)
boolean sw1;
long RX;                      //Variable to Store the Read ASCII Value
int i = 0,t=0,a=0,b=99;
long Values;
unsigned int decValue = 0;
long value;

int temp=0,temp1=0,temp2=0,temp3=0,prox1=0,prox2=0,prox3=0,setTemp=0;

String total,control,emergency;
      
  int arr[]={34,64,98};


// ThermoCouple
#define S01 D5
#define S02 D6
#define S03 D7
#define CS D0  
#define SC D1



MAX6675 thermocouple1(SC, CS, S01);
MAX6675 thermocouple2(SC, CS, S02);
MAX6675 thermocouple3(SC, CS, S03);


void reciever();
void transmitter();

void txCondition();

void setup() {
  Serial.begin(9600);
  RS485.begin(9600);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("connecting"); 
 while (WiFi.status() != WL_CONNECTED) { 
  Serial.print("."); 
  delay(500); 
 } 
 Serial.println(); 
 Serial.print("connected: "); 
 Serial.println(WiFi.localIP()); 
 Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 


  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
//  sw1 = digitalRead(SW1);   //Switch 1
  
//if(sw1 == HIGH){
//    digitalWrite(DIR, HIGH);
//    RS485.write(62);  //Sending the value to PLC
//    
//    delay(200);
//    digitalWrite(DIR, LOW);
//    delay(100);
//}


//    digitalWrite(DIR, HIGH);
////    delay(250);
//    RS485.write(62);  //Sending the value to PLC
////delay(100);
//    digitalWrite(DIR, LOW);
////        delay(100);
////          delay(60);
//    delay(200);


      if(a==3){
  a=0;
  }
  

  emergency = Firebase.getString("emergency");
  delay(20);
//  setTemp = Firebase.getInt("setTemp");
//  delay(20);

  
     control=String(temp)+","+emergency;


     
  arr[0]=control.substring(0,2).toInt();
  arr[1]=control.substring(3,4).toInt();
  arr[2]=control.substring(5,8).toInt();

//       Serial.println(arr[a]);
//   b = Firebase.getInt("setTemp");
//  delay(20);
//  Serial.println(emergency);
//  Serial.println(t);
//  sw1 = digitalRead(SW1);   //Switch 

//    temp1=thermocouple1.readCelsius();
    temp2=thermocouple2.readCelsius();
    temp3=thermocouple3.readCelsius();
    temp = ((temp2+temp2+temp3)/3);



//if(sw1 == HIGH){
//    digitalWrite(DIR, HIGH);
//    RS485.write(69);  //Sending the value to PLC
//    digitalWrite(DIR, LOW);
//    delay(100);
////    delay(20);
//}

    //Serial.print("ThermoCouple1: ");
    //Serial.println(a);
    //Firebase.setFloat("Temperature_1", thermocouple1.readCelsius());

   // Serial.print("ThermoCouple2: ");
   // Serial.println(b);
   // Firebase.setFloat("Temperature_2", thermocouple2.readCelsius());

   // Serial.print("ThermoCouple3: ");
   // Serial.println(c);
   // Firebase.setFloat("Temperature_3", thermocouple3.readCelsius());




  transmitter();
  txCondition();
  reciever();
  //  char hexNum[] = "FFFF";
  value = hexadecimalToDecimal(data);

}

void txCondition()
{

  if (!strcmp(reg, "0000"))
  {
    if (!strcmp(data, "0005"))
    {
prox1=1;
prox2=0;
prox3=0;
    }
    else if (!strcmp(data, "0006"))
    {

prox1=0;
prox2=1;
prox3=0;
    }
    else if (!strcmp(data, "0007"))
    {
prox1=0;
prox2=0;
prox3=1;
    }
    else if (!strcmp(data, "0008"))
    {
prox1=0;
prox2=0;
prox3=0;
    }
  }
  total=String(prox1)+","+String(prox2)+","+String(prox3)+","+String(temp2)+","+String(temp2)+","+String(temp3);
    Firebase.setString("NodeMCU",total);
    delay(20);
//    Serial.println(total);
//
//  delay(80);



}

void transmitter()
{
  if (Serial.available())
  {
    RX = Serial.read();            // Read the byte
    //  Serial.write(RX);           // To Print ASCII Value
    if (RX == '0')                // Starting Address of bit to pick the sequence
    {
      while (1)
      {
        if (Serial.available())
        {
          RX = Serial.read();      //Read the byte
          //        Serial.write(RX);
          str[i] = RX;            //To store the values (Address[0], Register[3]-[6], Data[7]-[10])
          i++;
          if (RX == ':')          //To end the sequence
          {
            i = 0;
            break;
          }
        }
      }
   } 
//   Serial.print("Address: ");
//  Serial.print(address);
//  Serial.print("  -   Register No: ");
//  Serial.print(reg);
//  Serial.print("  -   Data: ");
//  Serial.print(data);
//  Serial.print("  -   Value: ");
//  Serial.println(value);
  }

  
  address = str[0];               //Storing the value in address

  reg[0] = str[3]; //hex          //Storing the value in register array
  reg[1] = str[4];
  reg[2] = str[5];
  reg[3] = str[6];

  data[0] = str[7]; //hex         //Storing the value in data array
  data[1] = str[8];
  data[2] = str[9];
  data[3] = str[10];


//
  //  Serial.print("  -   Values: ");
  //  Serial.println(Values);

//  if(sw1 == HIGH){
    digitalWrite(DIR, HIGH);
    delay(100);
    
    RS485.write(arr[a]);  //Sending the value to PLC
    delay(100);
//    RS485.write(69);  //Sending the value to PLC
//    
//    RS485.write(32);  //Sending the value to PLC       
//    delay(100);
    
    digitalWrite(DIR, LOW);
//    delay(100);
////    delay(20);
//}
a++;
}


void reciever()
{
//    t = Firebase.getInt("data");
//


//if(sw1 == HIGH){
//    digitalWrite(DIR, HIGH);
//    RS485.write(44);  //Sending the value to PLC
//    digitalWrite(DIR, LOW);
//    delay(100);
////    delay(20);
//}

  
//    digitalWrite(DIR, HIGH);
//    RS485.write(65);  //Sending the value to PLC
//    digitalWrite(DIR, LOW);
//    delay(50);


//   else if(a == 2){
//    digitalWrite(DIR, HIGH);
//    RS485.write(10);
//    digitalWrite(DIR, LOW);
//    delay(50);
//  }
//  else if(a == 3){
//    digitalWrite(DIR, HIGH);
//    RS485.write(60);
//    digitalWrite(DIR, LOW);
//    delay(50);
//  }
    
//    digitalWrite(DIR, HIGH);
//    RS485.write(emergency);
//    digitalWrite(DIR, LOW); 
//    delay(10); 
    
//    digitalWrite(DIR, LOW);

//    delay(50);
    
//    digitalWrite(DIR, HIGH);
//    RS485.write(t);               //Sending the value to PLC
//    digitalWrite(DIR, LOW);
//    delay(50);
//
//    digitalWrite(DIR, HIGH);
//    RS485.write(emergency);               //Sending the value to PLC
//    digitalWrite(DIR, LOW);
//    delay(50);

  //Serial.print(sw1);

}

// Converting from Hex to Decimal:

// NOTE: This function can handle a positive hex value from 0 - 65,535 (a four digit hex string).
//       For larger/longer values, change "unsigned int" to "long" in both places.


int hexadecimalToDecimal(char hexVal[])
{
  int len = strlen(hexVal);

  // Initializing base value to 1, i.e 16^0
  int base = 1;

  int dec_val = 0;

  // Extracting characters as digits from last character
  for (int i = len - 1; i >= 0; i--)
  {
    // if character lies in '0'-'9', converting
    // it to integral 0-9 by subtracting 48 from
    // ASCII value.
    if (hexVal[i] >= '0' && hexVal[i] <= '9')
    {
      dec_val += (hexVal[i] - 48) * base;

      // incrementing base by power
      base = base * 16;
    }

    // if character lies in 'A'-'F' , converting
    // it to integral 10 - 15 by subtracting 55
    // from ASCII value
    else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
    {
      dec_val += (hexVal[i] - 55) * base;

      // incrementing base by power
      base = base * 16;
    }
  }

  return dec_val;
}
