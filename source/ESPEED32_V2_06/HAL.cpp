/*********************************************************************************************************************/
/*------------------------------------------------------Includes-----------------------------------------------------*/
/*********************************************************************************************************************/
#include "HAL.h"
#include "slot_ESC.h"
#include <math.h>

#ifdef AS5600_MAG
  #include "AS5600.h"
  AS5600 as5600;   // define magnnetic sensor, use default Wire

#elif defined (AS5600L_MAG)
  #include "AS5600L.h"
  AS5600L as5600;// define magnnetic sensor,use default Wire the 5600L has differnt address

#elif defined (MT6701_MAG)
  #include "MT6701.hpp"
  MT6701 mt6701; // magnetic sensor, install MT6701 library by Noran Raskin

#elif defined (TLE493D_MAG)
  #include <Wire.h>                 // default I²C library
  
  /* TLE493D Sensor I2C Address Configuration */
  /* The TLE493D family has different variants with different I2C addresses */
  
  #if defined(TLE493D_W2B6)
    // TLE493D-W2B6 I2C addresses (7-bit addressing)
    // The W2B6 comes in 4 variants: A0, A1, A2, A3 with different addresses
    // A0: 0x1F (default), A1: 0x22, A2: 0x6E, A3: 0x44
    // Note: 8-bit addresses shown in some datasheets are 7-bit << 1
    // Example: 0x44 (8-bit write) = 0x22 (7-bit) for version A1
    #define TLE493D_ADDRESS_A0  0x1F    // W2B6-A0 (most common)
    #define TLE493D_ADDRESS_A1  0x22    // W2B6-A1 (0x44 in 8-bit notation)
    #define TLE493D_ADDRESS_A2  0x6E    // W2B6-A2
    #define TLE493D_ADDRESS_A3  0x44    // W2B6-A3
    #define SENSOR_NAME "TLE493D-W2B6"
    
    // Array of all possible addresses to scan
    const uint8_t TLE493D_W2B6_addresses[] = {TLE493D_ADDRESS_A0, TLE493D_ADDRESS_A1, 
                                               TLE493D_ADDRESS_A2, TLE493D_ADDRESS_A3};
    
  #elif defined(TLE493D_P3B6)
    // TLE493D-P3B6 I2C addresses (7-bit addressing)
    // The P3B6 comes in 4 variants: A0, A1, A2, A3 with different addresses
    // Based on Infineon TLE493D family addressing scheme
    #define TLE493D_ADDRESS_A0  0x35    // P3B6-A0 (most common)
    #define TLE493D_ADDRESS_A1  0x22    // P3B6-A1 (shared with W2B6-A1)
    #define TLE493D_ADDRESS_A2  0x78    // P3B6-A2
    #define TLE493D_ADDRESS_A3  0x5D    // P3B6-A3
    #define SENSOR_NAME "TLE493D-P3B6"
    
    // Array of all possible addresses to scan
    const uint8_t TLE493D_P3B6_addresses[] = {TLE493D_ADDRESS_A0, TLE493D_ADDRESS_A1,
                                               TLE493D_ADDRESS_A2, TLE493D_ADDRESS_A3};
    
  #else
    // Default to W2B6 if neither is defined (as per schematic)
    #define TLE493D_PRIMARY_ADDRESS   0x1F
    #define TLE493D_SECONDARY_ADDRESS 0x5E
    #define SENSOR_NAME "TLE493D-W2B6 (default)"
    #warning "No TLE493D variant defined, defaulting to W2B6. Define TLE493D_W2B6 or TLE493D_P3B6 in HAL.h"
  #endif
  
  // Global variable to store the detected I2C address
  uint8_t TLE493D_detected_address = 0;

#endif

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

