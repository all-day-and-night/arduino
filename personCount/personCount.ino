
#include "FirebaseESP8266.h" // 반드시 파이어베이스 라이브러리를 먼저 추가해야 합니다.
#include <ESP8266WiFi.h>
 
//#define FIREBASE_HOST "skku-iot-project-default-rtdb.firebaseio.com" //(http:// 빼고, 마지막 / 빼고)" 
#define FIREBASE_HOST "practice2-8dff4-default-rtdb.firebaseio.com" //(http:// 빼고, 마지막 / 빼고)" 
//#define FIREBASE_AUTH "rYdhFhZKvi06qiDSdUjIYTIyWfhfhtr4pAYEk24i"
#define FIREBASE_AUTH "EdwwjeMDe4f57DLkTelsPOHLFOPLyJv187gEOtIw"
#define WIFI_SSID "iPhone" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "123123123" // wifi 비밀번호

#define IRSensor1 A0


FirebaseData firebaseData;
FirebaseJson json;
 


void setup() {
  // put your setup code here, to run once:
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
  pinMode(IRSensor1, INPUT);
}

int IRSensorValue1 = 0;
int prev = 0;
int cnt = 0;
bool check1 = false;
bool check2 = false;

int threshold1 = 600;
int threshold2 = 200;

void loop() {
  // put your main code here, to run repeatedly:
  IRSensorValue1 = analogRead(IRSensor1);
  

  Serial.print("IRSensor value1 : ");
  Serial.println(IRSensorValue1);
  Serial.print("cnt: ");
  Serial.println(cnt);

  if (IRSensorValue1 < threshold1 && IRSensorValue1 > threshold2 && !check1 && !check2){
    check1 = true;
  }
  else if (IRSensorValue1 > threshold1 && check1 && !check2){
    cnt++;
    Firebase.setInt(firebaseData, "count", cnt);
    check1 = false;
  }

  else if(IRSensorValue1 > threshold1 && !check2 && !check1)  {
    check2 = true;
  }
  else if(IRSensorValue1 < threshold1 && IRSensorValue1 > threshold2 && check2 && !check1){
    check2 = false;
    if(cnt > 0){
      cnt--;
      Firebase.setInt(firebaseData, "count", cnt);
    }
  }
  else{
    check1 = false;
    check2 = false;
  }
  
  delay(1000);
}
