#include "opus_graphics.h"

namespace opus {

static inline bool valid(const IndexedSurface& s) noexcept {
    return (s.pixels != nullptr) && (s.width != 0) && (s.height != 0) && (s.pitch_bytes != 0);
}

static inline bool valid(const XrgbSurface& s) noexcept {
    return (s.pixels != nullptr) && (s.width != 0) && (s.height != 0) && (s.pitch_bytes != 0);
}

// -------------------------
// Low-level pixel ops
// -------------------------

void clear(IndexedSurface dst, std::uint8_t index) {
    if (!valid(dst)) return;

    for (std::size_t y = 0; y < dst.height; ++y) {
        std::uint8_t* row = dst.pixels + y * dst.pitch_bytes;
        for (std::size_t x = 0; x < dst.width; ++x) {
            row[x] = index;
        }
    }
}

void pset(IndexedSurface dst, std::size_t x, std::size_t y, std::uint8_t index) {
    if (!valid(dst)) return;
    if (x >= dst.width || y >= dst.height) return;

    std::uint8_t* row = dst.pixels + y * dst.pitch_bytes;
    row[x] = index;
}

void blit(IndexedSurface dst, IndexedSurface src,
          std::size_t dst_x, std::size_t dst_y,
          std::uint8_t transparent_index) {
    if (!valid(dst) || !valid(src)) return;

    for (std::size_t sy = 0; sy < src.height; ++sy) {
        const std::size_t dy = dst_y + sy;
        if (dy >= dst.height) break;

        const std::uint8_t* srow = src.pixels + sy * src.pitch_bytes;
        std::uint8_t*       drow = dst.pixels + dy * dst.pitch_bytes;

        for (std::size_t sx = 0; sx < src.width; ++sx) {
            const std::size_t dx = dst_x + sx;
            if (dx >= dst.width) break;

            const std::uint8_t si = srow[sx];
            if (si == transparent_index) continue;

            drow[dx] = si;
        }
    }
}

void resolve_to_xrgb(XrgbSurface out, IndexedSurface indexed, const Palette256& pal) {
    if (!valid(out) || !valid(indexed)) return;

    // v0: strict match (keep it simple)
    if (out.width != indexed.width || out.height != indexed.height) return;

    const std::size_t out_row_pixels = out.pitch_bytes / sizeof(std::uint32_t);

    for (std::size_t y = 0; y < indexed.height; ++y) {
        const std::uint8_t* irow = indexed.pixels + y * indexed.pitch_bytes;
        std::uint32_t*      orow = out.pixels + y * out_row_pixels;

        for (std::size_t x = 0; x < indexed.width; ++x) {
            orow[x] = pal.xrgb[irow[x]];
        }
    }
}

// -------------------------
// Canvas
// -------------------------

Canvas::Canvas(std::size_t width, std::size_t height, std::size_t layer_count)
: m_width(width), m_height(height) {
    if (m_width == 0 || m_height == 0) {
        m_width = m_height = 0;
        layer_count = 0;
    }

    m_layers.resize(layer_count);
    for (auto& layer : m_layers) {
        layer.resize(m_width * m_height, 0);
    }
}

IndexedSurface Canvas::layer_surface(std::size_t layer_index) noexcept {
    if (layer_index >= m_layers.size() || m_width == 0 || m_height == 0) {
        return {};
    }

    auto& buf = m_layers[layer_index];
    return IndexedSurface{
        buf.data(),
        m_width,
        m_height,
        m_width // tight-packed: 1 byte per pixel, no padding
    };
}

void Canvas::clear_layer(std::size_t layer_index, std::uint8_t index) noexcept {
    if (layer_index >= m_layers.size()) return;
    opus::clear(layer_surface(layer_index), index);
}

void Canvas::clear_background(std::uint8_t index) noexcept {
    clear_layer(0, index);
}

void Canvas::clear_all(std::uint8_t index) noexcept {
    for (std::size_t i = 0; i < m_layers.size(); ++i) {
        clear_layer(i, index);
    }
}

bool Canvas::composite_to(IndexedSurface dst, std::uint8_t transparent_index) noexcept {
    if (!valid(dst)) return false;
    if (dst.width != m_width || dst.height != m_height) return false;

    if (m_layers.empty()) {
        opus::clear(dst, 0);
        return true;
    }

    // Copy layer 0 as base (always overwrites)
    {
        const IndexedSurface base = layer_surface(0);
        for (std::size_t y = 0; y < m_height; ++y) {
            const std::uint8_t* srow = base.pixels + y * base.pitch_bytes;
            std::uint8_t*       drow = dst.pixels  + y * dst.pitch_bytes;

            for (std::size_t x = 0; x < m_width; ++x) {
                drow[x] = srow[x];
            }
        }
    }

    // Overlay layers 1..N-1 skipping transparent_index
    for (std::size_t li = 1; li < m_layers.size(); ++li) {
        opus::blit(dst, layer_surface(li), 0, 0, transparent_index);
    }

    return true;
}

} // namespace opus
