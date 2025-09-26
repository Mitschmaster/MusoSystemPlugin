#include "MidiReceiver.h"


void IMidiReceiver::SetNextMidiTicks(uint32 Ticks)
{
	TicksToNextMidiEvent = Ticks;
}

bool IMidiReceiver::SubscribeToSubsystem(UObject* Object)
{
	return false;
}
