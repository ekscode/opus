#pragma once
#include <array>
#include <cstdint>

namespace opus::temp
{
    //***** Palette *****/
    class Palette
    {
    public:
        static constexpr uint16_t MAX_COLORS = 256;

        Palette();

        // Logical number of defined colors (0..256)
        uint16_t GetNumColors() const;
        void     SetNumColors(uint16_t count); // clamps to 0..256

        // Reset palette to empty (num colors = 0, colors zeroed)
        void Clear();

        // Direct access (0..255). No bounds checking by design.
        const ColorXRGB& GetColor(uint8_t index) const;
        ColorXRGB&       GetColor(uint8_t index);

        // Sets a color at index and auto-expands num colors to at least index+1.
        void SetColor(uint8_t index, const ColorXRGB& color);

    private:
        std::array<ColorXRGB, MAX_COLORS> m_colors;
        uint16_t m_numColors; // 0..256
    };
}
