#include "MusoAsset.h"


UMusoDataAssetFactory::UMusoDataAssetFactory()
{
	SupportedClass = UMusoData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UMusoDataAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UMusoData>(InParent, Class, Name, Flags, Context);
}

TSoftClassPtr<UObject> UAssetDefinition_UMusoData::GetAssetClass() const
{
	return UMusoData::StaticClass();
}

FLinearColor UAssetDefinition_UMusoData::GetAssetColor() const
{
	return FColor::Cyan;
}

FText UAssetDefinition_UMusoData::GetAssetDisplayName() const
{
	return FText::FromString(TEXT("Muso System Data"));
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_UMusoData::GetAssetCategories() const
{
	static const TArray<FAssetCategoryPath> Categories = {
	FAssetCategoryPath(FText::FromString(TEXT("Muso System Plugin")))
	};
	return Categories;
}
