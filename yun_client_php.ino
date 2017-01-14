#include <Bridge.h>
#include <HttpClient.h>
#include <Console.h>



// connect ssh   ssh root@192.168.1.8 telnet localhost 6571


#include <Bridge.h>
#include <Console.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

int lightLevel;
float humidity;
float temperature;
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT11

//create theDHT instance
DHT dht(DHTPIN, DHTTYPE);

// Relay pin
const int relay_pin = 7;



void setup() {
  int data;
  Bridge.begin();
  dht.begin();
  pinMode(13, OUTPUT);
  Bridge.begin();
  Console.begin();
  while (Console); // wait for a serial connection
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float tempvolts = digitalRead(DHTPIN);
  int data = 8;
  const int relay_pin = 7;
  const int led_pin = 13;

  // Initialize the client library
  HttpClient client;

  client.get("http://funtimequote.com/readRelayData.php");  //see if relay is on/off
  while (client.available()) {
    char c = client.read();
    // Console.print('1');
    Console.print(c);
    //if On
    if (c == '1' ) {
      Console.print("ITS 1 TURN on RELAY");
      //code to turn on relay
      // Relay pin
      //  digitalWrite(13, HIGH); // turn ON LED
      Console.println("LED TURNED ON");

      // Activate relay
      digitalWrite(relay_pin, LOW);  //Remember NO on relay used

      digitalWrite(led_pin, HIGH);  //Remember NO on relay used


    }
    if (c == '0') {
      Console.print("ITS 0 TURN OFF RELAY");
      digitalWrite(led_pin, LOW);  //Remember NO on relay used
    }


  }


  // Launch "curl" command and get Arduino ascii art logo from the network
  // curl is command line program for transferring data using different internet protocols
  Process p;        // Create a process and call it "p"
  p.begin("curl");  // Process that launch the "curl" command
  String myUrl = "http://funtimequote.com/writeData.php?value=";    // write temperature to server
  p.addParameter(myUrl + temperature); // Add the URL parameter to "curl"
  p.run();      // Run the process and wait for its termination
   Console.print(temperature);

  // A process output can be read with the stream methods
  while (p.available() > 0) {
    char c = p.read();     
    Console.print(c);
  }




  /* // Make a HTTP request:
   client.get("http://funtimequote.com/writeData.php?value=data");

   while (client.available()) {

   char c = client.read();
     Console.print(c);
   }


   */

  Console.flush();

  delay(5000);
}
