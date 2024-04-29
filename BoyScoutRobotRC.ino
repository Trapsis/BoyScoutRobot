/*  Remote Control robot source code designed 
 *  for the Robotics merit badge for the Boy Scouts
 *  This is for the DC motor / motor controller shield version
 *  ESP8266 NodeMCU
 *  
 *  Running this code creates a WiFi hotspot called CarWiFi that you can connect to.  
 *  Browse to 192.168.4.1 after connecting to control the robot 
 * 
 *   By Bill Caterino  -  ZigZone Automation
 */
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


// Motor shield pins
#define DIRA 0
#define PWMA 5
#define DIRB 2
#define PWMB 4

int DCgo = 1023;  // 0 (stop) - 1023 (max)
int DCstop = 0;

// WiFi credentials
const char *ssid = "CarWiFi";  // SSID of the car's WiFi network
const char *password = "carpassword";  // Password for the car's WiFi network

// Create a web server
ESP8266WebServer server(80);

// HTML page
String htmlPage = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
  <head>
    <title>WiFi Car Controller</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
      }
    .button {
        background-color: #4CAF50;
        color: #ffffff;
        padding: 10px 20px;
        width: 100px;
        height: 80px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
      }
    .button:hover {
        background-color: #3e8e41;
      }
    .table {
        margin-left: auto;
        margin-right: auto;
      }
    </style>
  </head>
  <body>
    <figure class="table">
        <table align="center">
            <tbody>
                <tr>
                    <td>
                        <h1>WiFi Car Controller</h1>
                    </td>
                </tr>
                <tr>
                    <td>
                        <p style="text-align:center;">Car is stopped</p>
                    </td>
                </tr>
            </tbody>
        </table>
    </figure>
    <figure class="table">
        <table align="center">
            <tbody>
                <tr>
                    <td>&nbsp;</td>
                    <td>
                        <div class="raw-html-embed" style="text-align:center;"><button class="button" onclick="sendCommand('forward')">FWD</button></div>
                    </td>
                    <td>&nbsp;</td>
                </tr>
                <tr>
                    <td>
                        <div class="raw-html-embed" style="text-align:center;"><button class="button" onclick="sendCommand('left')">L</button></div>
                    </td>
                    <td>
                        <div class="raw-html-embed" style="text-align:center;"><button class="button" onclick="sendCommand('stop')">Stop</button></div>
                    </td>
                    <td>
                        <div class="raw-html-embed" style="text-align:center;"><button class="button" onclick="sendCommand('right')">R</button></div>
                    </td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                    <td>
                        <div class="raw-html-embed" style="text-align:center;"><button class="button" onclick="sendCommand('backward')">BACK</button></div>
                    </td>
                    <td>&nbsp;</td>
                </tr>
            </tbody>
        </table>
    </figure>
    <table align="center">
      <tr>
        <td><button class="button" onclick="sendCommand('slower')">SLOW</button></td>
        <td><button class="button" onclick="sendCommand('faster')">FAST</button></td>
      </tr>
    </table>
    <script>
      function sendCommand(command) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/" + command, true);
        xhr.send();
      }
    </script>
  </body>
  </html>
)rawliteral";

void setup() {
  delay(1000);
  Serial.begin(115200);

  // Initialize motor pins as outputs
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Set up WiFi as an access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  //delay(100);
  setupServer();

  // Start web server
  server.begin();
  Serial.println("Car WiFi network started");
  Serial.println("SSID: " + String(ssid));
  Serial.println("Password: " + String(password));
  Serial.print("IP address: ");
  Serial.println(myIP);
}

void loop() {
  server.handleClient();
}

void handleForward() {
  Serial.println("handleForward");
  digitalWrite(DIRA,1); //Set forward direction A
  digitalWrite(DIRB,1); //Set forward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B    
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is stopped", "Car is moving forward");
  server.send(200, "text/html", newHtmlPage);
}

void handleBackward() {
  Serial.println("handleBackward");
  digitalWrite(DIRA,0); //Set backward direction A
  digitalWrite(DIRB,0); //Set backward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B   
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is stopped", "Car is moving backward");
  server.send(200, "text/html", newHtmlPage);
}

void handleLeft() {
  Serial.println("handleLeft");
  digitalWrite(DIRA,0); //Set backward direction A
  digitalWrite(DIRB,1); //Set forward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B  
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is stopped", "Car is turning left");
  server.send(200, "text/html", newHtmlPage);
}

void handleRight() {
  Serial.println("handleRight");
  digitalWrite(DIRA,1); //Set forward direction A
  digitalWrite(DIRB,0); //Set backward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B 
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is stopped", "Car is turning right");
  server.send(200, "text/html", newHtmlPage);
}

void handleStop() {
  Serial.println("handleStop");
  digitalWrite(DIRA,1); //Set forward direction A
  digitalWrite(DIRB,1); //Set forward direction B 
  analogWrite(PWMA,DCstop); //Turn off motor A
  analogWrite(PWMB,DCstop); //Turn off motor B  
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is moving", "Car is stopped");
  server.send(200, "text/html", newHtmlPage);
}

void handleFaster() {
  // Increase motor speed (not implemented)
  Serial.println("handleFaster");
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is stopped", "Car is moving faster");
  server.send(200, "text/html", newHtmlPage);
}

void handleSlower() {
  // Decrease motor speed (not implemented)
  Serial.println("handleSlower");
  String newHtmlPage = htmlPage;
  newHtmlPage.replace("Car is stopped", "Car is moving slower");
  server.send(200, "text/html", newHtmlPage);
}

void handleRoot() {
  Serial.println("handleRoot");
  server.send(200, "text/html", htmlPage);
}

void setupServer() {
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);
  server.on("/faster", handleFaster);
  server.on("/slower", handleSlower);
}