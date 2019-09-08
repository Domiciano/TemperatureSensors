#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x31, 0x31 };
IPAddress subnet(255, 255, 255, 0);
IPAddress ip(192, 168, 0, 50);
IPAddress gateway(192, 168, 0, 1);
IPAddress dnsserver(8, 8, 8, 8);


EthernetClient client;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;

void setup() {
  Serial.begin(57600);
  Ethernet.begin(mac,ip,dnsserver,gateway,subnet);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(1000); 

  
   
}

void sendPOST(){
   IPAddress server(192,168,0,2);
   Serial.println("connecting to server");
   Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 8080)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    

    String PostData = "{";
    PostData = PostData + "\"Hola\"\"Hijueputa\"";
    PostData = PostData + "}";


    // Make a HTTP request:
    client.println("POST /ApiRESTEjemplo/web/first/temp HTTP/1.1");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.println("Accept: */*");
    client.println("Host: camara-4a96c.firebaseio.com");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println("Connection: keep alive");
    client.println();
    client.println(PostData);

    //GET RESPONSE
    int len = client.available();
    if (len > 0) {
      byte buffer[80];
      if (len > 80) len = 80;
      client.read(buffer, len);
      if (printWebData) {
        Serial.write(buffer, len); // show in the serial monitor (slows some boards)
      }
      byteCount = byteCount + len;
    }
  
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      endMicros = micros();
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      Serial.print("Received ");
      Serial.print(byteCount);
      Serial.print(" bytes in ");
      float seconds = (float)(endMicros - beginMicros) / 1000000.0;
      Serial.print(seconds, 4);
      float rate = (float)byteCount / seconds / 1000.0;
      Serial.print(", rate = ");
      Serial.print(rate);
      Serial.print(" kbytes/second");
      Serial.println();  
    }
    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }


  
}



void sendGET(){
   char server[] = "192.168.0.2";
   Serial.println("connecting to server");
   Serial.println("...");

  // if you get a connection, report back via serial:
  int response = client.connect(server, 8080);
  Serial.print("RESPUESTA: ");
  Serial.println(response);
  if (response) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /index.jsp HTTP/1.1");
    client.println("Host: 192.168.0.2");
    client.println("Connection: close");
    client.println();


    //RESPUESTA
    while(true){
      int len = client.available();
      if (len > 0) {
        byte buffer[80];
        if (len > 80) len = 80;
        client.read(buffer, len);
        if (printWebData) {
          Serial.write(buffer, len); // show in the serial monitor (slows some boards)
        }
        byteCount = byteCount + len;
      }
    
      if (!client.connected()) {
        Serial.println("Disconnecting...");
        client.stop();
        Serial.println("Closed");
        delay(5000);
        break;
      }
    }
 
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  

  
  
  
}

void loop() {
  sendGET();
}
