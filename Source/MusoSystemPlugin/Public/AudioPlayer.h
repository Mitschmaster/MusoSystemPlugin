#pragma once

#include "CoreMinimal.h"

#include "AudioPlayer.generated.h"

USTRUCT(BlueprintType)
struct FMusoAudioParam
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (Required))
	USoundWave* SoundWave = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float Volume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float Pitch = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	FName name = FName::FName(NAME_None);
};

struct FComponentToLoopMap
{
	FMusoAudioParam	MusoAudioParam;
	UAudioComponent* AudioComponent;
};

UCLASS(Blueprintable)
class MUSOSYSTEMPLUGIN_API UAudioPlayer :public UObject
{
	GENERATED_BODY()

public:
	using FComponentToLoopMapList = TArray<FComponentToLoopMap>;

private:
	FComponentToLoopMapList ComponentToLoopMapList;

public:
	UFUNCTION(BlueprintCallable)
	void AddMusoAudioLoop(FMusoAudioParam AudioParam);

	UFUNCTION(BlueprintCallable)
	void Play();
};
