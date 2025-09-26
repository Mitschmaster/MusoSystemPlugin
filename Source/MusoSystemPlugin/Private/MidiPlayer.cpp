#include "MidiPlayer.h"

void UMidiPlayer::SetMidiFile(UMidiFile* MidiFileIn)
{
	Stop();
	MidiFile = MidiFileIn;
	if(!MidiFile)
		return;
	MidiTrackList = &MidiFile->GetTracks();
	MidiTempoMap = &MidiFile->GetSongMaps()->GetTempoMap();
}

void UMidiPlayer::SetTrackNumber(int32 InTrackNumber)
{
	Stop();
	TrackNumber = InTrackNumber;
}

int32 UMidiPlayer::GetMsecToNextNoteOnEvent() const
{
	return NextMidiEventMsec - GetWorld()->GetTimeSeconds() * 1000.f;
}

int32 UMidiPlayer::GetMsecSincePreviousNoteOnEvent() const
{
	return GetWorld()->GetTimeSeconds() * 1000.f - LastMidiEventMsec;
}

void UMidiPlayer::PlayerFunction()
{
	if (PlayerState != EPlayerState::Playing)
		return;

	FMidiEventListWithExtra nextEvents = GetNextEvents();
	if (nextEvents.Count == 0)
	{
		Stop();
		return;
	}

	float msecToNextEvent = 0;
	if (LastMidiTick > -1)
	{
		if (nextEvents.Tick > LastMidiTick)
			msecToNextEvent = MidiTempoMap->TickToMs(nextEvents.Tick) - MidiTempoMap->TickToMs(LastMidiTick);
		if (nextEvents.Tick < LastMidiTick)
			msecToNextEvent = MidiTempoMap->TickToMs(nextEvents.Tick) +
			MidiTempoMap->TickToMs(MidiFile->GetSongMaps()->GetSongLengthData().LengthTicks) -
			MidiTempoMap->TickToMs(LastMidiTick);
		if (nextEvents.Tick == LastMidiTick)
			msecToNextEvent = MidiTempoMap->TickToMs(MidiFile->GetSongMaps()->GetSongLengthData().LengthTicks);
	}
	else
		msecToNextEvent = FLT_MIN;

	NextMidiEventMsec = GetWorld()->GetTimeSeconds() * 1000.f + msecToNextEvent;
	LastMidiTick = nextEvents.Tick;
	LastMidiEventMsec = GetWorld()->GetTimeSeconds() * 1000.f;
	BroadcastEvent(nextEvents.Events);

	GetWorld()->GetTimerManager().SetTimer(
		PlayerTimerHandle,
		this,
		&UMidiPlayer::PlayerFunction,
		msecToNextEvent / 1000.f,
		false
	);

}

void UMidiPlayer::BroadcastEvent(const FMidiEventList Events) const
{
	FMusoMidiEvent MusoEvent;
	for (const auto& event : Events )
		MusoEvent.Notes.Add(event.GetMsg().Data1);
	MusoEvent.MsecSincePreviousNoteOnEvent = GetMsecSincePreviousNoteOnEvent();
	MusoEvent.MsecToNextNoteOnEvent = GetMsecToNextNoteOnEvent();

	OnNextMidiEvent.Broadcast(MusoEvent);
}

void UMidiPlayer::Play()
{
	if (!MidiFile || MidiTrackList->IsEmpty())
		return;
	PlayerState = EPlayerState::Playing;
	if(!GetWorld()->GetTimerManager().IsTimerActive(PlayerTimerHandle))
		PlayerFunction();
	if (GetWorld()->GetTimerManager().IsTimerPaused(PlayerTimerHandle))
		GetWorld()->GetTimerManager().UnPauseTimer(PlayerTimerHandle);
}

void UMidiPlayer::Stop()
{
	PlayerState = EPlayerState::Stopped;
	NextEventIndex = 0;
	LastMidiTick = -1;
	LastMidiEventMsec = 0;
	GetWorld()->GetTimerManager().ClearTimer(PlayerTimerHandle);
}

void UMidiPlayer::Pause()
{
	PlayerState = EPlayerState::Paused;
	if (GetWorld()->GetTimerManager().IsTimerActive(PlayerTimerHandle))
		GetWorld()->GetTimerManager().PauseTimer(PlayerTimerHandle);
}

const FMidiTrack* UMidiPlayer::GetTrack() const
{
	if (!MidiFile || MidiTrackList->IsEmpty())
		return nullptr;
	return &(*MidiTrackList)[TrackNumber];
}

FMidiEventListWithExtra UMidiPlayer::GetNextEvents()
{
	FMidiEventListWithExtra result;
	result.Tick = -1;
	result.Count = 0;

	const FMidiTrack* midiTrack = GetTrack();
	if (!midiTrack)
		return result;

	const FMidiEventList* midiEvents = &midiTrack->GetEvents();
	int32 CurrentTick = -1;
	bool bFound = false;
	bool bNoNoteOnsInTrack = false;

	while (NextEventIndex <= midiTrack->GetNumEvents())
	{
		if (NextEventIndex >= midiTrack->GetNumEvents())
		{
			NextEventIndex = 0;
			if (!bLooping || bNoNoteOnsInTrack)
			{
				Stop();
				break;
			}
			bNoNoteOnsInTrack = true;
		}

		bool bNoteOn = (*midiEvents)[NextEventIndex].GetMsg().IsNoteOn() && (*midiEvents)[NextEventIndex].GetMsg().Data2 > 0;
		if (bNoteOn && CurrentTick == -1)
		{
			CurrentTick = (*midiEvents)[NextEventIndex].GetTick();
			bFound = true;
		}
		if (bNoteOn && (*midiEvents)[NextEventIndex].GetTick() == CurrentTick)
		{
			result.Events.Add((*midiEvents)[NextEventIndex]);
			result.Tick = (*midiEvents)[NextEventIndex].GetTick();
			result.Count++;
		}
		if ((*midiEvents)[NextEventIndex].GetTick() != CurrentTick && bFound)
		{
			break;
		}
		NextEventIndex++;
	}
	return result;
}
