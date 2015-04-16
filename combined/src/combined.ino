#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
/* #include "utility/debug.h" */

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

#define WLAN_SSID "myhotspot"
#define WLAN_PASS "qwerty0987"
#define WLAN_SECURITY WLAN_SEC_WPA2

#define LISTEN_PORT 7

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS,
                                         ADAFRUIT_CC3000_IRQ,
                                         ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER);
// you can change this clock speed but DI

Adafruit_CC3000_Server echoServer(LISTEN_PORT);

int var = 1;

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n")); 

  /* Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC); */
  
  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
    {
      Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
      while(1);
    }
  
  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("Connected!"));
  
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
    {
      delay(100); // ToDo: Insert a DHCP timeout!
    }  
  
  while (! displayConnectionDetails()) {
    delay(100);
  }

  echoServer.begin();

  Serial.println(F("Listening for connections..."));

}

void loop(void)
{
  Adafruit_CC3000_ClientRef client = echoServer.available();
  if (client) {
    if (client.available() > 0)
      {
        /* uint8_t ch = client.read(); */
        /* char mstr[ ] = "marbles"; */
        char mstr = (char)var;
        client.write(mstr);
      }
  }
  delay(1000);
  /* Serial.println(var); */
  /* var++; */
}

bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
    {
      Serial.println(F("Unable to retrieve the IP Address!\r\n"));
      return false;
    }
  else
    {
      Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
      Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
      Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
      Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
      Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
      Serial.println();
      return true;
    }
  }
