/* #define DEBUG */

#include <Wire.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "utility/socket.h"
#include <stdlib.h>

#if defined DEBUG
#include "utility/debug.h"
#endif

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "myNetwork"           // cannot be longer than 32 characters!
#define WLAN_PASS       "myPassword"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define SEND_PORT           7    // What TCP port to listen on for connections.  The echo protocol uses port 7.

Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();
Adafruit_CC3000_Client client;

char out_buf[63];
char temp_buf[20];

int session_number;
unsigned long trial_num = 1;
unsigned long time_since_start;


void mystrcat(char a[],char b[]) {
  int c=0;
  int d=0;
  while (a[c]!= '\0') {
    c++;
  }

  while (b[d]!= '\0') {
    a[c]=b[d]; 
    c++;d++;
  }
  a[c]='\0';
}

void ctmp_insrt(char a[], const char b[]) {
  int c=0;
  
  while (b[c]!= '\0') {
    a[c]=b[c]; 
    c++;
  }
  a[c]='\0';
}

void send_packet(char out_buf[], char temp_buf[], char title[], float x, float y, float z, unsigned long trial_num, unsigned long time_since_start) {


  // buffer code is then: sensor type, x, y, z, trial number, timestamp
  
  // kill anything that was in buffers
  out_buf[0] = '\0';
  temp_buf[0] = '\0';

  ctmp_insrt(temp_buf, "");

  mystrcat(out_buf, title);
  mystrcat(out_buf, ",");
  
  add_to_buffer(out_buf, temp_buf, x);
  add_to_buffer(out_buf, temp_buf, y);
  add_to_buffer(out_buf, temp_buf, z);


  add_to_buffer(out_buf, temp_buf, (unsigned long)session_number);
  mystrcat(out_buf, ",");
  add_to_buffer(out_buf, temp_buf, trial_num);
  mystrcat(out_buf, ",");
  add_to_buffer(out_buf, temp_buf, time_since_start);

  client.fastrprint(out_buf);
  
#if defined DEBUG
  Serial.print("size of buffer: "); Serial.println(strlen(out_buf));
  Serial.println(out_buf);
#endif

}


void add_to_buffer(char buffer[], char temp_buf[], float a)
{
  ctmp_insrt(temp_buf, "");
  dtostrf(a, 4, 3, temp_buf);
  mystrcat(buffer, temp_buf);
  ctmp_insrt(temp_buf, ",");
  mystrcat(buffer, temp_buf);
}

void add_to_buffer(char buffer[], char temp_buf[], unsigned long a)
{
  ctmp_insrt(temp_buf, "");
  ltoa(a, temp_buf, 10);
  mystrcat(buffer, temp_buf);
}

void setup(void)
{

  // random numbers are probably fine for session IDs
  randomSeed(analogRead(0)); // reads noise off of pin 0
  session_number = random(1000);

#if defined DEBUG
  Serial.begin(9600);
  Serial.println(F("CC3000 good.")); 
#endif
  
  if (!cc3000.begin())
    {
#if defined DEBUG
      Serial.println(F("Couldn't begin CC3000."));
#endif
      while(1);
    }

  if (!lsm.begin())
    {
#if defined DEBUG
      Serial.println("Unable to initialize the LSM9DS0.");
#endif
    }

#if defined DEBUG
  Serial.println("LSM9DS0 9DOF good.");
  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
#endif

  while (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
#if defined DEBUG
    Serial.println(F("Failed!"));
#endif
    /* while(1); */
    delay(500);
  }

#if defined DEBUG
  Serial.println(F("Connected!"));
  Serial.println(F("Request DHCP"));
#endif
  while (!cc3000.checkDHCP())
    {
      delay(100); // ToDo: Insert a DHCP timeout!
    }  

  cc3000.begin();

  client = cc3000.connectUDP(cc3000.IP2U32(192, 168, 12, 1), 7400);

#if defined DEBUG
  Serial.println(displayConnectionDetails());
  Serial.println(F("Listening for connections..."));
#endif
  
}

void loop(void)
{

  sensors_event_t accel, mag, gyro, temp;
  lsm.getEvent(&accel, &mag, &gyro, &temp);

  
  delay(20);

#if defined DEBUG
  Serial.println("in loop");
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
#endif

  time_since_start = millis();
  send_packet(out_buf,
              temp_buf,
              "accel",
              accel.acceleration.x,
              accel.acceleration.y,
              accel.acceleration.z,
              trial_num,
              time_since_start);

  send_packet(out_buf,
              temp_buf,
              "gyro",
              gyro.gyro.x,
              gyro.gyro.y,
              gyro.gyro.z,
              trial_num,
              time_since_start);

  send_packet(out_buf,
              temp_buf,
              "mag",
              mag.magnetic.x,
              mag.magnetic.y,
              mag.magnetic.z,
              trial_num,
              time_since_start);

  trial_num++;


#if defined DEBUG
  Serial.print("size of buffer: "); Serial.println(strlen(out_buf));
  Serial.println(out_buf);
#endif

}

/**************************************************************************/
/*!
  @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
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
