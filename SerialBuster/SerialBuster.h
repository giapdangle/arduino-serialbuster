// Non blocking (chunked) Serial read/write

// Supports the use of custom commands and custom callbacks each command

// Protocol structure:
//  START     1byte (uint8)
//  RECIPIENT 1byte (uint8)
//  SENDER    1byte (uint8)
//  LENGTH    2byte (uint16)
//  PAYLOAD   Nbyte
//  CRC8      1byte (uint8)
//  END       1byte (uint8)


#ifndef SerialBuster_h
#define SerialBuster_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Buffer.h"

typedef void (*voidFuncPtr)(Buffer*);

// for checking if the serial line is busy
#if defined(__AVR_ATmega8__)
  #define UCSRA  UCSRA
  #define UDRE  UDRE
#else
  #define UCSRA  UCSR0A
  #define UDRE  UDRE0
#endif

// protocol constants
#define KS_START              0x02
#define KS_ESC                0x1B
#define KS_END                0x03
#define KS_ADDRESS_BROADCAST  0xFF
#define KS_ADDRESS_MASTER     0x00


class SerialBuster {
  public:
    SerialBuster(uint16_t in_size, uint16_t out_size);
    ~SerialBuster();
    void init(uint16_t baud_rate);
    void setPacketHandler(void (*cb)(Buffer*));
    void setAddress(uint8_t address);

    void update();
    bool isSending();
    bool isReceiving();

    uint8_t sendPacket(uint8_t recipient, Buffer * packet);
    uint8_t crc8(uint8_t *data, uint16_t len);

  protected:
    void incomingByte(uint8_t incoming);

    voidFuncPtr _cb;
    Buffer* _in_buf;
    Buffer* _out_buf;
    uint16_t _address;
};


#endif // SerialBuster_h