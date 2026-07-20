#include "video_player_soft.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "memory.h"


using namespace godot;

VideoPlayerSoft::VideoPlayerSoft() {
    image = Image::create(1920, 1080, false, Image::FORMAT_RG8);
    image->fill(Color(0.0, 0.0, 0.0, 1.0));
    image_texture = Ref<ImageTexture>(memnew(godot::ImageTexture));
    image_texture->set_image(image);
    texture = image_texture;
}

VideoPlayerSoft::~VideoPlayerSoft() {
    sav1_destroy_context(&context);
    cleanup_sdl(audio_device);
}

void VideoPlayerSoft::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init_playback"), &VideoPlayerSoft::init_playback);
}

int VideoPlayerSoft::initialize_sdl(Sav1Settings settings, SDL_AudioDeviceID *device_p) {
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO);

  // Define audio settings
  SDL_AudioSpec desired = {0};
  desired.freq = settings.frequency;
  desired.format = AUDIO_S16SYS;
  desired.channels = settings.channels;
  desired.callback = NULL;

  // Create audio device
  *device_p = SDL_OpenAudioDevice(NULL, 0, &desired, &audio_spec, 0);
  if (*device_p == 0) {
    return -1;
  }

  return 0;
}

void VideoPlayerSoft::cleanup_sdl(SDL_AudioDeviceID audio_device) {
  SDL_PauseAudioDevice(audio_device, 1);
  SDL_CloseAudioDevice(audio_device);

  SDL_Quit();
}


bool VideoPlayerSoft::create_video_context() {
    if (context.is_initialized) {
        sav1_destroy_context(&context);
    }

    if (file_name.is_empty()) {
        UtilityFunctions::print("VideoSprite: File name is empty.");
        return false;
    }

    CharString char_file_name = file_name.utf8();
    sav1_default_settings(&settings, (char*) char_file_name.get_data());
    settings.desired_pixel_format = SAV1_PIXEL_FORMAT_RGB;
    
    int success = sav1_create_context(&context, &settings);
    if (success < 0) {
        UtilityFunctions::print("Error creating context!");
        return false;
    }

    initialize_sdl(settings, &audio_device);
    SDL_PauseAudioDevice(audio_device, 0);
    
    return true;
}

void VideoPlayerSoft::init_playback() {
    if (!context.is_initialized) {
        create_video_context();
        sav1_start_playback(&context);
        sav1_stop_playback(&context);
    }
}

void VideoPlayerSoft::play() {
    if (!context.is_initialized) {
        if (!create_video_context()) {
            return;
        }
    }

    sav1_start_playback(&context);
    if (stopped) {
        sav1_seek_playback(&context, 0, SAV1_SEEK_MODE_FAST);
        stopped = false;
    }
}

void VideoPlayerSoft::pause() {
    if (!context.is_initialized)
        return;

    sav1_stop_playback(&context);
}

void VideoPlayerSoft::stop() {
    if (!context.is_initialized)
        return;

    sav1_stop_playback(&context);
    stopped = true;
}

bool VideoPlayerSoft::is_playing() {
    if (!context.is_initialized)
        return false;

    int is_paused;
    int success = sav1_is_playback_paused(&context, &is_paused);
    if (success < 0) {
        return false;
    }

    return (is_paused == 0);
}

void VideoPlayerSoft::process_video_frame() {    
    Sav1VideoFrame *sav1_frame;
    sav1_get_video_frame(&context, &sav1_frame);
    if (sav1_frame->pixel_format == SAV1_PIXEL_FORMAT_ORIG) {
        size_t width = sav1_frame->width;
        size_t height = sav1_frame->height;
        if ((width != image->get_width()) || (height != image->get_height()) || image->get_format() != Image::FORMAT_RG8) {
            image = Image::create(width, height, false, Image::FORMAT_RG8);
            image->fill(Color(1.0, 1.0, 1.0, 1.0));
            image_texture->set_image(image);
            texture = image_texture;
        }
        memcpy((void*)image->ptrw(), (void*)sav1_frame->data, width * height * 2);
    }
    else {
        size_t width = sav1_frame->width;
        size_t height = sav1_frame->height;
        if ((width != image->get_width()) || (height != image->get_height()) || image->get_format() != Image::FORMAT_RGB8) {
            image = Image::create(width, height, false, Image::FORMAT_RGB8);
            image->fill(Color(1.0, 1.0, 1.0, 1.0));
            image_texture->set_image(image);
            texture = image_texture;
        }
        memcpy((void*)image->ptrw(), (void*)sav1_frame->data, width * height * 3);
    }
    image_texture->update(image);
}

