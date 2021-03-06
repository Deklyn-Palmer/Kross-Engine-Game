/*
 *  Author: Chris Deitch.
 *  Editors:
 *      - Chris Deitch.
 *      - Deklyn Palmer.
 */

#include "AudioPlayer.h"
#include "../Manager/AudioManager.h"

#include "../Debug.h"

namespace Kross
{
	AudioPlayer::AudioPlayer() : 
		m_AudioHandle		(0), 
		p_AudioSrc			(nullptr), 
		p_AudioProperties	(KROSS_NEW AudioProperties()), 
		m_IsPlaying			(false), 
		m_Stopped			(true)
	{}

	void AudioPlayer::Play()
	{

	#ifndef KROSS_EDITOR
		/* If the Current Audio Player Doesn't have a Audio Handle. */
		if (!m_AudioHandle)
		{
			/* If the Sound if Streamable. */
			if (p_AudioSrc->IsStreamable())
			{
				//Set the handle that the audioPlayer owns to this source's newly generated handle.
				m_AudioHandle = AudioManager::AudioEngine()->play(*p_AudioSrc->GetWavStream(), p_AudioProperties->GetVolume(), p_AudioProperties->GetPan());

				//collect the handle and add to the audio source as well. This is so the player can recover handles.
				p_AudioSrc->SetHandle(m_AudioHandle);
			}

			/* Play it standardly. */
			else
			{
				//Set the handle that the audioPlayer owns to this source's newly generated handle.
				m_AudioHandle = AudioManager::AudioEngine()->play(*p_AudioSrc->GetWav(), p_AudioProperties->GetVolume(), p_AudioProperties->GetPan());

				//collect the handle and add to the audio source as well. This is so the player can recover handles.
				p_AudioSrc->SetHandle(m_AudioHandle);

			}

			/* Loop the Sound if Properties Allow. */
			AudioManager::AudioEngine()->setLooping(m_AudioHandle, p_AudioProperties->GetLoop());
		}

		/* If we already do that means a sound has been loaded to this Audio Player. */
		else
		{
			if (!m_IsPlaying && !m_Stopped)
			{
				/* Un Pause. */
				AudioManager::AudioEngine()->setPause(m_AudioHandle, false);

				/* Loop the Sound if Properties Allow. */
				AudioManager::AudioEngine()->setLooping(m_AudioHandle, p_AudioProperties->GetLoop());
			}
			else
			{
				Stop();

				/* If the Sound if Streamable. */
				if (p_AudioSrc->IsStreamable())
				{
					//Set the handle that the audioPlayer owns to this source's newly generated handle.
					m_AudioHandle = AudioManager::AudioEngine()->play(*p_AudioSrc->GetWavStream(), p_AudioProperties->GetVolume(), p_AudioProperties->GetPan());

					//collect the handle and add to the audio source as well. This is so the player can recover handles.
					p_AudioSrc->SetHandle(m_AudioHandle);
				}

				/* Play it standardly. */
				else
				{
					//Set the handle that the audioPlayer owns to this source's newly generated handle.
					m_AudioHandle = AudioManager::AudioEngine()->play(*p_AudioSrc->GetWav(), p_AudioProperties->GetVolume(), p_AudioProperties->GetPan());

					//collect the handle and add to the audio source as well. This is so the player can recover handles.
					p_AudioSrc->SetHandle(m_AudioHandle);

				}
			}
		}

		/* Sound is Playing. */
		m_IsPlaying = true;
		m_Stopped = false;
	#endif
	}

	void AudioPlayer::Pause()
	{
		/* If we have a Audio Handle. */
		if (m_AudioHandle)
		{
			AudioManager::AudioEngine()->setPause(m_AudioHandle, true);
		}

		/* No longer Playing. */
		m_IsPlaying = false;
		m_Stopped = false;
	}

