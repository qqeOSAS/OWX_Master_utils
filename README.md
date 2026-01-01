# OWX Master Utils

[![PlatformIO](https://img.shields.io/badge/PlatformIO-compatible-green)](https://platformio.org/)


---------------

      _____        ____  __    __  __           _              _   _ _   _ _     
     / _ \ \      / /\ \/ /   |  \/  | __ _ ___| |_ ___ _ __  | | | | |_(_) |___ 
    | | | \ \ /\ / /  \  /    | |\/| |/ _` / __| __/ _ \ '__| | | | | __| | / __|
    | |_| |\ V  V /   /  \    | |  | | (_| \__ \ ||  __/ |    | |_| | |_| | \__ \
     \___/  \_/\_/   /_/\_\___|_|  |_|\__,_|___/\__\___|_|     \___/ \__|_|_|___/
                         |_____|                                                 
                      





 Helper library for building `OWX systems` on master side.


Author: **qqeOSAS**  

---

## Features

- Packet-based data transmission over OneWire
- Command-based protocol with payload length and CRC8
- Dallas / Maxim CRC8 implementation
- Typed data sending **from MASTER to SLAVE**
- Scratchpad read & parse helpers
- Multi-slave ready (ROM select or SKIP ROM)

---

## Supported Data Types (Master → Slave)

The following data types are supported:

- `int8_t`
- `int16_t`
- `uint16_t`
- `int32_t`
- `uint32_t`
- `float` (IEEE754, 32-bit)
- `char`
- Custom structures (raw byte arrays)

---

OWX_slave_emulator Library (important)
-------------------------------------------------
When developing a project that uses the OWX_master_utils on the master side, You have to use slave-side library — OWX_Slave_Emulator.
link to [OWX_Slave_Emulator](https://github.com/qqeOSAS/One_Wire_EXTENDED_Slave_Emulator)


## Dependencies

- Arduino framework
- OneWire library  
  (`paulstoffregen/OneWire`)

---

## Installation

### PlatformIO

Add to `platformio.ini`:

```ini
lib_deps =
    paulstoffregen/OneWire
    https://github.com/qqeOSAS/OWX_Master_utils.git
```
Example of wiring 
-------------


```
    Example of wiring OWX master with multiple OWX slaves:
                     ┌────────────────────────────────────────────────┐
                     │               One Wire Master                  ├─── GND -------------------- GND (Slave1)
                     │                                                │                            GND (Slave2)
                     │                                                ├─── VCC -------------------------------- VCC (Slave1)
                     │                                                │                                         VCC (Slave2)
                     │                                                ├─── DQ |──[4.7kΩ]─── VCC
                     └────────────────────────────────────────────────|       |
                                                                              │
                                    |----------------------------------------│ 1-Wire Bus (DQ)
                                    |                                          
                                    |
                                    |                ┌─────────────────────── SLAVE 1 ───────────────────────┐
                                    |                │                                                      │
                                    |                │   GND ----------------------------------------------------------+
                                    |                │   VCC ----------------------------------------------------------+
                                    |----------------------------------DQ
                                    |                └───────────────────────────────────────────────────────┘
                                    |
                                    |                ┌─────────────────────── SLAVE 2 ───────────────────────┐
                                    |                │                                                      
                                    |                │   GND ----------------------------------------------------------+
                                    |                │   VCC ----------------------------------------------------------+
                                    |-----------------------------+ DQ
                                                     └───────────────────────────────────────────────────────┘

```
Packet structure example
----------------------------
```

    ┌──────────────────────────────────────────────────────────────────────────────┐
    │                         OWX PACKET FORMAT (SLAVE → MASTER)                   │
    ├──────────────────────────────────────────────────────────────────────────────┤
    │ [ CMD_SEND_VARIABLE |  CMD_data_type |  LEN  |  PAYLOAD (N bytes)  |  CRC8 ] │  
    │       (1 byte)           (1 byte)     (1 byte)     (N bytes)    (1 byte)     │
    ├──────────────────────────────────────────────────────────────────────────────┤
    │  Example: 0x01 | 0x0F | 0x01 | 0x7A | CRC                                    │
    │           │       │       │       └── payload (e.g. one int8)                │
    │           │       │       └────────── length in bytes                        │
    │           │       └────────────────── command describing data type           │
    │           └────────────────────────── main command "send variable"           │
    └──────────────────────────────────────────────────────────────────────────────┘


The process of reading int16_t from scratchpad, or any other data type, involves the following steps:


    +---------------------------------------------------------------+
    |  0   |  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |
    +---------------------------------------------------------------+
    |     int16   |    int16    |   int16     |    int16    | byte  |
    +---------------------------------------------------------------+
    ^
    |
    READ offset = 0




```