void HAL_InitHW()
{
  /* Setup fo the parameters for serial(debug) communication */ 
  Serial.begin(115200);   // debug restore me
  delay(2000);            // ⭐ Attendre 2 secondes
  Serial.println("\n=== ESPEED32 STARTED ===\n");
  Serial.println("Debug Start");

  #ifdef TLE493D_MAG
    Wire1.begin(SDA0_PIN, SCL0_PIN, 1000000L); // Initialize I2C for magnetic sensor
    
    // Auto-detect which TLE493D variant is connected
    bool sensor_found = false;
    
    Serial.println("=== TLE493D Sensor Detection ===");
    Serial.print("Scanning for ");
    Serial.println(SENSOR_NAME);
    
    #if defined(TLE493D_W2B6)
      // W2B6: Scan all 4 possible addresses (A0, A1, A2, A3 variants)
      Serial.println("  Scanning W2B6 addresses (A0/A1/A2/A3 variants)...");
      for (uint8_t i = 0; i < 4 && !sensor_found; i++) {
        uint8_t addr = TLE493D_W2B6_addresses[i];
        Wire1.beginTransmission(addr);
        if (Wire1.endTransmission() == 0) {
          TLE493D_detected_address = addr;
          sensor_found = true;
          Serial.print("✓ Sensor found at address: 0x");
          Serial.print(addr, HEX);
          Serial.print(" (");
          switch(addr) {
            case 0x1F: Serial.print("W2B6-A0"); break;
            case 0x22: Serial.print("W2B6-A1, 8-bit: 0x44/0x45"); break;
            case 0x6E: Serial.print("W2B6-A2"); break;
            case 0x44: Serial.print("W2B6-A3"); break;
            default: Serial.print("Unknown variant"); break;
          }
          Serial.println(")");
        }
      }
      
      if (!sensor_found) {
        Serial.println("✗ ERROR: TLE493D-W2B6 sensor not detected!");
        Serial.println("  Tried addresses: 0x1F (A0), 0x22 (A1), 0x6E (A2), 0x44 (A3)");
        Serial.println("  Note: 8-bit addresses are 2x 7-bit (e.g., 0x44 write = 0x22 in 7-bit)");
        Serial.println("  Please check:");
        Serial.println("  1. Sensor is properly connected");
        Serial.println("  2. I2C pullup resistors are present (4.7kΩ recommended)");
        Serial.println("  3. Correct sensor variant defined in HAL.h");
        Serial.println("  4. Check if you have W2B6-A0, A1, A2, or A3 variant");
        // Use A0 address as fallback
        TLE493D_detected_address = TLE493D_ADDRESS_A0;
      }
      
    #elif defined(TLE493D_P3B6)
      // P3B6: Scan all 4 possible addresses (A0, A1, A2, A3 variants)
      Serial.println("  Scanning P3B6 addresses (A0/A1/A2/A3 variants)...");
      for (uint8_t i = 0; i < 4 && !sensor_found; i++) {
        uint8_t addr = TLE493D_P3B6_addresses[i];
        Wire1.beginTransmission(addr);
        if (Wire1.endTransmission() == 0) {
          TLE493D_detected_address = addr;
          sensor_found = true;
          Serial.print("✓ Sensor found at address: 0x");
          Serial.print(addr, HEX);
          Serial.print(" (");
          switch(addr) {
            case 0x35: Serial.print("P3B6-A0"); break;
            case 0x22: Serial.print("P3B6-A1, 8-bit: 0x44/0x45"); break;
            case 0x78: Serial.print("P3B6-A2, 8-bit: 0xF0/0xF1"); break;
            case 0x5D: Serial.print("P3B6-A3, 8-bit: 0xBA/0xBB"); break;
            default: Serial.print("Unknown variant"); break;
          }
          Serial.println(")");
        }
      }
      
      if (!sensor_found) {
        Serial.println("✗ ERROR: TLE493D-P3B6 sensor not detected!");
        Serial.println("  Tried addresses: 0x35 (A0), 0x22 (A1), 0x78 (A2), 0x5D (A3)");
        Serial.println("  Note: 8-bit addresses are 2x 7-bit (e.g., 0x44 write = 0x22 in 7-bit)");
        Serial.println("  Please check:");
        Serial.println("  1. Sensor is properly connected");
        Serial.println("  2. I2C pullup resistors are present (4.7kΩ recommended)");
        Serial.println("  3. Correct sensor variant defined in HAL.h");
        Serial.println("  4. Check if you have P3B6-A0, A1, A2, or A3 variant");
        // Use A0 address as fallback
        TLE493D_detected_address = TLE493D_ADDRESS_A0;
      }
    #endif
    
    // Configure the sensor
    // Register 0x0A: MOD1 register - sets operation mode
    // 0xC6: Master Controlled Mode, Fast Mode, Low Power mode disabled
    // Register 0x0B: MOD2 register
    // 0x02: Temperature measurement disabled (can be enabled with 0x00)
    Wire1.beginTransmission(TLE493D_detected_address);
    Wire1.write(0x0A);      // Register address MOD1
    Wire1.write(0xC6);      // MOD1: Master controlled, Fast mode
    Wire1.write(0x02);      // MOD2: Temp disabled
    int config_result = Wire1.endTransmission();
    
    if (config_result == 0) {
      Serial.println("✓ Sensor configured successfully");
    } else {
      Serial.print("✗ Sensor configuration failed with error: ");
      Serial.println(config_result);
    }
    
    Serial.println("================================");
    Serial.println();
    
    delay(10); // Give sensor time to configure
  #endif

  /* configure motor control PWM functionalitites and attach the channel to the GPIO to be controlled */
  ledcAttachChannel(HB_IN_PIN, PWM_FREQ_DEFAULT*1000, THR_PWM_RES_BIT, THR_IN_PWM_CHAN);
  ledcAttachChannel(HB_INH_PIN, PWM_FREQ_DEFAULT*1000, THR_PWM_RES_BIT, THR_INH_PWM_CHAN);

/* LEDC Chan to Group/Channel/Timer Mapping
** ledc: 0  => Group: 0, Channel: 0, Timer: 0
** ledc: 1  => Group: 0, Channel: 1, Timer: 0
** ledc: 2  => Group: 0, Channel: 2, Timer: 1
** ledc: 3  => Group: 0, Channel: 3, Timer: 1
** ledc: 4  => Group: 0, Channel: 4, Timer: 2
** ledc: 5  => Group: 0, Channel: 5, Timer: 2
** ledc: 6  => Group: 0, Channel: 6, Timer: 3
** ledc: 7  => Group: 0, Channel: 7, Timer: 3
** ledc: 8  => Group: 1, Channel: 0, Timer: 0
** ledc: 9  => Group: 1, Channel: 1, Timer: 0
** ledc: 10 => Group: 1, Channel: 2, Timer: 1
** ledc: 11 => Group: 1, Channel: 3, Timer: 1
** ledc: 12 => Group: 1, Channel: 4, Timer: 2
** ledc: 13 => Group: 1, Channel: 5, Timer: 2
** ledc: 14 => Group: 1, Channel: 6, Timer: 3
** ledc: 15 => Group: 1, Channel: 7, Timer: 3
*/
}

