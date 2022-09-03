/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-web-server-sent-events-sse/
                              https://randomnerdtutorials.com/esp8266-nodemcu-web-server-sent-events-sse/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESPAsyncWebServer.h>
#include <ThingerESP8266.h>
#include <ArduinoJson.h>

/*#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>*/

// Replace with your network credentials
const char* ssid = "Gersonline";
const char* password = "carinegata";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create an Event Source on /events
AsyncEventSource events("/events");

// Timer variables
/*unsigned long lastTime = 0;  
unsigned long timerDelay = 3000000;*/
#define SENSOR  D7
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
//boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

// Create a sensor object
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}


/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

// Initialize WiFi
void initWiFi() {
    display.begin(i2c_Address, true); // Address 0x3C default
  
    // Clear the buffer.
    display.clearDisplay();

    // text display tests
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Failure is always an option");
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.display();
    display.clearDisplay();
      
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.print("Conectando no WiFi ..");
    display.display();
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        display.print(".");
        display.display();
        delay(3000);
    }
    Serial.println(WiFi.localIP());
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
    display.println(WiFi.localIP());
    display.display();
}

//aqui Jerson!!
String processor(const String& var){
  //getSensorReadings();
  //Serial.println(var);
  
  if(var == "totalMilliLitres"){
    return String(totalMilliLitres);
  }
  else if(var == "flowMilliLitres"){
    return String(flowMilliLitres);
  }
  else if(var == "flowRate"){
    return String(flowRate);
  }
  return String();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <script src="gauge.min.js"></script>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #FF8C00; color: white; font-size: 1rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 1.4rem; }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>Smart Hydrometer</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p><i class="fas fa-tint" style="color:#00add6;"></i> Consumo Total</p>
        <p><span class="reading"><span id="TotalConsumo">%TotalConsumo%</span> ml</p>
        <hr><br>
        <p><span class="reading"><span id="Litros">%Litros%</span> L</p>
        <hr><br>
        <p><span class="reading"><span id="m3">%m3%</span> m3</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-angle-double-down" style="color:#059e8a;"></i> Litros/Min</p><p><span class="reading"><span id="Litro_Min">%Litro_Min%</span> L/Min</span></p>
      <!-- <div id="chart_div" style="background-color: transparent; width: 100px; height: 100px; position: relative; margin-left: auto; margin-right: auto;" ></div>-->
      </div>
      <div class="card">
        <p><i class="fas fa-money-bill" style="color:#e1e437;"></i> Custo</p><p><span class="reading">R$ <span id="conta">%reais%</span></span></p>
      </div>
    </div>
  </div>
  <div class="content">
    <div class="cards">
      <div style="background-color: #343a40;" class="card">
        <p style="color: white;">Powered By: Gerson Lima</p>
      </div>
    </div>
  </div>
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 google.charts.load('current', {'packages':['gauge']});
 google.charts.setOnLoadCallback(drawChart);
 
function drawChart() {

        var data = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['l/min', 0]/*,
          ['CPU', 0],
          ['Network', 0]*/
        ]);

        var options = {
          width: 400, height: 120,
          redFrom: 90, redTo: 100,
          yellowFrom:75, yellowTo: 90,
          minorTicks: 5
        };

        var chart = new google.visualization.Gauge(document.getElementById('chart_div'));

        chart.draw(data, options);

        setInterval(function() {
          source.addEventListener('flowRate', function(e) {
          data.setValue(0, 1, Math.round(e.data));
          chart.draw(data, options);
          }, false);
        }, 50);

        /*setInterval(function() {
          data.setValue(1, 1, 40 + Math.round(60 * Math.random()));
          chart.draw(data, options);
        }, 50);
        setInterval(function() {
          data.setValue(2, 1, 60 + Math.round(20 * Math.random()));
          chart.draw(data, options);
        }, 50);*/
      }

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 

 source.addEventListener('totalMilliLitres', function(e) {
  console.log("totalMilliLitres", e.data);
  document.getElementById("TotalConsumo").innerHTML = e.data;
 }, false);
 
 source.addEventListener('Litros', function(e) {
  console.log("Litros", e.data);
  document.getElementById("Litros").innerHTML = e.data;
 }, false);
 
 source.addEventListener('flowRate', function(e) {
  console.log("flowRate", e.data);
  document.getElementById("Litro_Min").innerHTML = e.data;
 }, false);

 source.addEventListener('m3', function(e) {
  console.log("m3", e.data);
  document.getElementById("m3").innerHTML = e.data;
 }, false);
 source.addEventListener('conta', function(e) {
  console.log("conta", e.data);
  document.getElementById("conta").innerHTML = e.data;
 }, false);
 
}
</script>
</body>
</html>)rawliteral";

void setup() {
  pinMode(SENSOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  pulseCount = 0;
  flowRate = 0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  
  Serial.begin(115200);
  //thing.add_wifi(SSID, SSID_PASSWORD);
  initWiFi();
  
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(0, "text/html", index_html, processor);
  });

  server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request) {
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      DynamicJsonDocument json(1024);
      json["status"] = "ok";
      json["ssid"] = WiFi.SSID();
      json["ip"] = WiFi.localIP().toString();
      json["totalMilliLitres"] = totalMilliLitres;
      json["flowRate"] = flowRate;

      serializeJson(json, *response);
      request->send(response);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    
    client->send("hello!", NULL, millis(), 1000);
    
  });
  server.addHandler(&events);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
  
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
   

    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("ml / ");
    int Litros = totalMilliLitres / 1000;
    Serial.print(Litros);
    Serial.println("L");
    int m3 = Litros / 1000;
    Serial.print(m3,5);
    Serial.println("m³: ");
    float conta = 0.00;
    conta = ((Litros * 2.94)*0.09);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.setTextSize(1);
    //display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
    display.print("---------------------");
    display.setTextColor(SH110X_WHITE);
    display.print("litros/min: ");
    display.println(int(flowRate));
    display.print("---------------------");
    display.print("ml: ");
    display.print(int(totalMilliLitres));
    display.print(" | litros: ");
    display.println(int(Litros));
    display.print("---------------------");
    display.print("metros cubicos: ");
    display.print(m3,5);
    display.display();

  // Send Events to the Web Server with the Sensor Readings
    events.send("ping",NULL,millis());
    events.send(String(int(flowRate)).c_str(),"flowRate",millis());
    events.send(String(totalMilliLitres).c_str(),"totalMilliLitres",millis());
    events.send(String(Litros).c_str(),"Litros",millis());
    events.send(String(m3).c_str(),"m3",millis());
    events.send(String(conta).c_str(),"conta",millis());
    

    
    //lastTime = millis();
  }
   
}
