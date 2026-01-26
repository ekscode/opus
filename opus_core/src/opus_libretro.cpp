// opus_libretro.cpp - RGB565 red/blue checkerboard test pattern (320x240)

#include "opus_libretro.h"

// ------------------------------------------------------------
// Globals / libretro callbacks
// ------------------------------------------------------------
static retro_environment_t        g_environ     = nullptr;
static retro_video_refresh_t      g_video       = nullptr;
static retro_audio_sample_t       g_audio       = nullptr;
static retro_audio_sample_batch_t g_audio_batch = nullptr;
static retro_input_poll_t         g_input_poll  = nullptr;
static retro_input_state_t        g_input_state = nullptr;

// ------------------------------------------------------------
// Video config
// ------------------------------------------------------------
static constexpr int WIDTH  = 320;
static constexpr int HEIGHT = 240;

static uint16_t g_framebuffer[WIDTH * HEIGHT];

// Checkerboard config (tile size in pixels)
static constexpr int TILE_W = 8;
static constexpr int TILE_H = 8;

// RGB565 colors
static constexpr uint16_t RGB565_RED  = 0xF800;
static constexpr uint16_t RGB565_BLUE = 0x001F;

static void render_checkerboard_rgb565()
{
   for (int y = 0; y < HEIGHT; ++y)
   {
      const int ty = (y / TILE_H);
      for (int x = 0; x < WIDTH; ++x)
      {
         const int tx = (x / TILE_W);
         const bool even = ((tx + ty) & 1) == 0;
         g_framebuffer[y * WIDTH + x] = even ? RGB565_RED : RGB565_BLUE;
      }
   }
}

// ------------------------------------------------------------
// libretro API (export EVERYTHING RetroArch expects)
// ------------------------------------------------------------
extern "C" {

RETRO_API void retro_init(void) {}
RETRO_API void retro_deinit(void) {}

RETRO_API unsigned retro_api_version(void) { return RETRO_API_VERSION; }

RETRO_API void retro_set_environment(retro_environment_t cb)
{
   g_environ = cb;

   // Tell frontend we can use RGB565
   retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
   if (g_environ)
      g_environ(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);

   // Support launching with no content
   bool no_content = true;
   if (g_environ)
      g_environ(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);
}

RETRO_API void retro_set_video_refresh(retro_video_refresh_t cb) { g_video = cb; }
RETRO_API void retro_set_audio_sample(retro_audio_sample_t cb) { g_audio = cb; }
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { g_audio_batch = cb; }
RETRO_API void retro_set_input_poll(retro_input_poll_t cb) { g_input_poll = cb; }
RETRO_API void retro_set_input_state(retro_input_state_t cb) { g_input_state = cb; }

// *** THIS IS THE ONE RetroArch says is missing ***
RETRO_API void retro_set_controller_port_device(unsigned /*port*/, unsigned /*device*/)
{
   // No-op for now (fine for a test-pattern core).
}

RETRO_API void retro_get_system_info(retro_system_info* info)
{
   std::memset(info, 0, sizeof(*info));
   info->library_name     = "Opus Test Pattern";
   info->library_version  = "1.0";
   info->need_fullpath    = false;
   info->valid_extensions = ""; // no content required
}

RETRO_API void retro_get_system_av_info(retro_system_av_info* info)
{
   std::memset(info, 0, sizeof(*info));
   info->timing.fps        = 60.0;
   info->timing.sample_rate= 48000.0;

   info->geometry.base_width   = WIDTH;
   info->geometry.base_height  = HEIGHT;
   info->geometry.max_width    = WIDTH;
   info->geometry.max_height   = HEIGHT;
   info->geometry.aspect_ratio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
}

RETRO_API void retro_reset(void) {}

RETRO_API bool retro_load_game(const retro_game_info* /*game*/)
{
   std::memset(g_framebuffer, 0, sizeof(g_framebuffer));
   return true;
}

// Some frontends expect this symbol to exist even if you don’t use it.
RETRO_API bool retro_load_game_special(unsigned /*game_type*/,
                                       const retro_game_info* /*info*/,
                                       size_t /*num_info*/)
{
   return false;
}

RETRO_API void retro_unload_game(void) {}

RETRO_API unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }

RETRO_API size_t retro_serialize_size(void) { return 0; }
RETRO_API bool retro_serialize(void* /*data*/, size_t /*len*/) { return false; }
RETRO_API bool retro_unserialize(const void* /*data*/, size_t /*len*/) { return false; }

RETRO_API void retro_cheat_reset(void) {}
RETRO_API void retro_cheat_set(unsigned /*index*/, bool /*enabled*/, const char* /*code*/) {}

RETRO_API void* retro_get_memory_data(unsigned /*id*/) { return nullptr; }
RETRO_API size_t retro_get_memory_size(unsigned /*id*/) { return 0; }

RETRO_API void retro_run(void)
{
   if (g_input_poll)
      g_input_poll();

   render_checkerboard_rgb565();

   const size_t pitch = WIDTH * sizeof(uint16_t);
   if (g_video)
      g_video(g_framebuffer, WIDTH, HEIGHT, pitch);

   // No audio for this test core.
}

} // extern "C"
