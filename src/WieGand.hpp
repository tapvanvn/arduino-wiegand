///WieGand Protocol Helper v1.0
///Duy Nguyen <tapvanvn@gmail.com>
///https://github.com/tapvanvn/arduino-wiegand

#ifndef _H_WIEGAND
#define _H_WIEGAND

#include <arduino.h>
#include <stdint.h>
#include <assert.h>

using namespace std;

template <size_t PinD0, size_t PinD1, size_t BuffSizeInByte>
class WieGand
{
public:
    WieGand()
    {

    }

    static void reset()
    {
        _ptr = _buff;
        _bit_count = 0;
    }

    static void begin()
    {
        _last_wiegand = 0;
    	_bit_count = 0;

        pinMode(PinD0, INPUT_PULLUP);
    	pinMode(PinD1, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(PinD0), readD0, CHANGE);
    	attachInterrupt(digitalPinToInterrupt(PinD1), readD1, CHANGE);

        _ptr = _buff;
    }

	static bool available()
    {
        bool ret;
        noInterrupts();

        unsigned long sys_tick = millis();

    	ret = ((sys_tick - _last_wiegand) > 25) && _bit_count > 0;

    	interrupts();

    	return ret;
    }

    static uint8_t getBitCount()
    {
        return _bit_count;
    }

    static const uint8_t* getBuffer()
    {
        return _buff;
    }


private:
    static void readD0()
    {
        if(_bit_count % 8  == BuffSizeInByte)
        {
            reset();
        }

        _bit_count++;

        *_ptr <<= 1;

        if(_bit_count % 8 == 7)
        {
            _ptr++;
            *_ptr = 0;
        }


    	_last_wiegand = millis();
    }

	static void readD1()
    {
        if(_bit_count % 8  == BuffSizeInByte)
        {
            reset();
        }

        _bit_count ++;

        *_ptr |= 1;
        *_ptr <<= 1;

        if(_bit_count % 8 == 7)
        {
            _ptr++;
            *_ptr = 0;
        }

    	_last_wiegand = millis();
    }

    static volatile unsigned long _last_wiegand;

    static volatile uint8_t _bit_count;
    static uint8_t* _ptr;

    static uint8_t _buff[BuffSizeInByte];
};

#define INIT_WIEGAND(D0,D1,BuffSizeInByte)  \
    template<> volatile unsigned long WieGand<D0,D1,BuffSizeInByte>::_last_wiegand=0; \
    template<> volatile uint8_t WieGand<D0,D1,BuffSizeInByte>::_bit_count=0; \
    template<> uint8_t* WieGand<D0,D1,BuffSizeInByte>::_ptr=0; \
    template<> uint8_t WieGand<D0,D1,BuffSizeInByte>::_buff[BuffSizeInByte] = {0};


#endif // _H_WIEGAND
