/******************************************* 


        OWX Emulator Master utils Example 
    
        Demonstrates a OneWire master device that communicates with a OWX slave.
        It supports reading and writing various data types (int, float, bool, char, string) via Serial input.
        The master periodically requests updates from the slave and reads its scratchpad memory.

    tested on: ESP8266 (NodeMCU, Wemos D1 Mini Pro)
    by qqeOSAS(2025)
 
    
****************************************************

*/





#include <Arduino.h>
#include <OneWire.h>
#include "OW_emulator_master_utils.h"

/* ===================== CONFIG ===================== */
// OneWire bus pin
#define ONEWIRE_PIN D2

// Scratchpad polling interval (ms)
#define SCRATCHPAD_POLL_MS 2000

OneWire ow(ONEWIRE_PIN);

/* ===================== SLAVE ===================== */
// Test slave ROM address (optional, not used when ow.skip() is used)
uint8_t slaveROM[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

/* ===================== DATA TYPES ===================== */
// Supported input data types from Serial
enum ValueType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_UNKNOWN
};

/* ===================== TYPE DETECTION ===================== */
// Detect input type from Serial string
ValueType detectType(const String &s) {
    String str = s;
    str.trim();

    if (str.length() == 0) return TYPE_UNKNOWN;

    // Boolean detection
    if (str.equalsIgnoreCase("true") || str.equalsIgnoreCase("false"))
        return TYPE_BOOL;

    bool hasDot = false;
    bool isNumeric = true;
    uint8_t start = (str.charAt(0) == '-') ? 1 : 0;

    // Numeric validation
    for (uint8_t i = start; i < str.length(); i++) {
        char c = str.charAt(i);
        if (c == '.') {
            hasDot = true;
            continue;
        }
        if (!isDigit(c)) {
            isNumeric = false;
            break;
        }
    }

    if (isNumeric && hasDot)  return TYPE_FLOAT;
    if (isNumeric)            return TYPE_INT;
    if (str.length() == 1)    return TYPE_CHAR;

    return TYPE_STRING;
}

/* ===================== SERIAL COMMAND HANDLER ===================== */
// Read Serial input and send detected value to OneWire slave
void handleSerialInput() {
    if (!Serial.available()) return;

    String input = Serial.readStringUntil('\n');
    input.trim();

    ValueType type = detectType(input);

    switch (type) {

        case TYPE_INT: {
            int32_t val = input.toInt();
            Serial.print(F("[INT] "));
            Serial.println(val);
            ow_write_int32(ow, val);
            break;
        }

        case TYPE_FLOAT: {
            float val = input.toFloat();
            Serial.print(F("[FLOAT] "));
            Serial.println(val, 4);
            ow_write_float32(ow, val);
            break;
        }

        case TYPE_BOOL: {
            bool val = input.equalsIgnoreCase("true");
            Serial.print(F("[BOOL] "));
            Serial.println(val ? "true" : "false");
            // Can be sent as uint8 if required
            break;
        }

        case TYPE_CHAR: {
            char val = input.charAt(0);
            Serial.print(F("[CHAR] "));
            Serial.println(val);
            ow_write_char8(ow, val);
            break;
        }

        case TYPE_STRING:
            Serial.print(F("[STRING] "));
            Serial.println(input);
            break;

        default:
            Serial.println(F("[ERROR] Unknown input"));
            break;
    }
}

/* ===================== SLAVE COMMUNICATION ===================== */
// Request data update from slave device
void requestSlaveUpdate() {
    ow_write_handler_command(ow, OW_REQUEST_UPDATE);
}

// Read scratchpad and parse known data layout
void readAndParseScratchpad() {
    uint8_t* scratchpad = readScratchpad(ow);
    if (!scratchpad) return;

    uint8_t  u8;
    uint16_t u16;
    float    f32;

    // Parse scratchpad data using fixed offsets
    parse_uint8_scratchpad (ow, u8,  scratchpad, 0);
    parse_uint16_scratchpad(ow, u16, scratchpad, 1);
    parse_float32_scratchpad(ow, f32, scratchpad, 3);

    Serial.println(F("---- SCRATCHPAD ----"));
    Serial.print(F("uint8  : ")); Serial.println(u8);
    Serial.print(F("uint16 : ")); Serial.println(u16);
    Serial.print(F("float  : ")); Serial.println(f32, 4);
}

/* ===================== SETUP ===================== */
void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println(F("OW Master started"));
}

/* ===================== LOOP ===================== */
void loop() {
    static unsigned long lastPoll = 0;
    unsigned long now = millis();

    // Periodic scratchpad polling
    if (now - lastPoll >= SCRATCHPAD_POLL_MS) {
        lastPoll = now;
        requestSlaveUpdate();
        readAndParseScratchpad();
    }

    // Handle user input from Serial
    handleSerialInput();
}
