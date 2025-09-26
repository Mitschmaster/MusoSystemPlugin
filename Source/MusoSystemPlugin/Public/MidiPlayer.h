#pragma once

#include "CoreMinimal.h"
#include "HarmonixMidi/MidiFile.h"

#include "MidiPlayer.generated.h"


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Stopped,
	Paused,
	Playing
};

USTRUCT(BlueprintType)
struct FMusoMidiEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> Notes;
	UPROPERTY(BlueprintReadOnly)
	int32 MsecSincePreviousNoteOnEvent;
	UPROPERTY(BlueprintReadOnly)
	int32 MsecToNextNoteOnEvent;
};

struct FMidiEventListWithExtra
{
	FMidiEventList Events;
	int32 Tick;
	int32 Count;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextMidiEvent, FMusoMidiEvent, MusoMidiEvent);

UCLASS(Blueprintable)
class MUSOSYSTEMPLUGIN_API UMidiPlayer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMidiFile(UMidiFile* MidiFileIn);

	UFUNCTION(BlueprintCallable)
	void SetTrackNumber(int32 TrackNumber);

	UFUNCTION(BlueprintCallable)
	void Play();

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void Pause();
	
	UFUNCTION(BlueprintCallable)
	int32 GetMsecToNextNoteOnEvent() const;

	UFUNCTION(BlueprintCallable)
	int32 GetMsecSincePreviousNoteOnEvent() const;

	UFUNCTION(BlueprintCallable)
	EPlayerState GetPlayerState() const { return PlayerState; }

	UFUNCTION(BlueprintCallable)
	void SetLooping(bool bLoop) { bLooping = bLoop; }

	UFUNCTION(BlueprintCallable)
	bool IsLooping() const { return bLooping; }

	UPROPERTY(BlueprintAssignable)
	FOnNextMidiEvent OnNextMidiEvent;

private:
	UPROPERTY()
	UMidiFile* MidiFile = nullptr;

	const UMidiFile::FMidiTrackList* MidiTrackList;
	const FTempoMap* MidiTempoMap;

	int32 TrackNumber = 0;
	int32 NextEventIndex = 0;
	int32 LastMidiTick = -1;
	double LastMidiEventMsec = 0;
	double NextMidiEventMsec = 0;

	EPlayerState PlayerState = EPlayerState::Stopped;
	bool bLooping = false;
	
	FTimerHandle PlayerTimerHandle;

	UFUNCTION()
	void PlayerFunction();

	void BroadcastEvent(const FMidiEventList Events) const;
	const FMidiTrack* GetTrack() const;
	FMidiEventListWithExtra GetNextEvents();
};
