#pragma once
#include <variant>
#include <ostream>

namespace GameEngine
{
enum class DimensionType {
    WIDTH, HEIGHT
};

struct PercentValue {
    DimensionType dimensionType;
    float value;
};

struct DpValue {
    float value;
};

struct PlainValue {
    float value;
};

typedef std::variant<PercentValue, DpValue, PlainValue> ComplexValue;

class UnitsConverter {

public:
    virtual ~UnitsConverter() = default;

    virtual float complexValueToPixels(const ComplexValue& value) = 0;
    virtual ComplexValue widthPixelsToPercentValue(float pixels) = 0;
    virtual ComplexValue heightPixelsToPercentValue(float pixels) = 0;
    virtual ComplexValue pixelsToDpValue(float pixels) = 0;
};
}

bool operator==(const GameEngine::ComplexValue& lhs, const GameEngine::ComplexValue& rhs);
bool operator!=(const GameEngine::ComplexValue& lhs, const GameEngine::ComplexValue& rhs);

std::ostream& operator<<(std::ostream& os, const GameEngine::ComplexValue& complexValue);
