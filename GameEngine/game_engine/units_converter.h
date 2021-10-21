#pragma once

#include <variant>
#include <ostream>
#include <memory>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class ServiceLocator;

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

class UnitsConverter 
{
    std::shared_ptr<ServiceLocator> m_serviceLocator;

public:
    explicit UnitsConverter(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator) {}
    virtual ~UnitsConverter() = default;

    virtual float complexValueToPixels(const ComplexValue& value);
    virtual ComplexValue widthPixelsToPercentValue(float pixels);
    virtual ComplexValue heightPixelsToPercentValue(float pixels);
    virtual ComplexValue pixelsToDpValue(float pixels);
};
}

bool operator==(const GameEngine::ComplexValue& lhs, const GameEngine::ComplexValue& rhs);
bool operator!=(const GameEngine::ComplexValue& lhs, const GameEngine::ComplexValue& rhs);

std::ostream& operator<<(std::ostream& os, const GameEngine::ComplexValue& complexValue);
