#pragma once

#include "CoreMinimal.h"
#include "MusoData.h"
#include "PlayerBase.h"

#include "AudioPlayer.generated.h"

USTRUCT()
struct MUSOSYSTEMPLUGIN_API FComponentToLoopMap
{
	GENERATED_BODY()

	FMusoAudioParam	MusoAudioParam;
	
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};

UCLASS()
class MUSOSYSTEMPLUGIN_API UAudioPlayer : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<FComponentToLoopMap> ComponentToLoopMapList;

public:
	void Initialize(UMusoData* MusoData);

	void Play();
	void Stop();
	void Pause();
	void UnPause();
	void FadeIn(float Length);
	void FadeOut(float Length);
	void SetVolume(float Volume);
};
