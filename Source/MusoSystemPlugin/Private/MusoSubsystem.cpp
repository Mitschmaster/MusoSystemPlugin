#include "MusoSubsystem.h"
#include "Quartz/QuartzSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"

void UMusoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	MidiPlayer = NewObject<UMidiPlayer>(this);
	AudioPlayer = NewObject<UAudioPlayer>(this);
}

void UMusoSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
