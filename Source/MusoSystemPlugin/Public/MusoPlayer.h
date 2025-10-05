#pragma once

#include "CoreMinimal.h"

#include "MidiPlayer.h"
#include "AudioPlayer.h"
#include "MusoData.h"

#include "MusoPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Stopped,
	Paused,
	Playing,
};

DECLARE_DELEGATE_OneParam(FOnNextMusoMidiEventFromMusoPlayer, const FMusoMidiEvent&);
DECLARE_DELEGATE_OneParam(FOnPlayerFadedOut, UMusoPlayer*);


UCLASS()
class MUSOSYSTEMPLUGIN_API UMusoPlayer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UMusoPlayer(const FObjectInitializer& ObjectInitializer);
	void Initialize(UMusoData* InMusoData);

private:
	UPROPERTY()
	UMidiPlayer* MidiPlayer;

	UPROPERTY()
	UAudioPlayer* AudioPlayer;

	UPROPERTY()
	UMusoData* MusoData;

	EPlayerState PlayerState = EPlayerState::Stopped;

public:
	FOnNextMusoMidiEventFromMusoPlayer OnNextMusoMidiEventFromMusoPlayer;
	FOnPlayerFadedOut OnPlayerFadedOut;

	void OnNextMidiEventFromMidiPlayer(const FMusoMidiEvent& MusoMidiEvent) const;
	
	void Play();
	void Stop();
	void Pause();
	void FadeIn();
	void FadeOut();

	float GetMsecToNextNoteOnEvent() const;
	float GetMsecSincePreviousNoteOnEvent() const;
	
	void SetDebugSounds(bool bDebugSoundsIn);

private:
	FTimerHandle FadeOutTimerHandle;
	void StartDestructionTimer();
	void DestructionComplete();
	void CancelDestruction();

	// FTickableGameObject
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableInEditor() const override { return false; };

private:
	float CurrentVolume = 1.0f;
	float TargetVolume = 1.0f;
	
	FTimerHandle AudioLoopTimer;
};
