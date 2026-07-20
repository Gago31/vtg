#ifndef VIDEO_PLAYER_SOFT
#define VIDEO_PLAYER_SOFT

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/image.hpp>

#include "video_player.h"
#include "sav1.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace godot {

class VideoPlayerSoft : public VideoPlayer {
    GDCLASS(VideoPlayerSoft, VideoPlayer)

public:
    VideoPlayerSoft();
    ~VideoPlayerSoft();

    void play() override;
    void pause() override;
    void stop() override;
    bool is_playing() override;
    void update_frame() override;
    void init_playback();
    void set_playback_time(double playback_time) override;
    void seek_playback_fast(double playback_time) override;
    double get_playback_time() override;
    double get_playback_duration() override;
    void set_playback_speed(double playback_speed) override;
    double get_playback_speed() override;

protected:
    static void _bind_methods();

private:
    double time_passed;
    Ref<ImageTexture> image_texture;
    Ref<Image> image;

    Sav1Settings settings;
    Sav1Context context = {0};

    SDL_AudioSpec audio_spec = {0};
    SDL_AudioDeviceID audio_device;

    bool stopped = false;
    bool must_seek = false;
    uint64_t seek_target = 0;
    Sav1SeekMode seek_mode;

private:
    bool create_video_context();
    void process_video_frame();
    void process_audio_frame();
    int initialize_sdl(Sav1Settings settings, SDL_AudioDeviceID *device_p);
    void cleanup_sdl(SDL_AudioDeviceID audio_device);
};

}

#endif // VIDEO_PLAYER_SOFT

