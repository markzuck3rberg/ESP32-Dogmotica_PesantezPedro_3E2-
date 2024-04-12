#include <Arduino.h>

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const char* WIFI_SSID = "celu";
const char* WIFI_PASSWORD = "123456789";

const char* API_KEY= "AIzaSyBKXO88g2bpXPrVmVBm0warSPxs1t6eLnA";

const char* DATABASE_URL= "https://rtdb-iot2-default-rtdb.firebaseio.com/";

const char* USER_EMAIL= "insumo@gmail.com";
const char* USER_PASSWORD= "123456789";


FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

const int atico =15;
int buttonStateAtico = 0;
const int banio =16;
int buttonStateBanio = 0;
const int cocina =17;
int buttonStateCocina = 0;
const int dormitorio =18;
int buttonStateDormitorio = 0;
const int garaje =19;
int buttonStateGaraje = 0;
const int sala =21;
int buttonStateSala = 0;


void setup_WIFI(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  config.signer.tokens.legacy_token = "<database secret>";
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}

void setup() {
    Serial.begin(115200);
    setup_WIFI();
    setupFirebase();
    pinMode(banio,INPUT);
}

void loop() {

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
      buttonStateAtico = digitalRead(atico);
      buttonStateBanio = digitalRead(banio);
      buttonStateCocina = digitalRead(cocina);
      buttonStateDormitorio = digitalRead(dormitorio);
      buttonStateGaraje = digitalRead(garaje);
      buttonStateSala = digitalRead(sala);
      
    //Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, F("/sensor/a")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/atico"), buttonStateAtico) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/banio"), buttonStateBanio) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/cocina"), buttonStateCocina) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/dormitorio"), buttonStateDormitorio) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/garaje"), buttonStateGaraje) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/sala"), buttonStateSala) ? "ok" : fbdo.errorReason().c_str());

  }

}
