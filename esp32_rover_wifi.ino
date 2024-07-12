#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ROVER-ESP32";
const char* password = "rover123"; 

WebServer server(80);

const int pwm1Pin = 18;
const int pwm2Pin = 19;

void setup() {

  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/slider1", handleSlider1);
  server.on("/slider2", handleSlider2);
  server.begin();

  ledcSetup(1, 50, 12);
  ledcAttachPin(pwm1Pin, 1);
  ledcSetup(2, 50, 12);  
  ledcAttachPin(pwm2Pin, 2);

  ledcWrite(1, 307);
  ledcWrite(2, 307);
}

void loop() {
  server.handleClient();
}

void handleRoot() {

  String html = "<html>";
  
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";  
  html += "<style>";
  html += "body { font-family: Arial; }";
  html += ".slider-container { display: flex; align-items: center; }";
  html += ".slider-wrapper { margin-right: 20px; display: flex; flex-direction: column; align-items: center; }";
  html += ".slider-label { margin: 0; }";
  html += ".vertical-slider { writing-mode: bt-lr; width: 100px; height: 200px; transform: rotate(-90deg); }";
  html += ".horizontal-slider { width: 200px;transform: rotate(-180deg); }";
  html += "</style>";
  html += "</head>";
  
  html += "<body>";
  
  html += "<div class='slider-container'>";

html += "<div class='slider-wrapper'>";
html += "<p class='slider-label'>BLDC</p>";
html += "<input type='range' min='1500' max='2000' value='0' id='pwm1' oninput='slider1(value)' class='vertical-slider' orient='vertical'/>";
html += "<span id='slider-value'></span>"; // Agregar un elemento span para mostrar el valor
html += "</div>";

  html += "<div class='slider-wrapper'>";
  html += "<p class='slider-label'>SERVO</p>";
  html += "<input type='range' min='1500' max='2000' value='1500' id='pwm2' oninput='slider2(value)' class='horizontal-slider' orient='horizontal'/>";
  html += "</div>";
  
  html += "</div>";
  
  html += "<script>";
  html += "function slider1(pos) {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open(\"GET\", \"/slider1?value=\" + pos, true);";
  html += "xhr.send();";
  html += "}";

  html += "function slider2(pos) {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open(\"GET\", \"/slider2?value=\" + pos, true);";
  html += "xhr.send();";
  html += "}";
  html += "</script>";

  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
  
}

void handleSlider1() {
  int var=server.arg(0).toInt();
  ledcWrite(1, int(var*128/625));
  Serial.println(var);
}

void handleSlider2() {
  int var=server.arg(0).toInt();
  ledcWrite(2, int(var*128/625));
  Serial.println(server.arg(0).toInt());
}