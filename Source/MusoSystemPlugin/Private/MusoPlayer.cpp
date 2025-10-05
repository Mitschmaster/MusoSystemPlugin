#include "MusoPlayer.h"
#include "TimerManager.h"


UMusoPlayer::UMusoPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MidiPlayer = CreateDefaultSubobject<UMidiPlayer>(TEXT("Midi Player"));
	AudioPlayer = CreateDefaultSubobject<UAudioPlayer>(TEXT("Audio Player"));

	MidiPlayer->OnNextMidiEventFromMidiPlayer.BindUObject(this, &UMusoPlayer::OnNextMidiEventFromMidiPlayer);
}

void UMusoPlayer::OnNextMidiEventFromMidiPlayer(const FMusoMidiEvent& MusoMidiEvent) const
{
	OnNextMusoMidiEventFromMusoPlayer.ExecuteIfBound(MusoMidiEvent);
}

void UMusoPlayer::Initialize(UMusoData* InMusoData)
{
	for (const FMusoAudioParam& audioParam : InMusoData->AudioTracks)
	{
		AudioPlayer->Initialize(InMusoData);
	}
	if (InMusoData->MidiFile)
		MidiPlayer->Initialize(InMusoData);
	MusoData = InMusoData;
	CurrentVolume = 1.0f;
	TargetVolume = 1.0f;
}

void UMusoPlayer::Play()
{
	if (PlayerState == EPlayerState::Playing)
	{
		MidiPlayer->Stop();
		MidiPlayer->Play();
		AudioPlayer->Play();
		GetWorld()->GetTimerManager().SetTimer(
			AudioLoopTimer,
			this,
			&UMusoPlayer::Play,
			MusoData->MusoPlayerLoopLength,
			false
		);
	}
	if (PlayerState == EPlayerState::Paused)
	{
		MidiPlayer->UnPause();
		AudioPlayer->UnPause();
		GetWorld()->GetTimerManager().UnPauseTimer(AudioLoopTimer);
	}
	if (PlayerState == EPlayerState::Stopped)
	{
		MidiPlayer->Play();
		AudioPlayer->Play();
		GetWorld()->GetTimerManager().SetTimer(
			AudioLoopTimer,
			this,
			&UMusoPlayer::Play,
			MusoData->MusoPlayerLoopLength,
			false
		);
	}
	
	CancelDestruction();
	PlayerState = EPlayerState::Playing;
}

void UMusoPlayer::Stop()
{
	MidiPlayer->Stop();
	AudioPlayer->Stop();
	CancelDestruction();
	GetWorld()->GetTimerManager().ClearTimer(AudioLoopTimer);
	PlayerState = EPlayerState::Stopped;
}

void UMusoPlayer::Pause()
{
	MidiPlayer->Pause();
	AudioPlayer->Pause();
	CancelDestruction();
	GetWorld()->GetTimerManager().PauseTimer(AudioLoopTimer);
	PlayerState = EPlayerState::Paused;
}

void UMusoPlayer::FadeIn()
{
	TargetVolume = 1.0f;
	CancelDestruction();
}

void UMusoPlayer::FadeOut()
{
	TargetVolume = 0.0f;
	CancelDestruction();
	StartDestructionTimer();
}

float UMusoPlayer::GetMsecToNextNoteOnEvent() const
{
	if (MidiPlayer)
	{
		return MidiPlayer->GetMsecToNextNoteOnEvent();
	}
	return -1.f;
}

float UMusoPlayer::GetMsecSincePreviousNoteOnEvent() const
{
	if (MidiPlayer)
	{
		return MidiPlayer->GetMsecSincePreviousNoteOnEvent();
	}
	return -1.f;
}

void UMusoPlayer::SetDebugSounds(bool bDebugSoundsIn)
{
	if (MidiPlayer)
	{
		MidiPlayer->SetDebugSounds(bDebugSoundsIn);
	}
}

void UMusoPlayer::StartDestructionTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		FadeOutTimerHandle,
		this,
		&UMusoPlayer::DestructionComplete,
		MusoData->FadeOutTime + MusoData->PlayerRemainTime,
		false
	);
}

void UMusoPlayer::DestructionComplete()
{
	MidiPlayer->Stop();
	AudioPlayer->Stop();
	OnPlayerFadedOut.ExecuteIfBound(this);
}

void UMusoPlayer::CancelDestruction()
{
	GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
} 

void UMusoPlayer::Tick(float DeltaTime)
{
	if (CurrentVolume < TargetVolume)
	{
		CurrentVolume += DeltaTime / MusoData->FadeInTime;
		if (CurrentVolume >= TargetVolume)
		{
			CurrentVolume = TargetVolume;
		}
	}
	if (CurrentVolume > TargetVolume)
	{
		CurrentVolume -= DeltaTime / MusoData->FadeOutTime;
		if (CurrentVolume <= TargetVolume)
		{
			CurrentVolume = TargetVolume;
		}
	}
	AudioPlayer->SetVolume(CurrentVolume);
}

bool UMusoPlayer::IsTickable() const
{
	if (IsTemplate(RF_ClassDefaultObject))
	{
		return false;
	}
	return true;
}

TStatId UMusoPlayer::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMusoPlayer, STATGROUP_Tickables);
}
