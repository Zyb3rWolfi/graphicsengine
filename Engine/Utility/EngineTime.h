//
// Created by Zyb3r on 18/03/2026.
//

#ifndef ENGINE_TIME_H
#define ENGINE_TIME_H



class EngineTime {
public:
    // Call this ONCE at the start of your while loop
    static void Update();

    // Use this anywhere else to get the frame time
    static float GetDeltaTime() { return m_DeltaTime; }
    static float GetCurrentTime() { return m_CurrentFrame; }

private:
    static float m_LastFrame;
    static float m_CurrentFrame;
    static float m_DeltaTime;
};
#endif //TIME_H
