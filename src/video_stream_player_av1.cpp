#include "video_stream_player_av1.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "memory.h"

using namespace godot;


VideoStreamPlayerAv1::VideoStreamPlayerAv1() {
	video_player = Ref<VideoPlayerSoft>(memnew(VideoPlayerSoft));
	video_player->set_file_name(file_name);
	set_process_internal(true);
	// UtilityFunctions::print("VideoStreamPlayerAv1");
	// if (video_player.is_null()) {
	// 	UtilityFunctions::print("null player");
	// } else {

	// }
}

VideoStreamPlayerAv1::~VideoStreamPlayerAv1() {

}

void VideoStreamPlayerAv1::play() {
	video_player->play();
}

void VideoStreamPlayerAv1::pause() {
	video_player->pause();
}

void VideoStreamPlayerAv1::stop() {
	video_player->stop();
}

void VideoStreamPlayerAv1::set_playing(bool value) {
	if (file_name == "" || video_player == NULL) return;
	if (value) {
		video_player->play();
	} else {
		video_player->pause();
	}
}

bool VideoStreamPlayerAv1::is_playing() const {
	if (file_name == "" || video_player.is_null()) return false;
	return video_player->is_playing();
}

void VideoStreamPlayerAv1::set_playback_position(double playback_time) {
	video_player->seek_playback_fast(playback_time);
	// video_player->set_playback_time(playback_time);
}

void VideoStreamPlayerAv1::init_playback() {
	if (file_name == "" || video_player.is_null()) return;
	video_player->init_playback();
	video_player->update_frame();
	texture = Ref<Texture2D>(video_player->get_texture());
	queue_redraw();
}

// void VideoStreamPlayerAv1::seek_playback_fast(double playback_time) {
// 	video_player->seek_playback_fast(playback_time);
// }

double VideoStreamPlayerAv1::get_playback_position() const {
	return video_player->get_playback_time();
}

double VideoStreamPlayerAv1::get_playback_duration() const {
	return video_player->get_playback_duration();
}

void VideoStreamPlayerAv1::set_playback_speed(double playback_speed) {
	video_player->set_playback_speed(playback_speed);
}

double VideoStreamPlayerAv1::get_playback_speed() const {
	return video_player->get_playback_speed();
}

void VideoStreamPlayerAv1::set_looping(bool value) {
	loop = value;
}

bool VideoStreamPlayerAv1::is_looping() const {
	return loop;
}

void VideoStreamPlayerAv1::set_file_name(String value) {
	file_name = value;
	video_player->set_file_name(file_name);
	stop();
}

String VideoStreamPlayerAv1::get_file_name() const {
	return file_name;
}

void VideoStreamPlayerAv1::set_expand(bool p_expand) {
	if (expand == p_expand) return;
	expand = p_expand;
	queue_redraw();
	update_minimum_size();
}

bool VideoStreamPlayerAv1::has_expand() const {
	return expand;
}

void VideoStreamPlayerAv1::set_autoplay(bool value) {
	autoplay = value;
}

bool VideoStreamPlayerAv1::has_autoplay() const {
	return autoplay;
}



void VideoStreamPlayerAv1::texture_changed(const Ref<Texture2D> &p_texture) {
	const Size2 new_texture_size = p_texture.is_valid() ? p_texture->get_size() : Size2();

	if (new_texture_size == texture_size) {
		return;
	}

	texture_size = new_texture_size;

	queue_redraw();

	if (!expand) {
		update_minimum_size();
	}
}

Size2 VideoStreamPlayerAv1::get_minimum_size() const {
	if (!expand && texture.is_valid()) {
		return texture_size;
	} else {
		return Size2();
	}
}


