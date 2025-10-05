#include "MusoSubsystem.h"
#include "AudioDeviceManager.h"

void UMusoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// UQuartzSubsystem* quartzSubsystem = Collection.InitializeDependency<UQuartzSubsystem>();
	// if (!quartzSubsystem)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Quartz Subsystem not found."));
	// 	return;
	// }
	// FName ClockName = FName(TEXT("Uniquexyz"));//MusoPluginConstants::QuartzClockName();
	//
	// FQuartzClockSettings ClockSettings;
	// ClockSettings.TimeSignature.NumBeats = 4;
	// ClockSettings.TimeSignature.BeatType = EQuartzTimeSignatureQuantization::QuarterNote;
	//
	// QuartzClockHandle = quartzSubsystem->CreateNewClock(GetWorld(), ClockName, ClockSettings, true);
	// UE_LOG(LogTemp, Error, TEXT("Latency: %f"), quartzSubsystem->GetAudioRenderThreadToGameThreadAverageLatency());
	// UE_LOG(LogTemp, Error, TEXT("World: %p"), GetWorld());
	// QuartzClockHandle->StartClock(GetWorld(),QuartzClockHandle);
}

void UMusoSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMusoSubsystem::PostInitialize()
{
	Super::PostInitialize();
	//
	// UQuartzSubsystem* quartzSubsystem = GetWorld()->GetSubsystem<UQuartzSubsystem>();
	// if (!quartzSubsystem)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Quartz Subsystem not found."));
	// 	return;
	// }
	// FName ClockName = FName(TEXT("Uniquexyz"));//MusoPluginConstants::QuartzClockName();
	//
	// FQuartzClockSettings ClockSettings;
	// ClockSettings.TimeSignature.NumBeats = 4;
	// ClockSettings.TimeSignature.BeatType = EQuartzTimeSignatureQuantization::QuarterNote;
	//
	// QuartzClockHandle = quartzSubsystem->CreateNewClock(GetWorld(), ClockName, ClockSettings, true);
	// UE_LOG(LogTemp, Error, TEXT("Latency: %f"), quartzSubsystem->GetAudioRenderThreadToGameThreadAverageLatency());
	// UE_LOG(LogTemp, Error, TEXT("World: %p"), GetWorld());
}

void UMusoSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	UE_LOG(LogTemp, Error, TEXT("InWorld: %p"), &InWorld);

	UE_LOG(LogTemp, Error, TEXT("World: %p"), GetWorld());
	if (GEngine && GEngine->GetAudioDeviceManager() && GEngine->GetAudioDeviceManager()->GetMainAudioDeviceRaw())
	{
		UE_LOG(LogTemp, Error, TEXT("Audio Ready"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Audio NOT Ready"));
	}
	
	UQuartzSubsystem* Quartz = GetWorld()->GetSubsystem<UQuartzSubsystem>();
	if (!Quartz)
	{
		UE_LOG(LogTemp, Error, TEXT("Quartz Subsystem not found."));
		return;
	}
	FName ClockName = FName(TEXT("Uniquexyz"));//MusoPluginConstants::QuartzClockName();
	
	FQuartzClockSettings ClockSettings;
	ClockSettings.TimeSignature.NumBeats = 4;
	ClockSettings.TimeSignature.BeatType = EQuartzTimeSignatureQuantization::QuarterNote;
	
	QuartzClockHandle = Quartz->CreateNewClock(GetWorld(), ClockName, ClockSettings, true);
	QuartzClockHandle->StartClock(GetWorld(),QuartzClockHandle);
}

void UMusoSubsystem::EnterMusoArea(UMusoData* MusoData)
{
	RemoveFadeMusoPlayers();
	UMusoPlayer* musoPlayer = NewObject<UMusoPlayer>(this);
	musoPlayer->OnNextMusoMidiEventFromMusoPlayer.BindUObject(this, &UMusoSubsystem::OnNextMusoPlayerEvent);
	musoPlayer->OnPlayerFadedOut.BindUObject(this, &UMusoSubsystem::RemovePlayer);
	musoPlayer->Initialize(MusoData);
	//musoPlayer->Play();
	MusoPlayers.Add(musoPlayer);
}

void UMusoSubsystem::RemoveFadeMusoPlayers()
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->FadeOut();
	}
}

void UMusoSubsystem::Play()
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->Play();
	}
}

void UMusoSubsystem::Stop()
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->Stop();
	}
}

void UMusoSubsystem::Pause()
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->Pause();
	}
}

void UMusoSubsystem::FadeIn()
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->FadeIn();
	}
}

void UMusoSubsystem::FadeOut()
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->FadeOut();
	}
}

void UMusoSubsystem::OnNextMusoPlayerEvent(const FMusoMidiEvent& MusoMidiEvent) const
{
	OnNextMidiEvent.Broadcast(MusoMidiEvent);
}

UQuartzClockHandle* UMusoSubsystem::GetQuartzClockHandle() const
{
	return QuartzClockHandle;
}

void UMusoSubsystem::RemovePlayer(UMusoPlayer* Player)
{
	Player->Stop();
	MusoPlayers.Remove(Player);
}

void UMusoSubsystem::SetDebugSounds(bool bDebugSoundsIn)
{
	for (UMusoPlayer* player : MusoPlayers)
	{
		player->SetDebugSounds(bDebugSoundsIn);
	}
}