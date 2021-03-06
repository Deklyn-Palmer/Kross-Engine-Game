/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

#include "../Animation/Animation.h"
#include "Component.h"
#include "Renderer.h"

namespace Kross
{
	/*!
		\attention Cannot modify the Transform if the Object is Static or using Physics.
		\attention Cannot Change Sprite if no Sprite Renderer is found.
	*/
	class KROSS_API Animator : public Component
	{
	private:
		Animation* m_AnimationCurrent;
		std::vector<Animation*> m_Animations;

		/* Keyframe Properties Components. */

		Rigidbody2D* m_Rigidbody;

		SpriteRenderer* m_SpriteRenderer;

		/* ------------------------------- */

	protected:
		/*!
			Gives Object Editor access to all Protected and Private areas of the class.
		*/
		friend class ObjectEditor;

		/*!
			Gives File System access to all Protected and Private areas of the class.
		*/
		friend class FileSystem;

		/*!
			Animator's Start Method.
		*/
		void OnStart() override;

		/*!
			Animator's Update Method.
		*/
		void OnUpdate() override;

	public:
		/*!
			Animator's Constructor.
		*/
		Animator() :
			m_AnimationCurrent	(nullptr),
			m_Animations		(std::vector<Animation*>()),
			m_Rigidbody			(nullptr),
			m_SpriteRenderer	(nullptr)
		{};
		/*!
			Animator's Destructor.
		*/
		~Animator();

		/*!
			Plays the Current Animation.
		*/
		void Play();

		/*!
			Pauses the Current Animation.
		*/
		void Pause();

		/*!
			Stops the Current Animation.
		*/
		void Stop();

		/*!
			Adds an Animation to the Animator.
		*/
		void AttachAnimation(Animation* animation);

		/*!
			Removes an Animation from the Animator.
		   \attention Must be between 0 and Animation List Size - 1!
		*/
		void DetachAnimation(int index);

		/*!
			Removes the Animation from the Animator.
			\attention Is case sensitive!
		*/
		void DetachAnimation(const std::string& name);

		/*!
			Gets the Current Animation from the Animator.
		*/
		Animation* GetCurrentAnimation() const { return m_AnimationCurrent; };

		/*!
			Sets the Current Animation.
			\attention Must be between 0 and Animation List Size - 1!
		*/
		void SetCurrentAnimation(int index);

		/*!
			Sets the Current Animation.
			\attention Is case sensitive!
		*/
		void SetCurrentAnimation(const std::string& name);
	};
}