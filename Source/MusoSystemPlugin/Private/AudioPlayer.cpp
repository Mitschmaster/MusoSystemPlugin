#include "AudioPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void UAudioPlayer::Initialize(UMusoData* MusoData)
{
	for (const FMusoAudioParam& audioParam : MusoData->AudioTracks)
	{
		UAudioComponent* component = UGameplayStatics::CreateSound2D(
			this,
			audioParam.SoundWave,
			audioParam.Volume,
			audioParam.Pitch,
			0.f,
			nullptr,
			false,
			false
		);

		if (component)
		{
			component->bIsUISound = false;
			FComponentToLoopMap componentToLoopMap;
			componentToLoopMap.MusoAudioParam = audioParam;
			componentToLoopMap.AudioComponent = component;

			ComponentToLoopMapList.Add(componentToLoopMap);
		}
	}
}

void UAudioPlayer::Play()
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->Play();
		}
	}
}

void UAudioPlayer::Stop()
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->Stop();
		}
	}
}

void UAudioPlayer::Pause()
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->SetPaused(true);
		}
	}
}

void UAudioPlayer::UnPause()
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->SetPaused(false);
		}
	}
}

void UAudioPlayer::FadeIn(float Length)
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->FadeIn(Length);
		}
	}
}

void UAudioPlayer::FadeOut(float Length)
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->FadeOut(Length, 0.0f);
		}
	}
}

void UAudioPlayer::SetVolume(float Volume)
{
	for (const FComponentToLoopMap& componentToLoopMap : ComponentToLoopMapList)
	{
		if (componentToLoopMap.AudioComponent)
		{
			componentToLoopMap.AudioComponent->SetVolumeMultiplier(Volume);
		}
	}
}