void VideoPlayerSoft::process_audio_frame() {
    Sav1AudioFrame *sav1_frame;
    sav1_get_audio_frame(&context, &sav1_frame);
    SDL_QueueAudio(audio_device, sav1_frame->data, sav1_frame->size);
}

void VideoPlayerSoft::update_frame() {
    if (!context.is_initialized)
         return;

    // if (!is_playing()) 
    //     return;
    
    int isAtEnd = 0;
    sav1_is_playback_at_file_end(&context, &isAtEnd);
    if (isAtEnd) {
        if (is_looping()) {
            sav1_seek_playback(&context, 0, SAV1_SEEK_MODE_FAST);
            return;
        }
        else {
            stop();
        }
    }

    // video frame
    int video_frame_ready;
    int success = sav1_get_video_frame_ready(&context, &video_frame_ready);
    if (success < 0) 
        UtilityFunctions::print("Error getting frame!");

    if (video_frame_ready) {
        process_video_frame();
    }

    // audio frame
    int audio_frame_ready;
    sav1_get_audio_frame_ready(&context, &audio_frame_ready);
    if (audio_frame_ready) {
        process_audio_frame();
    }
}

void VideoPlayerSoft::set_playback_time(double playback_time) {
    if (!context.is_initialized) return;
    // if (is_playing()) {
    //  pause();
    // }
    uint64_t time_ms = playback_time * 1000;
    seek_target = time_ms;
    must_seek = true;
    seek_mode = SAV1_SEEK_MODE_PRECISE;
    // int err = sav1_seek_playback(&context, time_ms, SAV1_SEEK_MODE_PRECISE);
    // if (err) {
    //  UtilityFunctions::print("Seeking failed");
    // }
}

void VideoPlayerSoft::seek_playback_fast(double playback_time) {
    if (!context.is_initialized) return;
    // if (is_playing()) {
    //  pause();
    // }
    uint64_t time_ms = playback_time * 1000;
    seek_target = time_ms;
    // must_seek = true;
    // seek_mode = SAV1_SEEK_MODE_FAST;
    int err = sav1_seek_playback(&context, time_ms, SAV1_SEEK_MODE_FAST);
    if (err) {
        // sav1_seek_playback(&context, time_ms, SAV1_SEEK_MODE_FAST);
        // UtilityFunctions::print("Seeking failed");
        UtilityFunctions::print(sav1_get_error(&context));
    }
}

double VideoPlayerSoft::get_playback_time() {
    if (!context.is_initialized) return 0.0;
    uint64_t time_ms;
    int err = sav1_get_playback_time(&context, &time_ms);
    if (err) {
        UtilityFunctions::print("Failed to get playback time");
        return 0.0;
    }
    double time = time_ms * 0.001;
    return time;
}

double VideoPlayerSoft::get_playback_duration() {
    if (!context.is_initialized) return 0.0;
    uint64_t time_ms;
    int err = sav1_get_playback_duration(&context, &time_ms);
    if (err) {
        UtilityFunctions::print("Failed to get duration");
        return 0.0;
    }
    double time = time_ms * 0.001;
    return time;
}

void VideoPlayerSoft::set_playback_speed(double playback_speed) {
    if (!context.is_initialized) return;
    int err = sav1_set_playback_speed(&context, playback_speed);
    if (err) {
        UtilityFunctions::print("Failed to set playback speed");
    }
}

double VideoPlayerSoft::get_playback_speed() {
    if (!context.is_initialized) return 1.0;
    double playback_speed;
    int err = sav1_get_playback_speed(&context, &playback_speed);
    if (err) {
        UtilityFunctions::print("Failed to get playback speed");
        return 1.0;
    }
    return playback_speed;
}

