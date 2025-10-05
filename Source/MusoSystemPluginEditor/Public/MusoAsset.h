#pragma once

#include "AssetDefinitionDefault.h"
#include "Factories/Factory.h"
#include "MusoData.h"

#include "MusoAsset.generated.h"


UCLASS()
class MUSOSYSTEMPLUGINEDITOR_API UAssetDefinition_UMusoData : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual FText GetAssetDisplayName() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

UCLASS()
class MUSOSYSTEMPLUGINEDITOR_API UMusoDataAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	explicit UMusoDataAssetFactory();
	UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
