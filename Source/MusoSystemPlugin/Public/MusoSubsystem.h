#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"

#include "MidiPlayer.h"
#include "AudioPlayer.h"

#include "MusoSubsystem.generated.h"


UCLASS(Blueprintable, meta = (DisplayName = "MusoSubsystem"))
class MUSOSYSTEMPLUGIN_API UMusoSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	UPROPERTY(BlueprintReadOnly, Category = "Systems")
	UMidiPlayer* MidiPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Systems")
	UAudioPlayer* AudioPlayer;
};
