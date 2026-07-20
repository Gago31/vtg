#ifndef VIDEO_STREAM_PLAYER_AV1
#define VIDEO_STREAM_PLAYER_AV1

// #include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/video_stream.hpp>
#include <godot_cpp/classes/video_stream_playback.hpp>

#include "video_player_soft.h"


namespace godot {

class VideoStreamPlayerAv1 : public Control{
    GDCLASS(VideoStreamPlayerAv1, Control)

public:
    VideoStreamPlayerAv1();
    ~VideoStreamPlayerAv1();

		Size2 get_minimum_size() const;
		void set_expand(bool p_expand);
		bool has_expand() const;

		Ref<Texture2D> get_video_texture() const;

    void play();
    void pause();
    void stop();

		void set_playing(bool value);
    bool is_playing() const;
    // void update_frame();
		
		void set_file_name(String file_name);
		String get_file_name() const;
		
		void init_playback();
		// void seek_playback_fast(double playback_time);
		void set_playback_position(double playback_time);
		double get_playback_position() const;
		double get_playback_duration() const;
		
		void set_playback_speed(double playback_speed);
		double get_playback_speed() const;
		
		void set_looping(bool value);
		bool is_looping() const;

		void set_autoplay(bool value);
		bool has_autoplay() const;

protected:
    static void _bind_methods();
		void _notification(int p_notification);
		// void _validate_property(PropertyInfo &p_property) const;

private:
		String file_name;
		Size2 texture_size;
		bool loop = false;
		bool expand = false;
		bool autoplay = false;
		Ref<Texture2D> texture;
		Ref<VideoPlayerSoft> video_player;

		void texture_changed(const Ref<Texture2D> &p_texture);
};

}

#endif // VIDEO_STREAM_PLAYER_AV1

