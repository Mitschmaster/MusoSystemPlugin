#pragma once

#include "CoreMinimal.h"

#include "MusoData.generated.h"

UCLASS(BlueprintType)
class MUSOSYSTEMPLUGIN_API UMusoData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Data;
};