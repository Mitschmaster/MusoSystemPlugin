//#pragma once
//
//#include "CoreMinimal.h"
//#include "MusoPlayer.h"
//
//#include "MusoPlayerManager.generated.h"
//
//
//USTRUCT()
//struct MUSOSYSTEMPLUGIN_API FPlayerProperties
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	UMusoPlayer* Player = nullptr;
//
//	UPROPERTY()
//	UMusoData* MusoData = nullptr;
//
//	FTimerHandle TimerHandle;
//};
//
//UCLASS()
//class MUSOSYSTEMPLUGIN_API MusoPlayerManager : public UObject
//{
//	GENERATED_BODY()
//
//private:
//	UPROPERTY()
//	TArray<FPlayerProperties> MusoPlayers;
//
//	UPROPERTY()
//	MUsoPlayer* ActiceMidiPlayer = nullptr;
//
//	void BeginPlayerFadeOut(UMusoPlayer* MusoPlayer);
//	void FadeOutDone(UMusoPlayer* MusoPlayer);
//	void CancelFadeOut(UMusoPlayer* MusoPlayer);
//
//public:
//	void EnterMusoArea(UMusoData* MusoData);
//	void LeaveMusoArea(UMusoData* MusoData);
//
//};
