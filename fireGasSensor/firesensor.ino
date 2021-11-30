
#include "FirebaseESP8266.h" // 반드시 파이어베이스 라이브러리를 먼저 추가해야 합니다.
#include <ESP8266WiFi.h>
 
//#define FIREBASE_HOST "skku-iot-project-default-rtdb.firebaseio.com" //(http:// 빼고, 마지막 / 빼고)" 
#define FIREBASE_HOST "practice2-8dff4-default-rtdb.firebaseio.com" //(http:// 빼고, 마지막 / 빼고)" 
//#define FIREBASE_AUTH "rYdhFhZKvi06qiDSdUjIYTIyWfhfhtr4pAYEk24i"
#define FIREBASE_AUTH "EdwwjeMDe4f57DLkTelsPOHLFOPLyJv187gEOtIw"
#define WIFI_SSID "iPhone" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "123123123" // wifi 비밀번호
 
#define gasSensor1 A0
//#define gasSensor2 D9
//#define gasSensor3 D10
//#define gasSensor4 D11

#define flameSensor1 D4
#define flameSensor2 D2
#define flameSensor3 D3
#define flameSensor4 D6

float gasSensorValue1;  //variable to store sensor value
//float gasSensorValue2;  //variable to store sensor value
//float gasSensorValue3;  //variable to store sensor value
//float gasSensorValue4;  //variable to store sensor value

int flameSensorValue1;
int flameSensorValue2;
int flameSensorValue3;
int flameSensorValue4;

int check = false;
 
FirebaseData firebaseData;
FirebaseJson json;
 
void printResult(FirebaseData &data);
void setup() // wifi 접속 과정.
{
  Serial.begin(9600);
  delay(500);
  Serial.println();
  Serial.println(WIFI_SSID);
  Serial.println(WIFI_PASSWORD);
  delay(500);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(500);
  Serial.println();
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  pinMode(gasSensor1, INPUT);
  pinMode(flameSensor1,INPUT);
  pinMode(flameSensor2, INPUT);
  pinMode(flameSensor3, INPUT);
  pinMode(flameSensor4, INPUT);
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop()
{

  gasSensorValue1 = analogRead(gasSensor1); // read analog input pin 1
  //gasSensorValue2 = analogRead(gasSensor2); // read analog input pin 2
  //gasSensorValue3 = analogRead(gasSensor3); // read analog input pin 3
  //gasSensorValue4 = analogRead(gasSensor4); // read analog input pin 4
  
  flameSensorValue1 = digitalRead(flameSensor1);
  flameSensorValue2 = digitalRead(flameSensor2);
  flameSensorValue3 = digitalRead(flameSensor3);
  flameSensorValue4 = digitalRead(flameSensor4);
  
  Serial.print("gas Sensor1 Value: ");
  Serial.println(gasSensorValue1);
  /*Serial.print("gas Sensor2 Value: ");
  Serial.println(gasSensorValue2);
  Serial.print("gas Sensor3 Value: ");
  Serial.println(gasSensorValue3);
  Serial.print("gas Sensor4 Value: ");
  Serial.println(gasSensorValue4);
  */
  
  Serial.print("flame Sensor1 Value: ");
  Serial.println(flameSensorValue1);
  Serial.print("flame Sensor2 Value: ");
  Serial.println(flameSensorValue2);
  Serial.print("flame Sensor3 Value: ");
  Serial.println(flameSensorValue3);
  Serial.print("flame Sensor4 Value: ");
  Serial.println(flameSensorValue4);

  //flame sensor value
  if(flameSensorValue1 == 0){
    Firebase.setBool(firebaseData, "flameSensorValue1", !flameSensorValue1);
    Firebase.setInt(firebaseData, "status", 1);
    check = true;
    Serial.println("area 1 is fire");
  }
  if(flameSensorValue2 == 0){
    Firebase.setBool(firebaseData, "flameSensorValue2", !flameSensorValue2);
    Firebase.setInt(firebaseData, "status", 1);
    check = true;
    Serial.println("area 1 is fire");
  }
  if(flameSensorValue3 == 0){
    Firebase.setBool(firebaseData, "flameSensorValue3", !flameSensorValue3);
    Firebase.setInt(firebaseData, "status", 1);
    check = true;
    Serial.println("area 1 is fire");
  }
  if(flameSensorValue4 == 0){
    Firebase.setBool(firebaseData, "flameSensorValue4", !flameSensorValue4);
    Firebase.setInt(firebaseData, "status", 1);
    Serial.println("area 1 is fire");
    
  }

  if(gasSensorValue1 > 200 && (flameSensorValue1 * flameSensorValue2 * flameSensorValue3 * flameSensorValue4 == 1) ){
    Firebase.setInt(firebaseData, "gasSensorValue1", gasSensorValue1);
    Firebase.setInt(firebaseData, "status", 2);
    check = true;
    Serial.println("area 1 is warning");
  }
  
  if(!check){
    Firebase.setBool(firebaseData, "flameSensorValue1", !flameSensorValue1);
    Firebase.setBool(firebaseData, "flameSensorValue2", !flameSensorValue2);
    Firebase.setBool(firebaseData, "flameSensorValue3", !flameSensorValue3);
    Firebase.setBool(firebaseData, "flameSensorValue4", !flameSensorValue4);
    Firebase.setInt(firebaseData, "gasSensorValue1", gasSensorValue1);
    Firebase.setInt(firebaseData, "status", 0);
  }
  check = false;

  
  Serial.println("");
  delay(2000); // wait 2s for next reading

}
