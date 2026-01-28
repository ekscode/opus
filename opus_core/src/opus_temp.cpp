#include "opus_temp.h"

namespace opus::temp
{

    //***** Palette *****/
    Palette::Palette()
        : m_colors{}, m_numColors(0)
    {
        // std::array default-initializes; ColorXRGB should default to 0.
    }

    uint16_t Palette::GetNumColors() const
    {
        return m_numColors;
    }

    void Palette::SetNumColors(uint16_t count)
    {
        if (count > MAX_COLORS)
        {
            m_numColors = MAX_COLORS;
        }
        else
        {
            m_numColors = count;
        }
    }

    void Palette::Clear()
    {
        m_numColors = 0;

        // Reset all entries to default ColorXRGB
        for (uint16_t i = 0; i < MAX_COLORS; ++i)
        {
            m_colors[static_cast<size_t>(i)] = ColorXRGB{};
        }
    }

    const ColorXRGB& Palette::GetColor(uint8_t index) const
    {
        return m_colors[index];
    }

    ColorXRGB& Palette::GetColor(uint8_t index)
    {
        return m_colors[index];
    }

    void Palette::SetColor(uint8_t index, const ColorXRGB& color)
    {
        m_colors[index] = color;

        const uint16_t needed = static_cast<uint16_t>(index) + 1;
        if (needed > m_numColors)
        {
            m_numColors = needed;
        }
    }


}