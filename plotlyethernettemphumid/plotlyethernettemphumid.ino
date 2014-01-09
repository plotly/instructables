// Using Plot.ly's Arduino API to visualize Temperature and Humidity Readings from A DHT22 Sensor

#include <SPI.h>
#include <Ethernet.h>
#include "plotly_ethernet.h"
#include "DHT.h"


// DHT Sensor Setup
#define DHTPIN 2     // We have connected the DHT to Digital Pin 2
#define DHTTYPE DHT22 // This is the type of DHT Sensor (Change it to DHT11 if you're using that model)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT object

// Plotly Setup
plotly plotly; // initialize a plotly object
int nTraces=2; // number of traces/series of the graph
int nPoints=1; // number of points to transmit for each trace -- all traces must transmit the same number of points
char layout[]="{}"; // Not needed as we will be styling through the Plot.ly web GUI
char filename[] = "Plotly Arduino API"; // name of the plot that will be saved in your plotly account -- resaving to the same filename will simply extend the existing traces with new data


// Ethernet Setup
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // doesn't really matter
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"

void startEthernet(){
    Serial.println("Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("Done initializing ethernet");
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

  plotly.VERBOSE = true; // turn to false to suppress printing over serial
  plotly.DRY_RUN = false; // turn to false when you want to connect to plotly's servers
  plotly.username = "username"; // your plotly username -- sign up at https://plot.ly/ssu
  plotly.api_key = "api_key"; // your plot.ly API key --> if you don't have one : plot.ly/api/key
  plotly.timestamp = true; // Tell plotly this is a time/date graph and plotly will convert the x data automatically
  plotly.timezone = "America/Montreal"; // Timezone -- > You can check out a full list in the Plotly-Arduino API
}

void loop() {


    plotly.open_stream(nPoints, nTraces, filename, layout); // open stream to send nPoints and nTraces to plotly

    float h = dht.readHumidity(); // read humitidy from DHT pin
    float t = dht.readTemperature() - 8; // read temperature from DHT pin


    plotly.post(millis(),h); // post humidity to plotly
    delay(750);
    plotly.post(millis(),t); // post temperature to plotly
    delay(4*60000);

  }
