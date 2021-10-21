#include "units_converter.h"
#include <glm/gtx/compatibility.hpp>
#include <game_engine/service_locator.h>

using namespace GameEngine;
using namespace std;

bool operator==(const ComplexValue& lhs, const ComplexValue& rhs) {
    if (holds_alternative<PercentValue>(lhs) && holds_alternative<PercentValue>(rhs)) {
        auto& lhsPercentValue = get<PercentValue>(lhs);
        auto& rhsPercentValue = get<PercentValue>(rhs);
        return
            lhsPercentValue.dimensionType == rhsPercentValue.dimensionType &&
            abs(rhsPercentValue.value - lhsPercentValue.value) < FLT_EPSILON;
    }
    else if (holds_alternative<DpValue>(lhs) && holds_alternative<DpValue>(rhs)) {
        auto& lhsDpValue = get<DpValue>(lhs);
        auto& rhsDpValue = get<DpValue>(rhs);
        return abs(rhsDpValue.value - lhsDpValue.value) < FLT_EPSILON;
    }
    else if (holds_alternative<PlainValue>(lhs) && holds_alternative<PlainValue>(rhs)) {
        auto& lhsPlainValue = get<PlainValue>(lhs);
        auto& rhsPlainValue = get<PlainValue>(rhs);
        return abs(rhsPlainValue.value - lhsPlainValue.value) < FLT_EPSILON;
    }
    else {
        return false;
    }
}

bool operator!=(const ComplexValue& lhs, const ComplexValue& rhs) {
    return !(lhs == rhs);
}

ostream& operator<<(ostream& os, const ComplexValue& complexValue) {
    if (holds_alternative<PercentValue>(complexValue)) {
        auto percentValue = get<PercentValue>(complexValue);
        return os << percentValue.value << "%";
    }
    else if (holds_alternative<DpValue>(complexValue)) {
        auto dpValue = get<DpValue>(complexValue);
        return os << dpValue.value << "dp";
    }
    else {
        auto plainValue = get<PlainValue>(complexValue);
        return os << plainValue << "px";
    }
}

float GameEngine::UnitsConverter::complexValueToPixels(const ComplexValue& value)
{
    if (std::holds_alternative<PercentValue>(value)) {
        auto& percentValue = std::get<PercentValue>(value);
        auto renderingWindowInfoProvider = m_serviceLocator->renderingWindowInfoProvider();
        if (percentValue.dimensionType == DimensionType::WIDTH) {
            return glm::lerp(0.0f, renderingWindowInfoProvider->width(), percentValue.value / 100);
        } else {
            return glm::lerp(0.0f, renderingWindowInfoProvider->height(), percentValue.value / 100);
        }
    } else if (std::holds_alternative<DpValue>(value)) {
        auto& dpValue = std::get<DpValue>(value);
        auto renderingWindowInfoProvider = m_serviceLocator->renderingWindowInfoProvider();
        return dpValue.value * renderingWindowInfoProvider->densityFactor();
    } else {
        auto& plainValue = std::get<PlainValue>(value);
        return plainValue.value;
    }
}

ComplexValue GameEngine::UnitsConverter::widthPixelsToPercentValue(float pixels)
{
    return PercentValue{ DimensionType::WIDTH, pixels / m_serviceLocator->renderingWindowInfoProvider()->width() * 100 };
}

ComplexValue GameEngine::UnitsConverter::heightPixelsToPercentValue(float pixels)
{
    return PercentValue{ DimensionType::HEIGHT, pixels / m_serviceLocator->renderingWindowInfoProvider()->height() * 100 };
}

ComplexValue GameEngine::UnitsConverter::pixelsToDpValue(float pixels)
{
    return PlainValue{ pixels / m_serviceLocator->renderingWindowInfoProvider()->densityFactor() };
}
