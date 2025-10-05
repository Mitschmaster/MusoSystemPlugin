#pragma once

#include "CoreMinimal.h"
#include "HarmonixMidi/MidiFile.h"

#include "MusoData.generated.h"

USTRUCT(BlueprintType)
struct FMusoAudioParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundWave* SoundWave = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Volume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Pitch = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name = FName::FName(NAME_None);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AudioTrackLength = 0.0f;
};


UCLASS(BlueprintType)
class MUSOSYSTEMPLUGIN_API UMusoData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMusoAudioParam> AudioTracks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMidiFile* MidiFile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DefaultMidiTrackNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeInTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeOutTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PlayerRemainTime = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MidiTrackLength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MusoPlayerLoopLength = 5.0f;
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};