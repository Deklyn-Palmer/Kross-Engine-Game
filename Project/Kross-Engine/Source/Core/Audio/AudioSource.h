/*
 *  Author: Chris Deitch.
 *  Editors:
 *      - Chris Deitch.
 */
#pragma once
#include "../Core.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

namespace Kross
{
    class KROSS_API AudioSource
    {
    public:
        /*!
            On creation: If this is a sound effect, just a filepath is fine.
            If it needs to be streamed, a bool of "true" is required.
        */
        static AudioSource* OnCreate(const std::string& filepath, const std::string& name, bool isStream = false);

        /*!
            Destroys the Specified Source.
        */
        static void OnDestroy(AudioSource* audioSource);

        /*!
            Gets the Audio Source Name.
        */
        const std::string GetName() const { return m_Name; };

        /*!
            Gets the Audio Source Filepath.
        */
        const std::string GetFilepath() const { return m_Filepath; };

        /*!
            Gets the Audio Source Stream.
        */
        const bool IsStreamable() const { return m_IsStream; };

        /*!
            Gets the Source handle.
        */
        SoLoud::handle GetHandle() const { return m_SourceHandle; };

    protected:
        /*!
            Gives Resource Manager access to all Protected and Private areas of the class.
        */
        friend class ResourceManager;

        /*!
            Gives Audio Player access to all Protected and Private areas of the class.
        */
        friend class AudioPlayer;

        /*!
            Gives Audio Manager access to all Protected and Private areas of the class.
        */
        friend class AudioManager;

        /*!
            Sets the Audio Source Name.
        */
        void SetName(const std::string& name) { m_Name = name; };

        /*!
            Sets the Audio Source Filepath.
        */
        void SetFilepath(const std::string& filepath) { m_Filepath = filepath; };

        /*!
            Sets the Audio Source Stream.
        */
        void SetStream(bool stream) { m_IsStream = stream; };

        /*!
            Sets the Source handle.
        */
        void SetHandle(SoLoud::handle handle) { m_SourceHandle = handle; };

        /*!
            Gets the Wav from SoLoud.
        */
        SoLoud::Wav* GetWav() { return &m_WavFile; };

        /*!
            Gets the Wav Stream from SoLoud.
        */
        SoLoud::WavStream* GetWavStream() { return &m_WavStreamFile; };

    private:
        /*!
            Audio Source Constructor.
        */
        AudioSource() :   
            m_IsStream      (false),
            m_Filepath      (""),
            m_Name          (""),
            m_WavFile       (SoLoud::Wav()),
            m_WavStreamFile (SoLoud::WavStream()),
            m_SourceHandle  (SoLoud::handle())
        {};
        /*!
            Audio Source Dsestructor.
        */
        ~AudioSource()
        {};

        bool m_IsStream;

        std::string m_Filepath;
        std::string m_Name;

        SoLoud::Wav m_WavFile;
        SoLoud::WavStream m_WavStreamFile;

        SoLoud::handle m_SourceHandle;
    };

}