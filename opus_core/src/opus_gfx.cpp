#include "opus_gfx.h"

namespace opus::gfx
{
    static constexpr uint32_t MASK_RGB = 0x00FFFFFFu;


    /***** Class ColorXRGB ******/ 
    ColorXRGB::ColorXRGB()
        : m_value(0)
    {
    }

    ColorXRGB::ColorXRGB(uint32_t packed)
        : m_value(packed)
    {
    }

    ColorXRGB ColorXRGB::FromRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return FromXRGB(0, r, g, b);
    }

    ColorXRGB ColorXRGB::FromXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b)
    {
        ColorXRGB c;
        c.SetXRGB(x, r, g, b);
        return c;
    }

    uint32_t ColorXRGB::GetXRGB() const
    {
        return m_value;
    }

    uint32_t ColorXRGB::GetRGB() const
    {
        return (m_value & MASK_RGB);
    }

    void ColorXRGB::SetXRGB(uint32_t packed)
    {
        m_value = packed;
    }

    void ColorXRGB::SetXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b)
    {
        m_value =
            (uint32_t(x) << 24) |
            (uint32_t(r) << 16) |
            (uint32_t(g) << 8)  |
            (uint32_t(b) << 0);
    }

    uint8_t ColorXRGB::GetX() const { return uint8_t((m_value >> 24) & 0xFFu); }
    uint8_t ColorXRGB::GetR() const { return uint8_t((m_value >> 16) & 0xFFu); }
    uint8_t ColorXRGB::GetG() const { return uint8_t((m_value >>  8) & 0xFFu); }
    uint8_t ColorXRGB::GetB() const { return uint8_t((m_value >>  0) & 0xFFu); }

    void ColorXRGB::SetX(uint8_t x)
    {
        m_value = (m_value & MASK_RGB) | (uint32_t(x) << 24);
    }

    void ColorXRGB::SetR(uint8_t r)
    {
        m_value = (m_value & 0xFF00FFFFu) | (uint32_t(r) << 16);
    }

    void ColorXRGB::SetG(uint8_t g)
    {
        m_value = (m_value & 0xFFFF00FFu) | (uint32_t(g) << 8);
    }

    void ColorXRGB::SetB(uint8_t b)
    {
        m_value = (m_value & 0xFFFFFF00u) | (uint32_t(b) << 0);
    }

    void ColorXRGB::SetRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        const uint32_t xpart = (m_value & 0xFF000000u);
        m_value = xpart |
                  (uint32_t(r) << 16) |
                  (uint32_t(g) << 8)  |
                  (uint32_t(b) << 0);
    }

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