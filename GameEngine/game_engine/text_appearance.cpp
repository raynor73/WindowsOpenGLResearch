#include "text_appearance.h"

using namespace GameEngine;
using namespace std;

bool TextAppearance::operator==(const TextAppearance& other) const
{
    if (this == &other) {
        return true;
    }

    return m_fontPath == other.m_fontPath && m_textSize == other.m_textSize;
}
