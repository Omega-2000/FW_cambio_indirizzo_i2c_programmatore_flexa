#include <Wire.h>
#include <SparkFunMLX90614.h>

#define LED_SUP   9  //  PH6    //  BLU
#define LED_INF   8  //  PH5    //  GIALLO
#define BUTTON    2  //  PH4
#define RESET    10   

byte oldAddress;
byte newAddress;
unsigned long t_button = 0;

void scan();
void change();


//  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


void scan() {
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      //  LED
      if (address == 90/*5A*/) {
        digitalWrite(LED_SUP, 1);
        digitalWrite(LED_INF, 0);

        oldAddress = 0x5A;
        newAddress = 0x5B;
      }

      if (address == 91/*5B*/) {
        digitalWrite(LED_SUP, 0);
        digitalWrite(LED_INF, 1);

        oldAddress = 0x5B;
        newAddress = 0x5A;
      }
      //

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  //delay(5000); // Wait 5 seconds for next scan

}


void change() {
  IRTherm therm; // Create an IRTherm object to interact with throughout

  therm.begin(oldAddress); // Try using the old address first
  
  byte address;
  if (!therm.readID()) // Try reading the ID registers
  {
    // If the read fails, try re-initializing with the
    // new address. Maybe we've already set it.
    therm.begin(newAddress);
    
    if (therm.readID()) // Read from the ID registers
    { // If the read succeeded, print the ID:
      Serial.println("Communicated with new address.");
      Serial.println("ID: 0x" + 
                    String(therm.getIDH(), HEX) +
                    String(therm.getIDL(), HEX));
    }
    else
    {
      Serial.println("Failed to communicate with either address.");      
    }
  }
  else
  {
    // If the read suceeds, change the address to something
    // new.
    if (!therm.setAddress(newAddress))
    {
      Serial.println("Failed to set new address.");
    }
    else
    {
      Serial.println("Set the address to 0x" + String(newAddress, HEX));
      Serial.println("Cycle power to try it out.");
    }
  }

  Serial.println("Restart MLX90614 to apply the change");
  therm.sleep();  // sleep() sets the MLX90614 into a low-power sleep mode.
  therm.wake(); // wake() should revive the MLX90614 from low-power sleep mode.

}


//  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


void setup() {
  Serial.begin(115200);

  Wire.begin();

  pinMode(LED_SUP, OUTPUT);
  pinMode(LED_INF, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.println("setup");
  Serial.println();
}

void loop() {
  scan();

  if (digitalRead(BUTTON) == 0) {
    if ((millis() - t_button) > 500) {
      t_button = millis();
      change();
    }
  }

  delay(10);
}

