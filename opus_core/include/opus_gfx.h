#pragma once
#include <array>
#include <cstdint>

namespace opus::gfx
{
    /***** Class ColorXRGB ******/ 
    class ColorXRGB
    {
    public:
        ColorXRGB();
        explicit ColorXRGB(uint32_t packed);

        // Factory helpers
        static ColorXRGB FromRGB(uint8_t r, uint8_t g, uint8_t b);
        static ColorXRGB FromXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b);

        // Whole values
        uint32_t GetXRGB() const; // 0x??RRGGBB
        uint32_t GetRGB() const;  // 0x00RRGGBB

        // Set whole value
        void SetXRGB(uint32_t packed);
        void SetXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b);

        // Channels
        uint8_t GetX() const;
        uint8_t GetR() const;
        uint8_t GetG() const;
        uint8_t GetB() const;

        void SetX(uint8_t x);
        void SetR(uint8_t r);
        void SetG(uint8_t g);
        void SetB(uint8_t b);
        void SetRGB(uint8_t r, uint8_t g, uint8_t b);

    private:
        uint32_t m_value;
    };

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