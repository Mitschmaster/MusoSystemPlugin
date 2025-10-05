#include "MusoSubsystem.h"


void UMusoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMusoSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMusoSubsystem::EnterMusoArea(UMusoData* MusoData)
{
	RemoveFadeMusoPlayers();
	UMusoPlayer* musoPlayer = NewObject<UMusoPlayer>(this);
	musoPlayer->OnNextMusoMidiEventFromMusoPlayer.BindUObject(this, &UMusoSubsystem::OnNextMusoPlayerEvent);
	musoPlayer->OnPlayerFadedOut.BindUObject(this, &UMusoSubsystem::RemovePlayer);
	musoPlayer->Initialize(MusoData);
	musoPlayer->Play();
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