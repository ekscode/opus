#pragma once
#include <array>
#include <cstdint>

// Class Color
namespace opus::gfx
{
    class Color
    {
    public:
        // Constructor and Destructor
        Color();
        Color(uint32_t xrgb);
        ~Color();

        // Getters
        uint32_t GetXRGB() const;
        uint32_t GetRGB() const;
        uint8_t GetX() const;
        uint8_t GetR() const;
        uint8_t GetG() const;
        uint8_t GetB() const;

        // Setters
        void SetXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b);
        void SetRGB(uint8_t r, uint8_t g, uint8_t b);
        void SetX(uint8_t x);
        void SetR(uint8_t r);
        void SetG(uint8_t g);
        void SetB(uint8_t b);

        // Helpers
        static Color FromRGB(uint8_t r, uint8_t g, uint8_t b);
        static Color FromXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b);

    private:
        uint32_t m_color;
    };
}

// Class Palette
namespace opus::gfx
{
    class Palette
    {
    public:
        static constexpr uint8_t MAX_COLORS = 256;

        // Constructor and Destructor
        Palette();
        ~Palette();

        // Getters
        uint8_t GetNumColor();
        const Color& GetColor(uint8_t index) const;
        Color& GetColor(uint8_t index);

        // Setters
        void SetColor(uint8_t index, const Color& color);

    private:
        uint8_t m_numColors; // 0..255
        std::array<Color, MAX_COLORS> m_palette;
    };
}
