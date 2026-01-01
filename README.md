# OWX Master Utils

A helper library for building **extended OneWire (1-Wire) master devices** that communicate with **OWX-compatible slave emulators**.

> ⚠️ This library **does NOT replace** the original OneWire library.  
> It **extends** it with a higher-level, packet-based protocol.

Author: **qqeOSAS**  
GitHub: https://github.com/qqeOSAS/OWX_Master_utils

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
