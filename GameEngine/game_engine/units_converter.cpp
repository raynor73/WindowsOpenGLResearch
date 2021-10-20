#include "units_converter.h"

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
