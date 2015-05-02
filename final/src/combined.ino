#include <Wire.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "utility/debug.h"
#include "utility/socket.h"
#include <stdlib.h>

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

#define LISTEN_PORT           7    // What TCP port to listen on for connections.  The echo protocol uses port 7.

Adafruit_CC3000_Server wifiServer(LISTEN_PORT);
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();


/* void setupSensor() */
/* { */
/*   // 1.) Set the accelerometer range */
/*   lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G); */
/*   //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G); */
/*   //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G); */
/*   //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G); */
/*   //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G); */
  
/*   // 2.) Set the magnetometer sensitivity */
/*   lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS); */
/*   //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS); */
/*   //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS); */
/*   //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS); */

/*   // 3.) Setup the gyroscope */
/*   lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS); */
/*   //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS); */
/*   //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS); */
/* } */

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

void add_float_to_buffer(char buffer[], char tempbuf[], float a)
{
    
  ctmp_insrt(tempbuf, "");
  dtostrf(a, 4, 3, tempbuf);
  mystrcat(buffer, tempbuf);
  ctmp_insrt(tempbuf, ",");
  mystrcat(buffer, tempbuf);
}

void setup(void)
{

  Serial.begin(9600);
  Serial.println(F("Hello, CC3000!\n")); 

  /* Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC); */
  
  /* Initialise the module */
  /* Serial.println(F("\nInitializing...")); */
  if (!cc3000.begin())
    {
      Serial.println(F("Couldn't begin CC3000."));
      while(1);
    }

  if (!lsm.begin())
    {
      Serial.println("Unable to initialize the LSM9DS0.");
    }
  Serial.println("Found LSM9DS0 9DOF");
  
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

  // Display the IP address DNS, Gateway, etc.  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  wifiServer.begin();

  Serial.println(F("Listening for connections..."));
}

char out_buf[100];
char tempbuf[100];
char time_since_start[100];

void loop(void)
{
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = wifiServer.available();
  /* if (client) { */
  /*    // Check if there is data available to read. */
  /*    if (client.available() > 0) { */
  /*      // Read a byte and write it to all clients. */
  /*      uint8_t ch = client.read(); */
  /*      client.write(ch); */
  /*    } */
  /* } */
  /* sensors_event_t accel, mag, gyro, temp; */
    out_buf[0] = '\0';
  tempbuf[0] = '\0';
  time_since_start[0] = '\0';
  Serial.println(getFreeRam());
  Serial.println("in loop");
  // Try to get a client which is connected.
  /* Adafruit_CC3000_ClientRef client = wifiServer.available(); */

  if (client) {
    client.println("hello");
  }
  
  delay(100);

}

/* void loop(void) { */
/*   Serial.println("test"); */
/*   sensors_event_t accel, mag, gyro, temp; */
/*   /\* lsm.getEvent(&accel, &mag, &gyro, &temp); *\/ */

/*   // kill anything that was in buffers */
/*   out_buf[0] = '\0'; */
/*   tempbuf[0] = '\0'; */
/*   time_since_start[0] = '\0'; */
/*   Serial.println(getFreeRam()); */
/*   Serial.println("in loop"); */
/*   // Try to get a client which is connected. */
/*   Adafruit_CC3000_ClientRef client = wifiServer.available(); */

/*   if (client) { */
/*     client.println("hello"); */
/*   } */
  
/*   delay(100); */
/* } */

/* void loopr(void) */
/* { */

/*   sensors_event_t accel, mag, gyro, temp; */
/*   lsm.getEvent(&accel, &mag, &gyro, &temp); */

/*   // kill anything that was in buffers */
/*   out_buf[0] = '\0'; */
/*   tempbuf[0] = '\0'; */
/*   time_since_start[0] = '\0'; */

/*   delay(1000); */
  
/*   Serial.println("in loop"); */
/*   // Try to get a client which is connected. */
/*   Adafruit_CC3000_ClientRef client = wifiServer.available(); */

/*   delay(1000); */
  
/*   Serial.println("about to enter client conditional"); */

/*   if (client) { */
    
/*     snprintf(time_since_start, 8, "%d", millis()); */
/*     mystrcat(out_buf, time_since_start); */
/*     ctmp_insrt(tempbuf, ","); */
/*     mystrcat(out_buf, tempbuf); */
/*     Serial.print("time_since_start: "); Serial.println(time_since_start); */

    
/*     add_float_to_buffer(out_buf, tempbuf, accel.acceleration.x); */
/*     add_float_to_buffer(out_buf, tempbuf, accel.acceleration.y); */
/*     add_float_to_buffer(out_buf, tempbuf, accel.acceleration.z); */
  
/*     add_float_to_buffer(out_buf, tempbuf, gyro.gyro.x); */
/*     add_float_to_buffer(out_buf, tempbuf, gyro.gyro.y); */
/*     add_float_to_buffer(out_buf, tempbuf, gyro.gyro.z); */

/*     add_float_to_buffer(out_buf, tempbuf, mag.magnetic.x); */
/*     add_float_to_buffer(out_buf, tempbuf, mag.magnetic.y); */
/*     add_float_to_buffer(out_buf, tempbuf, mag.magnetic.z); */

/*     client.println(out_buf); */

/*   } */

/*   delay(200); */
/* } */

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
