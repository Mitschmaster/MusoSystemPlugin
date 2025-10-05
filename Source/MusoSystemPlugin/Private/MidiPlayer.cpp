#include "MidiPlayer.h"
#include "MusoSubsystem.h"

#include "Kismet/GameplayStatics.h"


UMidiPlayer::UMidiPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<USoundWave> soundObj(TEXT("/Script/Engine.SoundWave'/MusoSystemPlugin/beep.beep'"));
	if (soundObj.Succeeded())
	{
		 DebugSound = soundObj.Object;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to load SoundWave asset at path: /MusoSystemPlugin/beep.beep"));
}

void UMidiPlayer::Initialize(UMusoData* const MusoData)
{
	MidiFile = MusoData->MidiFile;
	if (!MidiFile)
	{
		UE_LOG(LogTemp, Error, TEXT("Midi Player: No Midi File!"));
		return;
	}
	MidiTrackList = &MidiFile->GetTracks();
	MidiTempoMap = &MidiFile->GetSongMaps()->GetTempoMap();
	if (!MidiTrackList->IsValidIndex(MusoData->DefaultMidiTrackNumber))
	{
		UE_LOG(LogTemp, Error, TEXT("Midi Player: Midi Track not found!"));
		return;
	}
	MidiEvents = &(*MidiTrackList)[MusoData->DefaultMidiTrackNumber].GetEvents();
	if (MidiEvents->IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Midi Player: No Midi Events found on Track!"));
		return;
	}
	bQueueStart = false;
	
	UMusoSubsystem* musoSubsystem = UWorld::GetSubsystem<UMusoSubsystem>(GetWorld());
	UQuartzClockHandle* clockHandle = musoSubsystem->GetQuartzClockHandle();
	
	FQuartzQuantizationBoundary quartzQuantizationBoundry;
	quartzQuantizationBoundry.Quantization = EQuartzCommandQuantization::Beat;
	quartzQuantizationBoundry.CountingReferencePoint = EQuarztQuantizationReference::BarRelative;
	quartzQuantizationBoundry.Multiplier = 1.0f;
	quartzQuantizationBoundry.bFireOnClockStart = false;

	//OnQuartzCommandEvent.BindDynamic(this, &UMidiPlayer::OnQuartzCommandEventFunction);

	
	// clockHandle->NotifyOnQuantizationBoundary(
	// 	this,
	// 	quartzQuantizationBoundry,
	// 	OnQuartzCommandEvent		
	// 	);

	FOnQuartzMetronomeEventBP OnQuartzMetronomeEvent;
	OnQuartzMetronomeEvent.BindDynamic(this, &UMidiPlayer::OnQuartzMetronomeEventFunction);
	
	clockHandle->SubscribeToQuantizationEvent(
		this,
		EQuartzCommandQuantization::Bar,
		OnQuartzMetronomeEvent,
		clockHandle);
	

}

float UMidiPlayer::GetMsecToNextNoteOnEvent() const
{
	return NextMidiEventMsec;
}

float UMidiPlayer::GetMsecSincePreviousNoteOnEvent() const
{
	return LastMidiEventMsec;
}

void UMidiPlayer::PlayerLoopFunction()
{
	//if (NextMidiTick == 0)
	//	OnPlayerStart.ExecuteIfBound();

	FMidiEventListWithExtra eventList = GetEventsAtTick(NextMidiTick);
	
	LastMidiTick = NextMidiTick;
	NextMidiTick = GetNextTick();
	
	float msecToNextEvent = 0;
	if (NextMidiTick > LastMidiTick)
		msecToNextEvent = MidiTempoMap->TickToMs(NextMidiTick) - MidiTempoMap->TickToMs(LastMidiTick);
	if (NextMidiTick < LastMidiTick)
		msecToNextEvent = MidiTempoMap->TickToMs(NextMidiTick) +
			MidiTempoMap->TickToMs(MidiFile->GetSongMaps()->GetSongLengthData().LengthTicks) -
			MidiTempoMap->TickToMs(LastMidiTick);
	if (NextMidiTick == LastMidiTick)
		msecToNextEvent = MidiTempoMap->TickToMs(MidiFile->GetSongMaps()->GetSongLengthData().LengthTicks);
	
	if (!eventList.Events.IsEmpty())
	{
		NextMidiEventMsec = msecToNextEvent;
		LastMidiEventMsec = 0;
		BroadcastEvent(eventList.Events);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		PlayerTimerHandle,
		this,
		&UMidiPlayer::PlayerLoopFunction,
		msecToNextEvent / 1000.f,
		false
	);
}

