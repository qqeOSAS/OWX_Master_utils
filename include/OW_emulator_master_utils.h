#ifndef OW_EMULATOR_MASTER_UTILS_H
#define OW_EMULATOR_MASTER_UTILS_H


#include <Arduino.h>
#include <OneWire.h>


// Packet command definitions
#define OW_LOW_CMD_SEND_VARIABLE_ 0x01  // відправка змінної зі слейва
#define OW_CMD_INT8        0x0F  // payload: 1 byte (int8_t)
#define OW_CMD_INT16       0x0E  // payload: 2 bytes (int16_t, LSB first)
#define OW_CMD_UINT16      0x0D  // payload: 2 bytes (uint16_t, LSB first)
#define OW_CMD_UINT32      0x12  // payload: 4 bytes (uint32_t, LSB first)
#define OW_CMD_INT32       0x10  // payload: 4 bytes (int32_t, LSB first)
#define OW_CMD_FLOAT32     0x11  // payload: 4 bytes (IEEE754 float, LSB first)
#define OW_CMD_CHAR8       0x13  // payload: 1 byte (char)
#define OW_CMD_STRUCT      0x14  // payload: N bytes (структура, LEN в заголовку)

#define OW_CMD_REQUEST     0x20  // майстер просить значення від слейва
#define OW_CMD_ACK         0x30  // підтвердження прийому
#define OW_CMD_NACK        0x31  // помилка

#define REQEST_READ_INT16 0xA0 // запит на читання int16_t зі слейва
#define OW_SCRATCHPAD_SIZE 9  // розмір scratchpad в байтах

// Формат пакету для передачі даних між майстром та слейвом
//[CMD SEND_VARIABLE | CMD_variable | LEN | PAYLOAD... | CRC8 ]
//      1               1               1       N         1

// CRC8 (Dallas/Maxim)
static uint8_t crc8_local(const uint8_t *data, size_t len, uint8_t crc_init = 0) {
    uint8_t crc = crc_init;
    while (len--) {
        uint8_t in = *data++;
        for (uint8_t i = 0; i < 8; ++i) {
            uint8_t mix = (crc ^ in) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            in >>= 1;
        }
    }
    return crc;
}


// Select the slave device by its ROM address
void select_OW_slave(OneWire& ow, uint8_t* rom_addr){
    ow.reset();
    ow.select(rom_addr); 

}
void write_packet(uint8_t cmd,uint8_t* data, int len, OneWire& ow) {

    uint8_t crc8_buffer[2 + len];
    crc8_buffer[0] = cmd;
    crc8_buffer[1] = len;

    for(int i = 0; i < len; i++)
        crc8_buffer[2+ i] = data[i];

    // crc8
    uint8_t packet_crc = crc8_local(crc8_buffer,2+len);
    crc8_buffer[2+len] = packet_crc;



    
    ow.reset();
	ow.skip(); // Select
    //writing packet
    ow.write(OW_LOW_CMD_SEND_VARIABLE_);
    //Serial.printf("[DEBUG] sending byte: 0x%02X\n", OW_LOW_CMD_SEND_VARIABLE_);
    ow.write(cmd);
   // Serial.printf("[DEBUG] sending byte: 0x%02X\n", cmd);
    ow.write(len);

    for(int i = 0; i < len; i++){
        ow.write(data[i]);
      //  Serial.printf("[DEBUG] sending byte: 0x%02X\n", data[i]);
    }
    ow.write(packet_crc);
 //   Serial.printf("[DEBUG] sending byte: 0x%02X\n", packet_crc);

    ow.reset();
   
}
void ow_write_int8(OneWire& ow, int8_t &value){
    uint8_t data[1];
    data[0] = (uint8_t)value;
    write_packet(OW_CMD_INT8, data, 1, ow);
}

void ow_write_int16(OneWire& ow, int16_t &value){
    uint8_t data[2];
    data[0] = (value) & 0xFF;         // LSB
    data[1] = ((value) >> 8) & 0xFF;  // MSB
    write_packet(OW_CMD_INT16, data, 2, ow);
}

void ow_write_uint16(OneWire& ow, uint16_t &value){
    uint8_t data[2];
    data[0] = (value) & 0xFF;         // LSB
    data[1] = ((value) >> 8) & 0xFF;  // MSB
    write_packet(OW_CMD_UINT16, data, 2, ow);
}

void ow_write_int32(OneWire& ow, int32_t &value){
    uint8_t data[4];
    data[0] = (value) & 0xFF;         
    data[1] = ((value) >> 8) & 0xFF;  
    data[2] = ((value) >> 16) & 0xFF; 
    data[3] = ((value) >> 24) & 0xFF; 
    write_packet(OW_CMD_INT32, data, 4, ow);
}
void ow_write_uint32(OneWire& ow, int32_t &value){
    uint8_t data[4];
    data[0] = (value) & 0xFF;         
    data[1] = ((value) >> 8) & 0xFF;  
    data[2] = ((value) >> 16) & 0xFF; 
    data[3] = ((value) >> 24) & 0xFF; 
    write_packet(OW_CMD_UINT32, data, 4, ow);
}

void ow_write_float32(OneWire& ow, float &value){
    uint8_t data[4];
    memcpy(data, &value, sizeof(float)); // копіюємо байти float у масив
    write_packet(OW_CMD_FLOAT32, data, 4, ow);
}

void ow_write_char8(OneWire& ow, char &value){
    uint8_t data[1];
    data[0] = (uint8_t)value;
    write_packet(OW_CMD_CHAR8, data, 1, ow);
}

void recive_slave_int16_packet(OneWire& ow, int16_t *value){
    ow.reset();
    ow.skip(); // Select
    ow.write(REQEST_READ_INT16);

    //очікуємо відповідь
    uint8_t resp_cmd = ow.read();
    if (resp_cmd == REQEST_READ_INT16) {
        uint8_t data[2];
        ow.read_bytes(data, 2);
        *value = (int16_t)(data[0] | (data[1] << 8));
    }
}









#endif