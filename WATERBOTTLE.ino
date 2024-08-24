#include<ESP8266WiFi.h>
#include <WiFiManager.h> 
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPL3ZlvGZ6lH"
#define BLYNK_TEMPLATE_NAME "water bottle"


const int flowPin =D2;
double flowRate;
double totalFlow=0;
volatile int count =0;
bool flowDetected =false;
double initialWaterLevel=5000;
double waterLevel = 5000;
char auth[] = "ShpfrWiaJXBj1KDwM3C-eMv1JO7clfoe";

void ICACHE_RAM_ATTR flow();


void setup() {
  Serial.begin(115200);
  pinMode(flowPin,INPUT);
  attachInterrupt(digitalPinToInterrupt (flowPin),flow,
  RISING);
  Serial.print("Initial Water level =");
  Serial.println(waterLevel);
  
  delay(1000);

  
    WiFiManager wm;
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("WATERBOTTLE", "password");
  
  if (!res) {
    Serial.println("Failed to connect to WiFi");
    // ESP.restart(); // Uncomment to restart the ESP if WiFi fails
  } else {
    Serial.println("Connected to WiFi");
    Serial.println("Connecting to Blynk...");
    Blynk.config(auth);
    if (Blynk.connect()) {
      Serial.println("Connected to Blynk");
    } else {
      Serial.println("Failed to connect to Blynk");
    }
  }
   
  }

void loop() {
   Blynk.run();
  count=0;
  delay(1000);
  if (count>0){
    flowDetected = true;
    flowRate = (count * 2.25);         
    flowRate = flowRate * 60;       
    
    totalFlow += flowRate / 60;        // Convert flow to milliliters per minute and add to total flow
    waterLevel -= flowRate / 60;       // Subtract flow from water level (in mL)
 
if(waterLevel<0){
  waterLevel=0;

  }
  //Serial.print("Total Flow: ");
  //Serial.print(totalFlow);
  //Serial.println("mL");
  Serial.print("water level: ");
  Serial.print(waterLevel);
  Serial.println("mL");
  Blynk.virtualWrite(V1,waterLevel );
  
if (waterLevel <= 1) {
  Serial.println("Warning: Water is Empty");
  resetBlynkOutputs();  // Reset when water is empty
}


}
}
void ICACHE_RAM_ATTR flow(){
  count++;
  
}
void resetBlynkOutputs() {
  waterLevel = initialWaterLevel;
  

  // Reset Blynk virtual pins
  Blynk.virtualWrite(V1, waterLevel);  // Reset water level
 

}
BLYNK_WRITE(V0) {
  int buttonState = param.asInt();  // Get button state

  if (buttonState == 1) {
    resetBlynkOutputs();  // Reset when button is pressed
  }
}