void VideoStreamPlayerAv1::_bind_methods() {
		// properties
    ClassDB::bind_method(D_METHOD("set_file_name", "file_name"), &VideoStreamPlayerAv1::set_file_name);
    ClassDB::bind_method(D_METHOD("get_file_name"), &VideoStreamPlayerAv1::get_file_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_name"), "set_file_name", "get_file_name");
    
    // methods
    ClassDB::bind_method(D_METHOD("set_looping", "value"), &VideoStreamPlayerAv1::set_looping);
    ClassDB::bind_method(D_METHOD("is_looping"), &VideoStreamPlayerAv1::is_looping);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_looping", "is_looping");

    ClassDB::bind_method(D_METHOD("play"), &VideoStreamPlayerAv1::play);
    ClassDB::bind_method(D_METHOD("pause"), &VideoStreamPlayerAv1::pause);
    ClassDB::bind_method(D_METHOD("stop"), &VideoStreamPlayerAv1::stop);

		ClassDB::bind_method(D_METHOD("init_playback"), &VideoStreamPlayerAv1::init_playback);
    
		ClassDB::bind_method(D_METHOD("set_playing"), &VideoStreamPlayerAv1::set_playing);
    ClassDB::bind_method(D_METHOD("is_playing"), &VideoStreamPlayerAv1::is_playing);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "is_playing");
    // ClassDB::bind_method(D_METHOD("get_texture"), &VideoStreamPlayerAv1::get_texture);

		ClassDB::bind_method(D_METHOD("set_playback_position", "value"), &VideoStreamPlayerAv1::set_playback_position);
		// ClassDB::bind_method(D_METHOD("seek_playback_fast", "value"), &VideoStreamPlayerAv1::seek_playback_fast);
		ClassDB::bind_method(D_METHOD("get_playback_position"), &VideoStreamPlayerAv1::get_playback_position);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "playback_position"), "set_playback_position", "get_playback_position");
		ClassDB::bind_method(D_METHOD("get_playback_duration"), &VideoStreamPlayerAv1::get_playback_duration);
		
		ClassDB::bind_method(D_METHOD("set_playback_speed", "value"), &VideoStreamPlayerAv1::set_playback_speed);
		ClassDB::bind_method(D_METHOD("get_playback_speed"), &VideoStreamPlayerAv1::get_playback_speed);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "playback_speed"), "set_playback_speed", "get_playback_speed");

		ClassDB::bind_method(D_METHOD("set_autoplay", "value"), &VideoStreamPlayerAv1::set_autoplay);
		ClassDB::bind_method(D_METHOD("has_autoplay"), &VideoStreamPlayerAv1::has_autoplay);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay"), "set_autoplay", "has_autoplay");

		ClassDB::bind_method(D_METHOD("set_expand", "value"), &VideoStreamPlayerAv1::set_expand);
		ClassDB::bind_method(D_METHOD("has_expand"), &VideoStreamPlayerAv1::has_expand);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "expand"), "set_expand", "has_expand");

}

void VideoStreamPlayerAv1::_notification(int p_notification) {
	switch (p_notification) {
		case NOTIFICATION_ENTER_TREE: {
			// if (file_name != "" && autoplay) {
			// 	play();
			// }
		} break;

		case NOTIFICATION_EXIT_TREE: {
			stop();
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			if (file_name == "" || /*!is_playing() ||*/ video_player.is_null()) {
				return;
			}

			video_player->update_frame();
			texture = Ref<Texture2D>(video_player->get_texture());
			queue_redraw();
			// set_texture();

			if (!is_playing()) {
				if (loop) {
					play();
					return;
				}
				// emit_signal(SceneStringName(finished));
			}
		} break;

		case NOTIFICATION_DRAW: {
			if (texture.is_null()) {
				return;
			}
			if (texture->get_width() == 0) {
				return;
			}

			// Size2 s = expand ? get_size() : texture_size;
			Size2 s = texture->get_size();
			draw_texture_rect(texture, Rect2(Point2(), s), false);
		} break;

		// case NOTIFICATION_SUSPENDED:
		case NOTIFICATION_PAUSED: {
			if (is_playing()) {
				// paused_from_tree = true;
				// if (playback.is_valid()) {
					// video_player->set_paused(true);
					pause();
					set_process_internal(false);
				// }
			}
		} break;

		// case NOTIFICATION_UNSUSPENDED: {
		// 	if (get_tree()->is_paused()) {
		// 		break;
		// 	}
		// 	[[fallthrough]];
		// }

		case NOTIFICATION_UNPAUSED: {
			// if (paused_from_tree) {
				// paused_from_tree = false;
				if (file_name != "") {
					// play();
					set_process_internal(true);
				}
			// }
		} break;
	}
}

// void VideoStreamPlayerAv1::_validate_property(PropertyInfo &p_property) const {
	
// };