#pragma once

#include <string>
#include <game_engine/units_converter.h>

namespace GameEngine 
{
class TextAppearance
{
    ComplexValue m_textSize;
    std::string m_fontPath;

public:
    TextAppearance(
        ComplexValue textSize,
        std::string fontPath
    ) : m_textSize(std::move(textSize)),
        m_fontPath(std::move(fontPath)) {}

    const ComplexValue& textSize() const { return m_textSize; }
    const std::string& fontPath() const { return m_fontPath; }

    bool operator==(const TextAppearance& other) const;
};
}
