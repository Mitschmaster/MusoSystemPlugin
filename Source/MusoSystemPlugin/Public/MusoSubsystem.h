#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MusoPlayer.h"

#include "MusoSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextMidiEvent, const FMusoMidiEvent&, MusoMidiEvent);


UCLASS()
class MUSOSYSTEMPLUGIN_API UMusoSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintReadOnly, Category = "Muso Subsystem")
	TArray<UMusoPlayer*> MusoPlayers;

	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void EnterMusoArea(UMusoData* MusoData);

	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void RemoveFadeMusoPlayers();

	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void Play();

	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void Stop();

	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void FadeIn();
	
	UFUNCTION(BlueprintCallable, Category = "Muso Subsystem")
	void FadeOut();

	UFUNCTION(BlueprintCallable, Category = "Muso Player")
	void SetDebugSounds(bool bDebugSoundsIn);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNextMidiEvent OnNextMidiEvent;

	void OnNextMusoPlayerEvent(const FMusoMidiEvent& MusoMidiEvent) const;

private:
	void RemovePlayer(UMusoPlayer* Player);
};
