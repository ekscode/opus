#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

namespace opus {

// -------------------------
// Surface views (no ownership)
// -------------------------

// 8-bit indexed surface (internal render target).
struct IndexedSurface final {
    std::uint8_t* pixels = nullptr;   // indices 0..255
    std::size_t   width  = 0;         // pixels
    std::size_t   height = 0;         // pixels
    std::size_t   pitch_bytes = 0;    // bytes per row (often == width)
};

// 32-bit XRGB8888 surface (final present target for libretro).
struct XrgbSurface final {
    std::uint32_t* pixels = nullptr;  // XRGB8888 pixels
    std::size_t    width  = 0;        // pixels
    std::size_t    height = 0;        // pixels
    std::size_t    pitch_bytes = 0;   // bytes per row
};

// Palette of 256 entries. Convention: index 0 is "transparent" for compositing/blits.
struct Palette256 final {
    std::uint32_t xrgb[256] = {};     // 0xFFRRGGBB recommended
};

// Helper to pack channels into XRGB8888 with X set to 0xFF.
constexpr std::uint32_t xrgb8888(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    return (0xFFu << 24) | (std::uint32_t(r) << 16) | (std::uint32_t(g) << 8) | std::uint32_t(b);
}

// -------------------------
// Low-level pixel ops
// -------------------------

// Fill entire indexed surface with an index.
void clear(IndexedSurface dst, std::uint8_t index);

// Set a single pixel (bounds-checked).
void pset(IndexedSurface dst, std::size_t x, std::size_t y, std::uint8_t index);

// Blit src onto dst at (dst_x, dst_y). Skip pixels equal to transparent_index (default 0).
void blit(IndexedSurface dst, IndexedSurface src,
          std::size_t dst_x, std::size_t dst_y,
          std::uint8_t transparent_index = 0);

// Convert indexed -> XRGB8888 using palette. (Output is always opaque; transparency is handled earlier.)
void resolve_to_xrgb(XrgbSurface out, IndexedSurface indexed, const Palette256& pal);

// -------------------------
// Canvas (ownership + layering)
// -------------------------

class Canvas final {
public:
    Canvas() = default;
    Canvas(std::size_t width, std::size_t height, std::size_t layer_count = 1);

    std::size_t width() const noexcept  { return m_width; }
    std::size_t height() const noexcept { return m_height; }
    std::size_t layer_count() const noexcept { return m_layers.size(); }

    // Returns a surface view of the given layer; returns an empty view if out of range.
    IndexedSurface layer_surface(std::size_t layer_index) noexcept;

    // Clear helpers
    void clear_layer(std::size_t layer_index, std::uint8_t index) noexcept;

    // Convention helper: background is layer 0
    void clear_background(std::uint8_t index) noexcept;

    // Clears all layers to index.
    void clear_all(std::uint8_t index) noexcept;

    // Composite all layers into dst (dst must match size).
    // Rules:
    // - Layer 0 is copied as the base (always overwrites).
    // - Layers 1..N-1 are blended by skipping pixels equal to transparent_index.
    bool composite_to(IndexedSurface dst, std::uint8_t transparent_index = 0) noexcept;

private:
    std::size_t m_width  = 0;
    std::size_t m_height = 0;

    // Each layer is a tightly-packed (width*height) array of indices.
    std::vector<std::vector<std::uint8_t>> m_layers;
};

} // namespace opus
