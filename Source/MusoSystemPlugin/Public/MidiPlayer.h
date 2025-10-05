#pragma once

#include "CoreMinimal.h"
#include "HarmonixMidi/MidiFile.h"
#include "Sound/SoundWave.h"
#include "PlayerBase.h"
#include "MusoData.h"

#include "MidiPlayer.generated.h"


USTRUCT(BlueprintType)
struct MUSOSYSTEMPLUGIN_API FMusoMidiEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> Notes;
	UPROPERTY(BlueprintReadOnly)
	int32 MsecSincePreviousNoteOnEvent = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 MsecToNextNoteOnEvent = 0;
};

struct FMidiEventListWithExtra
{
	FMidiEventList Events;
	int32 Tick;
};

DECLARE_DELEGATE_OneParam(FOnNextMidiEventFromMidiPlayer, const FMusoMidiEvent&);
DECLARE_DELEGATE(FOnPlayerStart);

UCLASS(Blueprintable)
class MUSOSYSTEMPLUGIN_API UMidiPlayer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UMidiPlayer(const FObjectInitializer& ObjectInitializer);
	void Initialize(UMusoData* const MusoData);

	void Play();
	void Stop();
	void Pause();
	void UnPause();

	float GetMsecToNextNoteOnEvent() const;
	float GetMsecSincePreviousNoteOnEvent() const;
	
	void SetDebugSounds(bool bDebugSoundsIn) { bDebugSounds = bDebugSoundsIn; }

	FOnNextMidiEventFromMidiPlayer OnNextMidiEventFromMidiPlayer;
	FOnPlayerStart OnPlayerStart;
	
	bool bLooping = false;

private:
	UPROPERTY()
	UMidiFile* MidiFile = nullptr;

	const UMidiFile::FMidiTrackList* MidiTrackList;
	const FTempoMap* MidiTempoMap;
	const FMidiEventList* MidiEvents;
	
	int32 NextMidiTick = 0;
	int32 LastMidiTick = -1;
	double LastMidiEventMsec = -1;
	double NextMidiEventMsec = -1;
	
	FTimerHandle PlayerTimerHandle;

	void PlayerLoopFunction();

	void BroadcastEvent(const FMidiEventList& Events) const;
	FMidiEventListWithExtra GetEventsAtTick(int32 Tick) const;
	int32 GetNextTick() const;

	UPROPERTY()
	USoundWave* DebugSound = nullptr;

	bool bDebugSounds = false;

	// FTickableGameObject
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableInEditor() const override { return false; };
};
