#include "aether/core/Time.h"
#include <GLFW/glfw3.h>

namespace aether {

double Time::s_LastFrame  = 0.0;
float  Time::s_DeltaTime  = 0.0f;
float  Time::s_Time       = 0.0f;
float  Time::s_FPS        = 0.0f;
int    Time::s_FrameCount = 0;
double Time::s_FPSAccum   = 0.0;

void Time::Init() {
    s_LastFrame  = glfwGetTime();
    s_DeltaTime  = 0.0f;
    s_Time       = 0.0f;
    s_FPS        = 0.0f;
    s_FrameCount = 0;
    s_FPSAccum   = 0.0;
}

void Time::Update() {
    const double now = glfwGetTime();
    s_DeltaTime = static_cast<float>(now - s_LastFrame);
    s_LastFrame = now;
    s_Time      = static_cast<float>(now);

    ++s_FrameCount;
    s_FPSAccum += s_DeltaTime;
    if (s_FPSAccum >= 1.0) {
        s_FPS = static_cast<float>(s_FrameCount) / static_cast<float>(s_FPSAccum);
        s_FrameCount = 0;
        s_FPSAccum  = 0.0;
    }
}

} // namespace aether
