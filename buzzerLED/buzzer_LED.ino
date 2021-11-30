
#include "FirebaseESP8266.h" // 반드시 파이어베이스 라이브러리를 먼저 추가해야 합니다.
#include <ESP8266WiFi.h>
 
//#define FIREBASE_HOST "skku-iot-project-default-rtdb.firebaseio.com" //(http:// 빼고, 마지막 / 빼고)" 
#define FIREBASE_HOST "practice2-8dff4-default-rtdb.firebaseio.com" //(http:// 빼고, 마지막 / 빼고)" 
//#define FIREBASE_AUTH "rYdhFhZKvi06qiDSdUjIYTIyWfhfhtr4pAYEk24i"
#define FIREBASE_AUTH "EdwwjeMDe4f57DLkTelsPOHLFOPLyJv187gEOtIw"

#define WIFI_SSID "iPhone" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "123123123" // wifi 비밀번호
 
#define LED_R D1
#define LED_Y D2
#define LED_G D3

#define buzzer D12

 
FirebaseData firebaseData;
FirebaseJson json;
 

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

 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Firebase.setInt(firebaseData, "buzzer", 0);
  Firebase.setInt(firebaseData, "status", 0);

  pinMode(buzzer, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
}

int situation = 0;
int sound = 0;
int value = 0;
int prev = 0;


void loop()
{
  if(Firebase.getInt(firebaseData, "status")){
    if(firebaseData.dataType()=="int"){
      value = firebaseData.intData();
      Serial.println(value);
    }
  }
  

  
 if (value == 0){
   Serial.println("normal");
   normal();
 }
 else if(value == 2){
   Serial.println("warning");
   yellow();
 }
 else if(value == 1){
    Serial.println("danger");
    red();
 }
  delay(100); // wait for next reading
}

void normal(){
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_R, LOW);
  noTone(buzzer);
}

void yellow(){
  digitalWrite(LED_Y, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, LOW);
  tone(buzzer, 700);
  delay(1000);
  noTone(buzzer);
}

void red(){
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_G, LOW);
  for(int i = 0; i < 100; i++){
    tone(buzzer,500+i*5);
    delay(5);
  }
  for(int i = 0; i<100; i++){
    tone(buzzer, 1000-i*5);
    delay(5);
  }
  noTone(buzzer);
}
