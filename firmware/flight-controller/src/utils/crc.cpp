#include "utils.h"

constexpr uint16_t CRC16 = 0x8005;

uint16_t calculateCRC16(const uint8_t* buffer, size_t bufferSize){
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if(buffer == nullptr)
        return 0;

    while(bufferSize > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*buffer >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            buffer++;
            bufferSize--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

    return crc;
}