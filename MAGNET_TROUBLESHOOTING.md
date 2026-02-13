# 🧲 Magnet Troubleshooting - No Reading Changes

## 🔍 Problem Diagnosis

Your TLE493D communicates correctly (detected on I2C) but values don't change when you approach the magnet? Here are possible causes and solutions.

---

## 🎯 Cause 1: Incorrect Magnet Orientation (MOST COMMON!)

### The Problem
The TLE493D measures magnetic fields in **3D** (X, Y, Z axes). Magnet orientation relative to the sensor is **CRITICAL**.

### How the Sensor Works

```
        Top view of TLE493D
        
    ┌─────────────────────────┐
    │                         │
    │         ┌───┐           │
    │         │ • │  ← Sensor │
    │         └───┘           │
    │           ↑ Z (perpendicular)
    │           │             │
    │      Y ←──┼──→ X        │
    │           │             │
    └─────────────────────────┘
    
    Z: Perpendicular to sensor (through chip)
    X: Parallel to long edge
    Y: Parallel to short edge
```

### ✅ Solution: Orient Magnet Correctly

**For slot car controller (angle measurement):**

The magnet must **ROTATE AROUND** the sensor, not just approach/move away!

```
Correct trigger configuration:

    Side view                  Top view
    
    ╔═══════╗                 ╔═══════╗
    ║       ║                 ║ [TLE] ║ ← Sensor
    ║ [TLE] ║ ← Sensor       ║       ║
    ║       ║                 ║   •   ║ ← Magnet position
    ╚═══════╝                 ╚═══════╝
        ↓                         ↓
        ↓                     Magnet rotates
    [  N  ]  ← Magnet        around sensor
    [  S  ]
    
    Magnet is parallel to PCB and ROTATES around the sensor!
```

**Simple test:**
1. Place magnet flat, parallel to sensor (about 5-10mm distance)
2. ROTATE the magnet around the sensor (360°)
3. Watch X and Y values in serial monitor
4. They should follow a sinusoidal cycle

---

## 🎯 Cause 2: Magnet Too Weak

### The Problem
TLE493D has measurement range of ±130 mT (milliTesla). If magnet is too weak, field won't be detected clearly.

### ✅ Solution: Use Appropriate Magnet

**Recommended magnets for slot car:**
- **Type**: Neodymium (NdFeB) - grade N35 or higher
- **Shape**: Disc or cylinder
- **Size**: 5-10mm diameter × 2-5mm thickness
- **Strength**: ~0.5-1 kg attraction force

**Magnets too weak (will NOT work well):**
- ❌ Fridge magnets (too weak)
- ❌ Standard ceramic magnets (too weak)
- ❌ Too small magnets (< 3mm)

### Magnet Strength Test
Your magnet should:
- ✅ Hold a light key suspended
- ✅ Stick firmly to a screwdriver
- ✅ Be felt through a 1.6mm PCB

---

## 🎯 Cause 3: Distance Too Large

### The Problem
Magnetic field decreases with square of distance. Too far = no signal.

### ✅ Solution: Optimal Distance

**For TLE493D-W2B6:**
- **Minimum distance**: 2-3mm (saturation risk if too close)
- **Optimal distance**: 5-10mm
- **Maximum distance**: 15-20mm (depends on magnet strength)

---

## 🎯 Cause 4: Sensor Reads But Code Doesn't Calculate Angle

### ✅ Solution: Check Raw Values

Add this debug code in `HAL.cpp`, function `HAL_ReadTriggerRaw()`:

```cpp
#elif defined (TLE493D_MAG)
  // ... existing code ...
  
  int16_t X = (int16_t)((buf[0] << 8) | ((buf[1] & 0x3F) << 2)) >> 2;
  int16_t Y = (int16_t)((buf[2] << 8) | ((buf[3] & 0x3F) << 2)) >> 2;
  
  // ⭐ ADD THESE LINES FOR DEBUG ⭐
  Serial.print("X: ");
  Serial.print(X);
  Serial.print(" | Y: ");
  Serial.print(Y);
  Serial.print(" | ");
  // ⭐ END DEBUG ⭐
#endif
```

**What you should see:**
- **X and Y change** when rotating magnet → Sensor works ✅
- **X and Y stay constant** (e.g., X=0, Y=0) → Reading problem ❌

---

## 🔬 Complete Test Program

Use this test code to diagnose the problem:

```cpp
// TLE493D Test - Put in loop()
void testTLE493D() {
  static unsigned long lastPrint = 0;
  
  if (millis() - lastPrint > 100) {  // Print every 100ms
    uint8_t buf[7];
    
    // Read sensor data
    Wire1.requestFrom(TLE493D_detected_address, 7);
    for (uint8_t i = 0; i < 7; i++) {
      if (Wire1.available()) {
        buf[i] = Wire1.read();
      }
    }
    
    // Extract X, Y, Z (12-bit signed)
    int16_t X = (int16_t)((buf[0] << 8) | ((buf[1] & 0x3F) << 2)) >> 2;
    int16_t Y = (int16_t)((buf[2] << 8) | ((buf[3] & 0x3F) << 2)) >> 2;
    int16_t Z = (int16_t)((buf[4] << 8) | ((buf[5] & 0x3F) << 2)) >> 2;
    
    // Calculate magnetic field magnitude
    float magnitude = sqrt(X*X + Y*Y + Z*Z);
    
    // Calculate angle
    float angleRad = atan2(Y, X);
    float angleDeg = angleRad * 180.0 / PI;
    if (angleDeg < 0) angleDeg += 360;
    
    // Display
    Serial.print("X:");
    Serial.print(X);
    Serial.print(" Y:");
    Serial.print(Y);
    Serial.print(" Z:");
    Serial.print(Z);
    Serial.print(" | Mag:");
    Serial.print(magnitude, 1);
    Serial.print(" | Angle:");
    Serial.print(angleDeg, 1);
    Serial.println("°");
    
    lastPrint = millis();
  }
}
```

**Result Interpretation:**

| Observation | Diagnosis | Solution |
|------------|-----------|----------|
| X, Y, Z = 0 or constant | Sensor not reading | Check I2C, power |
| Magnitude < 50 | Magnet too weak/far | Stronger magnet, move closer |
| Angle doesn't change | Wrong orientation | Rotate magnet |
| Magnitude > 2000 | Saturation | Move magnet farther |
| Erratic values | Interference | Isolate sensor |

---

## 📐 Optimal Mechanical Configuration

### For Slot Car Controller

```
Trigger side view:

         Finger
           ↓
    ╔═════════════╗
    ║   Trigger   ║
    ║             ║
    ║    [N S]    ║ ← Magnet mounted on trigger
    ╚══════╪══════╝
           │ Pivot
           ↓
    ┌──────────────┐
    │              │
    │    [TLE]     │ ← Sensor fixed on PCB
    │              │
    └──────────────┘
         PCB

Rest position: Magnet above sensor (N-S axis vertical)
Trigger pressed: Magnet rotates around sensor
```

### Recommended Mounting

1. **Sensor-magnet distance**: 5-10mm
2. **Magnet parallel to PCB**: Yes ✅
3. **N-S axis of magnet**: Perpendicular to movement
4. **Rotation radius**: As short as possible around sensor

---

## ✅ Verification Checklist

Check each point:

- [ ] Sensor detected on I2C (detection message OK)
- [ ] I2C pull-up resistors present (4.7kΩ)
- [ ] Magnet is neodymium N35 or higher
- [ ] Magnet at least 5mm diameter
- [ ] Sensor-magnet distance between 5-10mm
- [ ] Magnet parallel to PCB (flat)
- [ ] Magnet ROTATES around sensor (doesn't just approach)
- [ ] No motor/transformer nearby
- [ ] Debug code shows X, Y, Z changing
- [ ] Values don't saturate (< 2000)

---

## 🆘 If Nothing Works

Run the 3D magnetic field scan:

```cpp
void scanMagneticField() {
  Serial.println("\n=== 3D Magnetic Field Scan ===");
  
  for (int i = 0; i < 10; i++) {
    uint8_t buf[7];
    Wire1.requestFrom(TLE493D_detected_address, 7);
    for (uint8_t j = 0; j < 7; j++) {
      if (Wire1.available()) buf[j] = Wire1.read();
    }
    
    int16_t X = (int16_t)((buf[0] << 8) | ((buf[1] & 0x3F) << 2)) >> 2;
    int16_t Y = (int16_t)((buf[2] << 8) | ((buf[3] & 0x3F) << 2)) >> 2;
    int16_t Z = (int16_t)((buf[4] << 8) | ((buf[5] & 0x3F) << 2)) >> 2;
    
    Serial.print(i);
    Serial.print(": X=");
    Serial.print(X);
    Serial.print(" Y=");
    Serial.print(Y);
    Serial.print(" Z=");
    Serial.println(Z);
    
    delay(500);
  }
  
  Serial.println("Now rotate magnet slowly...");
  Serial.println("===================================\n");
}
```

**Expected results:**
- Without magnet: X, Y, Z close to 0 (±50)
- With static magnet: Constant non-zero values
- **Rotating magnet: X and Y oscillate in opposite phase** ← **THIS IS WHAT YOU SHOULD SEE**

---

**Created**: February 13, 2026  
**For**: ESPEED32 with TLE493D  
**Problem**: Magnet produces no significant reading change