void HALanalogWrite (const int PWMchan, int value)
{
  /* Adapted to new 3.0.0 ESP32 library --> ledcWrite takes input PIN, not CHANNEL */
  switch (PWMchan)
  {
    case THR_IN_PWM_CHAN:
      ledcWrite(HB_IN_PIN, (uint32_t)value);
      break;

    case THR_INH_PWM_CHAN:
      ledcWrite(HB_INH_PIN, (uint32_t)value);
      break;
    
    default:
      break;
  }
}

int16_t HAL_ReadTriggerRaw()
{
  uint16_t retVal = 0;

  #if defined (AS5600_MAG) || defined (AS5600L)
    retVal = as5600.readAngle();

  #elif defined (MT6701_MAG)
    retVal = mt6701.getAngleDegrees();

  #elif defined (ANALOG_TRIG)
    retVal = analogRead(AN_THROT_PIN);  // keep an analog pin aslso as backup, if I2C magnetic is not going

  #elif defined (TLE493D_MAG)
    int16_t angle10degXY = -1;  // angle in tenth of degree
    int16_t angle10degYZ = -1;  // angle in tenth of degree
    int16_t zSign, xSign;
    uint8_t buf[7];

    // Request 4 bytes from sensor (X, Y data registers)
    Wire1.requestFrom(TLE493D_detected_address, 4);

    // Read the 4 bytes
    for (uint8_t i = 0; i < 4; i++) {
      if (Wire1.available()) {
        buf[i] = Wire1.read();
      } else {
        buf[i] = 0; // Handle case where not enough bytes are available
      }
    }

    // Build 14-bit signed data for X and Y
    // TLE493D data format: 
    // Byte 0: X[11:4]
    // Byte 1: X[3:0] in bits [7:4], other data in bits [3:0]
    // Byte 2: Y[11:4]
    // Byte 3: Y[3:0] in bits [7:4], other data in bits [3:0]
    
    int16_t X = (int16_t)((buf[0] << 8) | ((buf[1] & 0x3F) << 2)) >> 2;
    int16_t Y = (int16_t)((buf[2] << 8) | ((buf[3] & 0x3F) << 2)) >> 2;
    
    // Calculate angle from X and Y components
    xSign = X < 0 ? -1 : 1;
    angle10degXY = 570 * (atan2(Y * xSign, X) + 1);  // Convert to tenth of degrees (0-3600)
    retVal = angle10degXY;
  #endif

  return retVal;
}


