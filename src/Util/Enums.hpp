#pragma once

enum class API { OPENGL, UNDEFINED };

enum class WindowMode {
    WINDOWED,
    FULLSCREEN,
};

enum class Events { NONE, RELOAD_SHADERS, ENABLE_AUTO_RELOAD_SHADERS };
