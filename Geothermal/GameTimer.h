//********************************************************* 
// 
// Copyright (c) Microsoft. All rights reserved. 
// This code is licensed under the MIT License (MIT). 
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY 
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR 
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT. 
// 
//*********************************************************

// This class is copied from microsoft Simple3DGameDX tutorial

#pragma once

namespace Geothermal
{
    class GameTimer
    {
    public:
        GameTimer();

        /// <summary>
        /// Return the Elapsed time the Game has been active in seconds since Reset
        /// </summary>
        float PlayingTime();
        /// <summary>
        /// Set the Elapsed playing time -- used for restarting in the middle of a game
        /// </summary>
        void PlayingTime(float time);
        /// <summary>
        /// Return the Delta time between the last two updates
        /// </summary>
        float DeltaTime();

        void Reset();
        void Start();
        void Stop();
        void Update();
        bool Active() { return m_active; };

    private:
        float m_secondsPerCount;  // 1.0 / Frequency
        float m_deltaTime;

        LARGE_INTEGER m_baseTime;
        LARGE_INTEGER m_pausedTime;
        LARGE_INTEGER m_stopTime;
        LARGE_INTEGER m_previousTime;
        LARGE_INTEGER m_currentTime;

        bool m_active;
    };
}