void UMidiPlayer::OnQuartzCommandEventFunction(EQuartzCommandDelegateSubType EventType, FName Name)
{
	UE_LOG(LogTemp, Warning, TEXT("Quartz Command Event: %i"), EventType);
	if (bQueueStart)
		Play();
	bQueueStart = false;
}

void UMidiPlayer::OnQuartzMetronomeEventFunction(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
	UE_LOG(LogTemp, Warning, TEXT("Quartz Command Event Bars: %i"), NumBars);
	if (bQueueStart)
		Play();
	bQueueStart = false;
}

void UMidiPlayer::BroadcastEvent(const FMidiEventList& Events) const
{
	FMusoMidiEvent MusoMidiEvent;
	for (const auto& event : Events )
		MusoMidiEvent.Notes.Add(event.GetMsg().Data1);
	MusoMidiEvent.MsecSincePreviousNoteOnEvent = GetMsecSincePreviousNoteOnEvent();
	MusoMidiEvent.MsecToNextNoteOnEvent = GetMsecToNextNoteOnEvent();

	OnNextMidiEventFromMidiPlayer.ExecuteIfBound(MusoMidiEvent);

	if (bDebugSounds && DebugSound)
	{
		for (const auto& event : Events)
		{
			int32 note = event.GetMsg().Data1 + 24; // Transpose up 2 octaves
			int32 beep_note = 60; // C4
			float pitch = pow(2.0f, (note - beep_note) / 12.0f);
			UGameplayStatics::PlaySound2D(GetWorld(), DebugSound, 1.0f, pitch);
		}
	}
}

void UMidiPlayer::Play()
{
	if (!MidiFile || MidiTrackList->IsEmpty())
		return;
	if(!GetWorld()->GetTimerManager().IsTimerActive(PlayerTimerHandle))
		PlayerLoopFunction();
	if (GetWorld()->GetTimerManager().IsTimerPaused(PlayerTimerHandle))
		GetWorld()->GetTimerManager().UnPauseTimer(PlayerTimerHandle);
}

void UMidiPlayer::Stop()
{
	NextMidiTick = 0;
	LastMidiTick = -1;
	LastMidiEventMsec = 0;
	GetWorld()->GetTimerManager().ClearTimer(PlayerTimerHandle);
}

void UMidiPlayer::Pause()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(PlayerTimerHandle))
		GetWorld()->GetTimerManager().PauseTimer(PlayerTimerHandle);
}

void UMidiPlayer::UnPause()
{
	if (GetWorld()->GetTimerManager().IsTimerPaused(PlayerTimerHandle))
		GetWorld()->GetTimerManager().UnPauseTimer(PlayerTimerHandle);
}

void UMidiPlayer::QueueStart()
{
	bQueueStart = true;
}

FMidiEventListWithExtra UMidiPlayer::GetEventsAtTick(int32 Tick) const
{
	FMidiEventListWithExtra result;
	for (const FMidiEvent& event : (*MidiEvents))
	{
		if (event.GetTick() == Tick)
			result.Events.Add(event);
	}
	result.Tick = Tick;
	return result;
}

int32 UMidiPlayer::GetNextTick() const
{
	int32 result = 0;
	for (const FMidiEvent& event : (*MidiEvents))
	{
		if (event.GetTick() > NextMidiTick)
		{
			result = event.GetTick();
			break;
		}
	}
	return result;
}

void UMidiPlayer::Tick(float DeltaTime)
{
	if (LastMidiEventMsec > 0.0f)
		LastMidiEventMsec += DeltaTime * 1000.f;
	if (NextMidiEventMsec > 0.0f)
		NextMidiEventMsec -= DeltaTime * 1000.f;
	if (NextMidiEventMsec < 0.0f)
		NextMidiEventMsec = 0.0f;
}

bool UMidiPlayer::IsTickable() const
{
	if (IsTemplate(RF_ClassDefaultObject))
	{
		return false;
	}
	return true;
}

TStatId UMidiPlayer::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMusoPlayer, STATGROUP_Tickables);
}