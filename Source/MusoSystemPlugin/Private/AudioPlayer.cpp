#include "AudioPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void UAudioPlayer::AddMusoAudioLoop(FMusoAudioParam AudioParam)
{
	if (!AudioParam.SoundWave)
		return;

	UAudioComponent* component = UGameplayStatics::CreateSound2D(
		this,
		AudioParam.SoundWave,
		AudioParam.Volume,
		AudioParam.Pitch,
		0.f,
		nullptr,
		false,
		true
	);

	if (component)
	{
		FComponentToLoopMap componentToLoopMap;
		componentToLoopMap.MusoAudioParam = AudioParam;
		componentToLoopMap.AudioComponent = component;

		ComponentToLoopMapList.Add(componentToLoopMap);
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