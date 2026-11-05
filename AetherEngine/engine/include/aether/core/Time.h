#pragma once

namespace aether {

class Time {
public:
    static void Init();
    static void Update();

    static float GetDeltaTime() { return s_DeltaTime; }
    static float GetTime()      { return s_Time; }
    static float GetFPS()       { return s_FPS; }

private:
    static double s_LastFrame;
    static float  s_DeltaTime;
    static float  s_Time;
    static float  s_FPS;
    static int    s_FrameCount;
    static double s_FPSAccum;
};

} // namespace aether
