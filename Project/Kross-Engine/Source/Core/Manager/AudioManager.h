/*
 *  Author: Chris Deitch.
 *  Editors:
 *      - Chris Deitch.
 */

#pragma once

#include "../Core.h"

#include "../Component/AudioPlayer.h"


#define WITH_WINMM
#include "soloud.h"
#include "soloud_wav.h"

namespace Kross
{
    class KROSS_API AudioManager
    {
    private:
       
        static AudioManager* s_Instance;
        SoLoud::Soloud* p_Soloud;


    protected:
        friend class AudioSource;
        
        // Gets the Audio Engine.
        static SoLoud::Soloud* GetAudioEngine() { return s_Instance->p_Soloud; };

    public:
        // Creates an Instance of the Manager.
        static void OnCreate();
         
        //inits soloud.
        static void OnStart();

        //De-inits the soloud manager. DOES NOT DELETE.
        static void OnShutdown();

        //deinits the soloud, AND Destroys an Instance of the Manager.
        static void OnDestroy();

        //Attaches the player's SoLoudReference. Do not call from the manager, call from the player on creation.
        static void OnAttachReference(AudioPlayer* player);


        //Returns SoLoud instance.
        SoLoud::Soloud* ReturnSoLoudReference() { return p_Soloud; };

        //"Sets global volume. 1 is normal, 0 is mute.
        void SetGlobalVolume(float volume);

        //Get global volume, returns current vol, 1 is normal, 0 is mute.
        float GetGlobalVolume();




    };
}