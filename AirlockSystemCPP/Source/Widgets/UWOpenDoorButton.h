// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWOpenDoorButton.generated.h"

/*Button widget that will unlock the owning door's owning module controller*/
UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetOpenDoorButton"))
class AIRLOCKMODULE_API UUWOpenDoorButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UUWOpenDoorButton(const FObjectInitializer& ObjectInitializer);

	/*Reference to the owning door*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		class AAirlockDoorBase* DoorOwner;

protected:

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	/*Bound to the Button OnClicked, unlock the module the owning door is owned by*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnButtonClicked();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UTextBlock* DisplayText;
};
