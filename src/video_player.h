#ifndef VIDEO_PLAYER
#define VIDEO_PLAYER

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {

class VideoPlayer : public RefCounted {
    GDCLASS(VideoPlayer, RefCounted)

public:
    VideoPlayer();
    virtual ~VideoPlayer() = 0;

    void set_file_name(const String& p_file_name);
    String get_file_name() const;

    Ref<Texture2D> get_texture();

    void set_looping(bool looping);
    bool is_looping() const;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual bool is_playing() = 0;

    virtual void update_frame() = 0;

    virtual void set_playback_time(double playback_time) = 0;
    virtual void seek_playback_fast(double playback_time) = 0;
    virtual double get_playback_time() = 0;
    virtual double get_playback_duration() = 0;
    virtual void set_playback_speed(double playback_speed) = 0;
    virtual double get_playback_speed() = 0;
    

protected:
    String file_name;
    Ref<Texture2D> texture;
    bool looping = false;
    static void _bind_methods();

};

}

#endif // VIDEO_PLAYER

