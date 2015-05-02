#include <Wire.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "utility/debug.h"
#include "utility/socket.h"

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

Adafruit_CC3000_Server echoServer(LISTEN_PORT);
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();


void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}

  
void setup(void)
{

  Serial.begin(115200);
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

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  echoServer.begin();
  
  Serial.println(F("Listening for connections..."));
}

void loop(void)
{
  sensors_event_t accel, mag, gyro, temp;
  lsm.getEvent(&accel, &mag, &gyro, &temp);
  /* lsm.read(); */
  /* int xaccel = lsm.accelData.x; */
  /* Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" "); */
  /* Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" "); */
  /* Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" "); */
  /* Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" "); */
  /* Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" "); */
  /* Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" "); */
  /* Serial.print("Gyro X: "); Serial.print((int)lsm.gyroData.x);   Serial.print(" "); */
  /* Serial.print("Y: "); Serial.print((int)lsm.gyroData.y);        Serial.print(" "); */
  /* Serial.print("Z: "); Serial.println((int)lsm.gyroData.z);      Serial.println(" "); */
  /* Serial.print("Temp: "); Serial.print((int)lsm.temperature);    Serial.println(" "); */
  delay(200);

  /* data = char[]  */
 
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = echoServer.available();

  if (client) {
    /* char data[ 9 ]; */
    /*   data[0] = accel.acceleration.x; */
    /*   data[1] = accel.acceleration.y; */
    /*   data[2] = accel.acceleration.z; */

    /*   data[3] = gyro.gyro.x; */
    /*   data[4] = gyro.gyro.y; */
    /*   data[5] = gyro.gyro.z; */

    /* /\* client.write('\'gyro\':'); *\/ */

    /*   data[6] = mag.magnetic.x; */
    /*   data[7] = mag.magnetic.y; */
    /*   data[8] = mag.magnetic.z; */        

    /*   client.write(data, sizeof(data)); */
 
    // Check if there is data available to read.
    /* if (client.available() > 0) { */
    // Read a byte and write it to all clients.
    /* uint8_t ch = client.read(); */
 
    /* } */

    /* client.write('-----------------------'); */
    /* client.write(accel.acceleration.x);  */
    /* client.write(accel.acceleration.y);  */
    /* client.write(accel.acceleration.z);  */
    /* client.write(gyro.gyro.x);  */
    /* client.write(gyro.gyro.y);  */
    /* client.write(gyro.gyro.z);  */
    /* client.write(mag.magnetic.x);  */
    /* client.write(mag.magnetic.y);  */
    /* client.write(mag.magnetic.z);  */
    char buf[10];
    sprintf(buf, "time%d%d", accel.acceleration.x, accel.acceleration.y);
    client.println(buf);
    /* Serial.println('hello'); */
    
  }
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
