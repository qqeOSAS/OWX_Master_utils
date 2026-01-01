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


