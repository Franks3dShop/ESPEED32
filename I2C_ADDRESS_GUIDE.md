# 📍 I2C Address Guide - TLE493D-W2B6 and P3B6

## ⚠️ IMPORTANT: 7-bit vs 8-bit Addressing

Arduino and most I2C libraries use **7-bit addressing**, but some datasheets show **8-bit addresses**. This often creates confusion!

### Converting 7-bit ↔ 8-bit

```
8-bit WRITE address = (7-bit address × 2) + 0
8-bit READ address  = (7-bit address × 2) + 1

Example for W2B6-A1:
7-bit: 0x22
8-bit WRITE: 0x22 × 2 = 0x44
8-bit READ:  0x22 × 2 + 1 = 0x45
```

---

## 🔍 TLE493D-W2B6: The 4 Variants

The **TLE493D-W2B6** sensor comes in **4 versions** identified as A0, A1, A2, or A3. Each has a unique I2C address.

### Complete Address Table

| Variant | Chip Marking | 7-bit Address (Arduino) | 8-bit WRITE | 8-bit READ | Availability |
|---------|--------------|-------------------------|-------------|------------|--------------|
| **W2B6-A0** | TLE493D-W2B6-A0 | `0x1F` | `0x3E` | `0x3F` | ⭐⭐⭐ Most common |
| **W2B6-A1** | TLE493D-W2B6-A1 | `0x22` | `0x44` | `0x45` | ⭐⭐⭐ Common |
| **W2B6-A2** | TLE493D-W2B6-A2 | `0x6E` | `0xDC` | `0xDD` | ⭐ Less common |
| **W2B6-A3** | TLE493D-W2B6-A3 | `0x44` | `0x88` | `0x89` | ⭐ Less common |

---

## 🎯 How to Identify My Variant?

### Method 1: Check chip marking
The sensor usually has an inscription:
- `TLE493D-W2B6-A0` → Variant A0 (address 0x1F)
- `TLE493D-W2B6-A1` → Variant A1 (address 0x22)
- etc.

### Method 2: I2C Bus Scanner

Use this simple code to discover the address:

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL for ESP32
  
  Serial.println("\n=== I2C Scanner ===");
  
  for(byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("Device found at 0x");
      if (addr < 16) Serial.print("0");
      Serial.print(addr, HEX);
      
      // Identify known TLE493D variants
      switch(addr) {
        case 0x1F:
          Serial.println(" → TLE493D-W2B6-A0");
          break;
        case 0x22:
          Serial.println(" → TLE493D-W2B6-A1 (8-bit: 0x44/0x45)");
          break;
        case 0x44:
          Serial.println(" → TLE493D-W2B6-A3");
          break;
        case 0x5D:
          Serial.println(" → TLE493D-P3B6-A0");
          break;
        case 0x6E:
          Serial.println(" → TLE493D-W2B6-A2");
          break;
        default:
          Serial.println(" → Unknown device");
          break;
      }
    }
  }
  
  Serial.println("===================");
}

void loop() {}
```

### Method 3: Use Modified ESPEED32 Code

The code auto-detects and displays which variant you have:

```
=== TLE493D Sensor Detection ===
Scanning for TLE493D-W2B6
  Scanning W2B6 addresses (A0/A1/A2/A3 variants)...
✓ Sensor found at address: 0x22 (W2B6-A1, 8-bit: 0x44/0x45)
✓ Sensor configured successfully
================================
```

---

## 🤔 Practical Cases - Resolving Confusion

### Case 1: "I have address 0x44 but don't know which variant"

**Two possibilities:**

#### If you see 0x44 in your **Arduino I2C scanner (7-bit)**:
→ You have a **TLE493D-W2B6-A3** (7-bit address = 0x44)

#### If you see 0x44 in a **datasheet as "write address"**:
→ You probably have a **TLE493D-W2B6-A1** (7-bit = 0x22, 8-bit write = 0x44)

**Simple solution:** Use the modified ESPEED32 code which auto-detects!

---

### Case 2: "My datasheet shows 0x3E but scanner finds 0x1F"

**This is normal!** 
- 0x3E = 8-bit WRITE address
- 0x1F = 7-bit address (Arduino)
- 0x3E ÷ 2 = 0x1F ✅

You have a **TLE493D-W2B6-A0**.

---

## 🔧 TLE493D-P3B6: The Variants

The **TLE493D-P3B6** (newer version) comes in 2 variants:

| Variant | 7-bit Address (Arduino) | 8-bit WRITE | 8-bit READ | Notes |
|---------|-------------------------|-------------|------------|-------|
| **P3B6-A0** | `0x5D` | `0xBA` | `0xBB` | Standard version |
| **P3B6-A1** | `0x35` | `0x6A` | `0x6B` | Alternative address |

---

## 📐 I2C Connection Schematic

```
ESP32                    TLE493D-W2B6
                         
GPIO21 (SDA) ----+------ SDA (pin 5)
                 |
              [4.7kΩ]
                 |
                3.3V
                
GPIO22 (SCL) ----+------ SCL (pin 6)
                 |
              [4.7kΩ]
                 |
                3.3V
                
3.3V --------------------  VDD (pin 1)
                
GND ---------------------  GND (pin 2,3,4)

Note: Pins may vary depending on package (PG-SSO-3, PG-TSOP-6, etc.)
Check your specific datasheet.
```

---

## 🎓 Using Multiple Sensors

To use **multiple TLE493D** simultaneously:

1. Choose variants with different addresses
2. Example: A0 (0x1F) + A1 (0x22) + A3 (0x44)
3. Connect all on same I2C bus
4. Address each individually in code

**Example code:**

```cpp
#define SENSOR1_ADDR 0x1F  // W2B6-A0
#define SENSOR2_ADDR 0x22  // W2B6-A1
#define SENSOR3_ADDR 0x44  // W2B6-A3

// Read sensor 1
Wire.beginTransmission(SENSOR1_ADDR);
// ... reading ...

// Read sensor 2
Wire.beginTransmission(SENSOR2_ADDR);
// ... reading ...
```

---

## 💡 Summary - Key Points

1. **Arduino uses 7-bit addresses**
   - If scanner finds 0x22, that's the correct address for Arduino
   - No conversion needed

2. **W2B6 has 4 variants: A0, A1, A2, A3**
   - Each has different address
   - Most common are A0 (0x1F) and A1 (0x22)

3. **Modified ESPEED32 code auto-detects**
   - Scans all 4 possible addresses
   - Tells you which variant you have
   - No manual configuration needed

4. **Address 0x44 = two possibilities**
   - W2B6-A3 in 7-bit addressing
   - OR W2B6-A1 in 8-bit addressing (real address = 0x22)
   - Arduino I2C scanner gives the answer

---

**Created**: February 13, 2026  
**Updated**: Support for 4 W2B6 variants (A0/A1/A2/A3)  
**Author**: Claude (Anthropic)
