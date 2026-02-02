#include "opus_gfx.h"

// Class ColorXRGB
namespace opus::gfx
{
    // Constructor
    Color::Color() : m_color(0) 
    {
    }
    Color::Color(uint32_t xrgb) : m_color(xrgb) 
    {
    }
    Color::~Color()
    {
    }

    // Getters
    uint32_t Color::GetXRGB() const 
    {
        return uint32_t(m_color);
    }
    uint32_t Color::GetRGB() const 
    {
        return (m_color & 0x00FFFFFFu);
    } 
    uint8_t Color::GetX() const 
    {
        return uint8_t((m_color >> 24) & 0xFFu);
    }
    uint8_t Color::GetR() const 
    {
        return uint8_t((m_color >> 16) & 0xFFu);
    }
    uint8_t Color::GetG() const 
    {
        return uint8_t((m_color >>  8) & 0xFFu);
    }
    uint8_t Color::GetB() const 
    {
        return uint8_t((m_color >>  0) & 0xFFu);
    }

    // Setters
    void Color::SetX(uint8_t x)
    {
        m_color = (m_color & 0x00FFFFFFu) | (uint32_t(x) << 24);
    }
    void Color::SetR(uint8_t r) 
    {
        m_color = (m_color & 0xFF00FFFFu) | (uint32_t(r) << 16);
    }
    void Color::SetG(uint8_t g) 
    {
        m_color = (m_color & 0xFFFF00FFu) | (uint32_t(g) << 8);
    }
    void Color::SetB(uint8_t b) 
    {
        m_color = (m_color & 0xFFFFFF00u) | (uint32_t(b) << 0);
    }
    void Color::SetXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b)
    {
        m_color = (uint32_t(x) << 24) |
                  (uint32_t(r) << 16) |
                  (uint32_t(g) << 8)  |
                  (uint32_t(b) << 0);
    }
    void Color::SetRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        m_color = (m_color & 0xFF000000u) |
                  (uint32_t(r) << 16) |
                  (uint32_t(g) << 8)  |
                  (uint32_t(b) << 0);
    }

    // Helpers
    Color Color::FromXRGB(uint8_t x, uint8_t r, uint8_t g, uint8_t b)
    {
        Color c;
        c.SetXRGB(x, r, g, b);
        return c;
    }
    Color Color::FromRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return FromXRGB(0, r, g, b);
    }
}

// Class PaletteXRGB
namespace opus::gfx
{
        // Constructor and Destructor
        Palette::Palette() : m_palette{}, m_numColors(0)
        {
        }
        Palette::~Palette()
        {
        };

        // Getters
        uint8_t Palette::GetNumColor()
        {
            return m_numColors;
        }
        const Color& Palette::GetColor(uint8_t index) const 
        {
            return m_palette[index];
        }
        Color& Palette::GetColor(uint8_t index)
        {

        };

        // Setters
        
    }

// Class Surface


// Class Drawable
namespace opus::gfx
{
    // Constructor and Destructor
    Drawable::Drawable()
        : m_renderable(true)
    {
    }
    Drawable::~Drawable()
    {
    }

    bool Drawable::IsDrawable() { return m_renderable; }
}

// Class DrawableTask
namespace opus::gfx
{
    
    bool DrawableTask::AddDrawable(Drawable& drawable)
    {
        // prevent duplicates
        auto it = std::find(m_drawables.begin(), m_drawables.end(), &drawable);
        if (it != m_drawables.end())
            return false;

        m_drawables.push_back(&drawable);
        return true;
    }

    bool DrawableTask::RemoveDrawable(Drawable& drawable)
    {
        auto it = std::remove(m_drawables.begin(), m_drawables.end(), &drawable);
        if (it == m_drawables.end())
            return false;

        m_drawables.erase(it, m_drawables.end());
        return true;
    }

    bool DrawableTask::Clear()
    {
        m_drawables.clear();
        return true;
    }

    void DrawableTask::OnUpdate(uint64_t /*count*/)
    {
        // Draw in the order they were added
        for (auto* d : m_drawables)
        {
            if (!d)
                continue;

            // Your current API names it IsDrawable(); use it as a visibility gate.
            if (!d->IsDrawable())
                continue;
        }
    }
}

