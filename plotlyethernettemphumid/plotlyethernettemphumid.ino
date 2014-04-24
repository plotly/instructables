// Using Plot.ly's Arduino API to visualize Temperature and Humidity Readings from A DHT22 Sensor

#include <Ethernet.h>
#include <SPI.h>
#include "plotly_streaming_ethernet.h"
#include "DHT.h"


// DHT Sensor Setup
#define DHTPIN 2 // We have connected the DHT to Digital Pin 2
#define DHTTYPE DHT22 // This is the type of DHT Sensor (Change it to DHT11 if you're using that model)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT object

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 2
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"token_1", "token_2"};
// arguments: username, api key, streaming token, filename
plotly graph = plotly("plotly_username", "plotly_api_key", tokens, "your_filename", nTraces);


// Ethernet Setup
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // doesn't really matter
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"

void startEthernet(){
    Serial.println("... Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("... Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("... Done initializing ethernet");
    delay(1000);
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
    
  dht.begin(); // initialize dht sensor reading
  startEthernet();    // initialize ethernet
  
  // Initialize plotly settings
  graph.fileopt="overwrite"; // See the "Usage" section in https://github.com/plotly/arduino-api for details
  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
  graph.convertTimestamp = true; // tell plotly that you're stamping your data with a millisecond counter and that you want plotly to convert it into a date-formatted graph
  graph.timezone = "America/Montreal"; // full list of timezones is here:
}

float h, t;
void loop() {
      h = dht.readHumidity(); // read humitidy from DHT pin
      t = dht.readTemperature() - 5; // read temperature from DHT pin (ours calibrated accuractly to our home thermostat with -5 to the temperature)

      graph.plot(millis(), h, tokens[0]);
      graph.plot(millis(), t, tokens[1]);
  }
