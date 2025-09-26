#include "MusoAsset.h"




/*
FText UMusoLoopAsset::GetAssetDisplayName() const
{
	return FText::FromString(TEXT("Muso Loop Data"));
}

FLinearColor UMusoLoopAsset::GetAssetColor() const
{
	return FLinearColor(1.0f, 0.5f, 1.0f);
}

TSoftClassPtr<UObject> UMusoLoopAsset::GetAssetClass() const
{
	return UMusoLoopData::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UMusoLoopAsset::GetAssetCategories() const
{
	static const auto Categories = { EAssetCategoryPaths::Audio / FText::FromString(TEXT("MusoSystem"))};
	return Categories;
}

FText UMusoLoopFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Muso Loop Data"));
}

FText UMusoLoopFactory::GetToolTip() const
{
	return FText::FromString(TEXT("Muso Loop Data for Muso Subsystem"));
}

UObject* UMusoLoopFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	return nullptr;
}
*/
/*
UClass* UMusoDataAssetDefinition::GetAssetClass() const
{
	return UMusoData::StaticClass();
}

FText UMusoDataAssetDefinition::GetName() const
{
	return INVTEXT("Muso System Data");
}

FColor UMusoDataAssetDefinition::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 UMusoDataAssetDefinition::GetCategories()
{
	return EAssetCategoryPaths::Misc;
}*/


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