	void AudioPlayer::Stop()
	{
		/* If we have a Audio Handle. */
		if (m_AudioHandle)
		{
			if (p_AudioSrc->IsStreamable())
			{
				AudioManager::AudioEngine()->stopAudioSource(*p_AudioSrc->GetWavStream());
			}
			else
			{
				AudioManager::AudioEngine()->stopAudioSource(*p_AudioSrc->GetWav());
			}

			/* Don't know if this is safe. */
			m_AudioHandle = 0;
			p_AudioSrc->SetHandle(0);

		}

		/* No longer Playing. */
		m_IsPlaying = false;
		m_Stopped = true;
	}

	void AudioPlayer::PlaySoundTimeStamped(float seconds)
	{
		/* Early out if the Seconds is in the Negatives. */
		AudioManager::AudioEngine()->getStreamPosition(m_AudioHandle);
		/**/
		if (p_AudioSrc->IsStreamable())
		{
			return;
		}

		AudioManager::AudioEngine()->seek(m_AudioHandle, seconds);
	}

	void AudioPlayer::PauseSoundAfterElapsedTime(float seconds)
	{
		AudioManager::AudioEngine()->schedulePause(m_AudioHandle, seconds);
	}

	void AudioPlayer::SetPlaySpeed(float playSpeed)
	{
		/* Set the Audio Properties. */
		p_AudioProperties->SetPlaySpeed(playSpeed);

		/* If we have a Audio Handle. */
		if (m_AudioHandle)
		{
			AudioManager::AudioEngine()->setRelativePlaySpeed(m_AudioHandle, p_AudioProperties->GetPlaySpeed());
		}

	}

	void AudioPlayer::SetPan(float pan)
	{
		/* Set the Audio Properties. */
		p_AudioProperties->SetPan(pan);

		/* If we have a Audio Handle. */
		if (m_AudioHandle)
		{
			AudioManager::AudioEngine()->setPan(m_AudioHandle, p_AudioProperties->GetPan());
		}
	}

	void AudioPlayer::SetVolume(float volume)
	{
		/* Set the Audio Properties. */
		p_AudioProperties->SetVolume(volume);

		/* If we have a Audio Handle. */
		if (m_AudioHandle)
		{
			float v = AudioManager::AudioEngine()->getVolume(m_AudioHandle);
			Debug::Log(v);
			AudioManager::AudioEngine()->setVolume(m_AudioHandle, glm::clamp(volume, 0.0f, 1.0f));
		}
	}

	void AudioPlayer::SetLoop(bool loop)
	{
		/* Set the Audio Properties. */
		p_AudioProperties->SetLoop(loop);

		/* If we have a Audio Handle. */
		if (m_AudioHandle)
		{
			AudioManager::AudioEngine()->setLooping(m_AudioHandle, p_AudioProperties->GetLoop());
		}
	}

	void AudioPlayer::FadeVolume(float destinedVolume, float secondsToFade)
	{
		AudioManager::AudioEngine()->fadeVolume(m_AudioHandle,destinedVolume,secondsToFade);
	}

	void AudioPlayer::FadePan(float destinedPan, float secondsToFade)
	{
		AudioManager::AudioEngine()->fadePan(m_AudioHandle, destinedPan, secondsToFade);
	}

	void AudioPlayer::FadeRelativePlaySpeed(float destinedSpeed, float secondsToFade)
	{
		AudioManager::AudioEngine()->fadeRelativePlaySpeed(m_AudioHandle, destinedSpeed, secondsToFade);
	}

	void AudioPlayer::OscillateVolume(float minVol, float maxVol, float seconds)
	{
		AudioManager::AudioEngine()->oscillateVolume(m_AudioHandle,minVol, maxVol, seconds);
	}

	void AudioPlayer::OscillatePan(float minPan, float maxPan, float seconds)
	{
		AudioManager::AudioEngine()->oscillatePan(m_AudioHandle, minPan, maxPan, seconds);
	}

	void AudioPlayer::OscillateRelitivePlaySpeed(float minSpeed, float maxSpeed, float seconds)
	{
		AudioManager::AudioEngine()->oscillateRelativePlaySpeed(m_AudioHandle, minSpeed, maxSpeed, seconds);
	}
}


