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







/*


UCLASS()
class UMusoLoopAsset : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
	virtual bool CanImport() const override { return true; };
};

UCLASS()
class UMusoLoopFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	//~ BEGIN UFactory interface
	virtual FText GetDisplayName() const override;
	virtual FText GetToolTip() const override;
	virtual bool FactoryCanImport(const FString& Filename) override { return true; };
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	virtual void CleanUp() override;
	//~ END of UFactory interface
	*/