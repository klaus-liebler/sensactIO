#pragma once
#include <cstddef>
#include <inttypes.h>
#include "errorcodes.hh"
#include "crgb.hh"
#include "generated/webui_core_comm_generated.h"

#define SetBit(A, k) (A[(k / 32)] |= (1 << (k % 32)))
#define ClearBit(A, k) (A[(k / 32)] &= ~(1 << (k % 32)))
#define TestBit(A, k) (A[(k / 32)] & (1 << (k % 32)))

template <typename T>
void _SetBitIdx(T &value, const int bitIdx) {
    value = value | (1 << bitIdx);
}
#define SetBitIdx(number, n)  ((number) |= (1UL << (n)))

template <typename T>
bool _GetBitIdx(const T value, const int bitIdx) {
    return value & (1 << bitIdx);
}
#define GetBitIdx(var,pos) ((var) & (1<<(pos)))

template <typename T> 
bool IntervalIntersects(const T& aLow, const T& aHigh, const T& bLow, const T& bHigh) {
    T min = std::max(aLow, bLow);
    T max =  std::min(aHigh, bHigh);
    return  max>=min;
}

template <size_t K>
bool GetBitInU8Array(const std::array<uint8_t, K> *arr, size_t offset, size_t bitIdx){
    uint8_t b = (*arr)[offset + (bitIdx>>3)];
    uint32_t bitpos = bitIdx & 0b111;
    return b & (1<<bitpos);
}


uint32_t constexpr mix(char m, uint32_t s)
{
    return ((s << 7) + ~(s >> 3)) + ~m;
}

uint32_t constexpr hashStr(const char *m)
{
    return (*m) ? mix(*m, hashStr(m + 1)) : 0;
}

typedef uint32_t Time_t;

constexpr uint16_t ACTIVE = UINT16_MAX;
constexpr uint16_t INACTIVE = 0;

namespace pins{
    namespace sensactOutdoor{
        constexpr uint16_t RELAY1 = 1;
        constexpr uint16_t RELAY2 = 2;
        constexpr uint16_t RELAY3 = 3;
        constexpr uint16_t RELAY4 = 4;
        constexpr uint16_t RELAY5 = 5;
        constexpr uint16_t RELAY6 = 6;

        constexpr uint16_t MOSFET1 = 7;
        constexpr uint16_t MOSFET2 = 8;
        constexpr uint16_t MOSFET3 = 9;
        constexpr uint16_t MOSFET4 = 10;
        constexpr uint16_t MOSFET5 = 11;
        constexpr uint16_t LED_CC = 12;
        constexpr uint16_t PIN_MAX = LED_CC;
    }
    namespace wroverkit{
        constexpr uint16_t LED_R = 1;
        constexpr uint16_t LED_G = 2;
        constexpr uint16_t LED_B = 3;
        constexpr uint16_t PIN_MAX = LED_B;
    }
    namespace sensactHsIO4{

        constexpr uint16_t J4_PIN1=1;//Input only
        constexpr uint16_t J4_PIN2=2;//Input only
        constexpr uint16_t J4_PIN3=3;//Input only
        constexpr uint16_t J4_PIN4=4;//Input only
        constexpr uint16_t J4_PIN5=5;
        constexpr uint16_t J4_PIN6=6;
        constexpr uint16_t J4_PIN7=7;
        constexpr uint16_t J4_PIN8=8;
        constexpr uint16_t J5_PIN1=9;
        constexpr uint16_t J5_PIN2=10;
        constexpr uint16_t J5_PIN3=11;
        constexpr uint16_t J5_PIN4=12;
        constexpr uint16_t J5_PIN5=13;
        constexpr uint16_t J5_PIN6=14;
        constexpr uint16_t J5_PIN7=15;
        constexpr uint16_t J5_PIN8=16;
        constexpr uint16_t J3_PIN1_2 = 17;//Output only
        constexpr uint16_t J3_PIN3_3 = 18;//Output only
        constexpr uint16_t J3_PIN5_6 = 19;//Output only
        constexpr uint16_t J3_PIN7_8 = 20;//Output only
        constexpr uint16_t PIN_MAX = J3_PIN7_8;
    }
}

enum class IOMode:uint8_t
{
    INPUT = 0,
    OUTPUT_OPEN_DRAIN = 1,
    OUTPUT_PUSH_PULL = 3,
};

/**
 * Important: Input pinId and output pinId are in the same "numberspace". Nur dadurch ist es möglich, einen physischen Pin sowohl als Eingang als auch als Ausgang zu konfigurieren
 */
class InputOutput{
    public:
    /**
     * @param pinId a value of zero means: no pin, just a null-device. All other values are hardware specific and defined in the "pins" namespace
     * */
    virtual ErrorCode ConfigureIO(uint16_t pinId, IOMode mode) = 0;
    /**
     * @param state 0 completely off, UINT16_MAX=completely on; all between is implementation specific
     */
    virtual ErrorCode SetU16Output(uint16_t pinId, uint16_t state) = 0;
    /**
     * @param value bit field with the first 32 (31) boolean inputs as bits (those with id 1...31; 0 is the "null-pin") 
     */
    virtual ErrorCode GetBoolInputs(uint32_t *value) = 0;
};

class HAL:public InputOutput
{
public:
    virtual ErrorCode Setup() = 0;
    virtual ErrorCode ConfigureIO(uint16_t pinId, IOMode mode) = 0;
    /**
     * @param state 0 completely off, UINT16_MAX=completely on; all between is implementation specific
     */
    virtual ErrorCode SetU16Output(uint16_t pinId, uint16_t state) = 0;
    ErrorCode SetU16Output(uint16_t pinId, bool state){SetU16Output(pinId, state?ACTIVE:INACTIVE);return ErrorCode::OK;}
    /**
     * @param value bit field with all inputs as bits (max 32 inputs) 
     */
    virtual size_t GetPinCnt()=0;
    virtual ErrorCode GetBoolInputs(uint32_t *value) = 0;
    virtual ErrorCode ColorizeLed(uint8_t index, CRGB color) = 0;
    virtual ErrorCode UnColorizeAllLed() = 0;
    virtual int64_t GetMicros() = 0;
    virtual uint32_t GetMillis() = 0;
    virtual int64_t GetMillis64() = 0;
    virtual ErrorCode AfterLoop() = 0;
    virtual ErrorCode BeforeLoop() = 0;
};

class PostOffice
{
public:
    virtual ErrorCode PostCommand(const sensact::comm::tCommand* cmd)=0;
};

class IOSource
{
public:
    virtual void SetInputs(InputOutput *io) = 0;
    virtual void SetOutputs(InputOutput *io) = 0;
    virtual void Configure(InputOutput *io) =0;
};

class BinaryWriter
{
public:
    virtual ErrorCode StartWriting(uint32_t applicationId, size_t sizeIn4byteUnits);
    virtual ErrorCode WriteU32(uint32_t value);
    virtual ErrorCode WriteF32(float value);
    virtual ErrorCode EndWriting(uint32_t applicationId);
};

struct SensactContext
{
    Time_t now;
    InputOutput *io;
    PostOffice *postOffice;
};