void HAL_PinSetup()
{
  pinMode(BUZZ_PIN, OUTPUT);     // Set BUZZ_PIN pin as an output
  pinMode(LED_BUILTIN, OUTPUT);  // Set ESP32 LED builtin pin as an output
  pinMode(BUTT_PIN, INPUT_PULLUP);// Set input pushbutton as input with pullup, so oyu con't need external resistor
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);// Set input pushbutton as input with pullup, so oyu con't need external resistor
  
}

/*
  readVoltageDivider: read the voltage applied to an ADC by a voltage divider
  @param:AnalogInput , at which ADC input is connected the voltage divider
  @param:rvfbl low side resistor on the voltage divider
  @param:rvfbh high side resistor on the voltage divider
  @returns: voltage applied to the voltage divider [mV]
*/
uint16_t HAL_ReadVoltageDivider(int AnalogInput, uint32_t rvfbl, uint32_t rvfbh)
{
  uint32_t sum = 0;                   /* Sum of samples taken */
  unsigned char sample_count = 0;   /* Current sample number */
  uint32_t voltage = 0;               /* Calculated voltage */
  uint32_t ADCraw;

  /* Calculate the voltage, refVolt is the calibrated reference voltage in [V] */
  ADCraw = analogRead(AnalogInput) ; // 1023 
  voltage = (ACD_VOLTAGE_RANGE_MVOLTS * ADCraw) / ACD_RESOLUTION_STEPS;// voltage at teh ADC pin
  voltage = (voltage * (rvfbl + rvfbh)) / rvfbl; // voltage appied to the  voltage divider
  
  return voltage;
}

void sound(note_t note, int ms)
{
  ledcAttachChannel(BUZZ_PIN, 5000, 8, BUZZ_CHAN);
  ledcWriteNote(BUZZ_PIN, note, 7);
  delay (ms);
  ledcDetach(BUZZ_PIN);
}


// function onSound: generates "enabling" sound withthe buzzer. E.G. use when the device wakes up
void offSound()
{ 
  sound(NOTE_E, 60);
  delay(60);// pause between each sound
  sound(NOTE_C, 60);  
}


// function offSound: generates "disabling" sound. E.G. use when battery is detached
void onSound()
{ 
  sound(NOTE_C, 30);
  sound(NOTE_E, 30);
}


// function offSound: generates "disabling" sound. E.G. use when battery is detached
void calibSound()
{ 
  sound(NOTE_C, 60);
  delay(60);// pause between each sound
  sound(NOTE_G, 60);  
  delay(60);// pause between each sound
  sound(NOTE_A, 60);  
}

// function keySound: generates key pressed sound
void keySound()
{ 
  sound(NOTE_D, KEY_SOUND_MS);
}
