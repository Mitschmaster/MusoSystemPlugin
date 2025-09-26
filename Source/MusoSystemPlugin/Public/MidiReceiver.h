#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "MidiReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UMidiReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MUSOSYSTEMPLUGIN_API IMidiReceiver
{
	GENERATED_BODY()

public:
	uint32 TicksToNextMidiEvent;
	virtual void SetNextMidiTicks(uint32 Ticks);
	virtual bool SubscribeToSubsystem(UObject* Object);
	
};
