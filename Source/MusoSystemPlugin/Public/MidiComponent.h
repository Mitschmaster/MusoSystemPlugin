#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HarmonixMidi/MidiFile.h"

#include "MidiComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSOSYSTEMPLUGIN_API UMidiComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMidiComponent();
	virtual void InitializeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void OnNextMidiEvent(int32 Ticks);

public:
	UFUNCTION(BlueprintCallable, Category = "MusoTest")
	void SetMidiFile(UMidiFile* MidiFile);
};
