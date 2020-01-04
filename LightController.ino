// Load Wi-Fi library
#include <WiFi.h>
#include <FS.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid     = "UNICORNSPARKLES";
const char* password = "!@#FairiesFartInTheWoods&UnicornsPoopRainbow$#@!";
IPAddress ip(192,168,40,4);
IPAddress gateway(192,168,30,1);
IPAddress submask(255,255,255,0);
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
File webFile;

// Decode HTTP GET value
String redString = "0";
String greenString = "0";
String blueString = "0";
String whiteString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  //WiFi.config(ip,gateway,submask);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
        return;
  }
  listDir(SPIFFS, "/", 0);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            if(header.indexOf("GET /bootstrap-slider.min.js") > -1){
              Serial.println("Requested js");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/js");
              client.println("Connection: close");
              client.println();
              webFile = SPIFFS.open("/bootstrap-slider.min.js", "r");
              Serial.println("serving page2");
              client.println();
            } else if(header.indexOf("GET /bootstrap-slider.min.css") > -1){
              Serial.println("Requested css");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/css");
              client.println("Connection: close");
              client.println();
              webFile = SPIFFS.open("/bootstrap-slider.min.css", "r");
              Serial.println("serving page2");
              client.println();
            } else {
              Serial.println("Running Default HTML");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
              client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
              client.println("<script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script>");
              client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js\" integrity=\"sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut\" crossorigin=\"anonymous\"></script>");
              client.println("<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js\" integrity=\"sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k\" crossorigin=\"anonymous\"></script>");
              client.println("<script src=\"bootstrap-slider.min.js\"></script>");
              client.println("<link rel=\"stylesheet\" href=\"bootstrap-slider.min.css\"></script>");
              client.println("</head><body><div class=\"container\"><div class=\"row\"><h1>ESP Color Picker</h1></div>");
              //Button for RGB Color
              client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");
              client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>");
              
              //Button for Posting White Value
              client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change White Value</a> ");
              //Slider
              client.println("<input id=\"ex1\" data-slider-id='ex1Slider' type=\"text\" data-slider-min=\"0\" data-slider-max=\"255\" data-slider-step=\"1\" data-slider-value=\"122\"/>");
              client.println("<script> var slider = new Slider('#ex1', {formatter: function(value) {return 'Current value: ' + value;}});");
              client.println("function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
              client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"w\" + document.getElementById('ex1').value + \"&\";}</script></body></html>");
              // The HTTP response ends with another blank line
              client.println();
            } 

            if (webFile) {

              byte clientBuf[64];
              int clientCount = 0; //why an int?  why not byte?
            
              while (webFile.available()){
                clientBuf[clientCount] = webFile.read();
                clientCount++;

                if (clientCount > 63){
                  // Serial.println("Packet");                
                  client.write((const uint8_t *)clientBuf, 64); // DAE added cast to const...
                  clientCount = 0;
                }
              }
                //final <64 byte cleanup packet
              if (clientCount > 0) client.write((const uint8_t *)clientBuf, clientCount); // DAE added cast to const...
            
                // close the file:
                webFile.close();
            } else {
              Serial.println("file !fnd");
            }
            // CSS to style the on/off buttons 

            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('w');
              pos5 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
              whiteString = header.substring(pos4+1, pos5);
              Serial.println(redString.toInt());
              Serial.println(greenString.toInt());
              Serial.println(blueString.toInt());
            }
            
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

/*String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}*/
