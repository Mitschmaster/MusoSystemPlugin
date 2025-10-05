//#include "MusoPlayerManager.h"
//
//void MusoPlayerManager::EnterMusoArea(UMusoData* MusoData)
//{
//	if (!MusoData)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MusoPlayerManager::EnterMusoArea: MusoData is null"));
//		return;
//	}
//	for (const FPlayerProperties& Player : MusoPlayers)
//	{
//		if (Player.MusoData == MusoData)
//		{
//
//			Player.Player->FadeIn(MusoData->FadeInTime);
//			return;
//		}
//	}
//	FPlayerProperties NewPlayerProperties;
//	NewPlayerProperties.MusoData = MusoData;
//	NewPlayerProperties.Player = NewObject<UMusoPlayer>();
//	NewPlayerProperties.Player->Initialize(MusoData);
//	NewPlayerProperties.Player->FadeIn(MusoData->FadeInTime);
//	MusoPlayers.Add(NewPlayerProperties);
//
//}