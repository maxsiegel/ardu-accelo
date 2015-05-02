#include "utility/debug.h"

#include <Wire.h>
#include <Adafruit_LSM9DS0.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

/* #include "utility/socket.h" */
#include <stdlib.h>


#define LISTEN_PORT           7    // What TCP port to listen on for connections.  The echo protocol uses port 7.

/* Adafruit_CC3000_Server echoServer(LISTEN_PORT); */
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();

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
  Serial.println("--------------------------------------------------------------------------------");
  Serial.println("--------------------------------------------------------------------------------");
  Serial.println("--------------------------------------------------------------------------------");

  if (!lsm.begin())
    {
      Serial.println("Unable to initialize the LSM9DS0.");
    }
  Serial.println("Found LSM9DS0 9DOF");
  
  Serial.println("Connected!");
  Serial.println("hello");

}

int t = 1;

char out_buf[100]; // = "\0";
char tempbuf[100];// = "\0";
char time[100];// = "\0";


void loop(void)
{

  sensors_event_t accel, mag, gyro, temp;
  lsm.getEvent(&accel, &mag, &gyro, &temp);
  /* lsm.read(); */

  out_buf[0] = '\0';
  tempbuf[0] = '\0';
  time[0] = '\0';
  /* memset(&out_buf[0], 0, sizeof(out_buf)); */
  /* memset(&tempbuf[0], 0, sizeof(tempbuf)); */
  /* memset(&time[0], 0, sizeof(time)); */
  /* char out_buf[100]; // = "\0"; */
  /* char tempbuf[100];// = "\0"; */
  /* char time[100];// = "\0"; */

  delay(20);

  /* data = char[]  */
 
  // Try to get a client which is connected.

  
  Serial.print("iteration number: "); Serial.println(t);

  t++;
  
  /* Adafruit_CC3000_ClientRef client = echoServer.available(); */
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  
  /* if (client) { */
  /* Serial.println('in client loop'); */

  int count = 0;
  snprintf(time, 8, "%d", millis());

  Serial.print("time: "); Serial.println(time);

  mystrcat(out_buf, time);
  ctmp_insrt(tempbuf, ",");
  mystrcat(out_buf, tempbuf);

  /* Serial.println("_____ accel.acceleration"); */

  add_float_to_buffer(out_buf, tempbuf, accel.acceleration.x);
  add_float_to_buffer(out_buf, tempbuf, accel.acceleration.y);
  add_float_to_buffer(out_buf, tempbuf, accel.acceleration.z);
  
  /* Serial.println("_____ gyro.gyro"); */

  add_float_to_buffer(out_buf, tempbuf, gyro.gyro.x);
  add_float_to_buffer(out_buf, tempbuf, gyro.gyro.y);
  add_float_to_buffer(out_buf, tempbuf, gyro.gyro.z);

  /* Serial.println("_____ mag.magnetic"); */
  add_float_to_buffer(out_buf, tempbuf, mag.magnetic.x);
  add_float_to_buffer(out_buf, tempbuf, mag.magnetic.y);
  add_float_to_buffer(out_buf, tempbuf, mag.magnetic.z);

  Serial.println("_____ length of buffer");
  Serial.print("length of buffer: ");
  Serial.println(strlen(out_buf));
  Serial.println("end of loop");
  Serial.print("buffer: "); Serial.println(out_buf);
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}

/**************************************************************************/
/*!
  @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
/* bool displayConnectionDetails(void) */
/* { */
/*   uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv; */
  
/*   if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) */
/*     { */
/*       Serial.println(F("Unable to retrieve the IP Address!\r\n")); */
/*       return false; */
/*     } */
/*   else */
/*     { */
/*       Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress); */
/*       Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask); */
/*       Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway); */
/*       Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv); */
/*       Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv); */
/*       Serial.println(); */
/*       return true; */
/*     } */
/* } */
