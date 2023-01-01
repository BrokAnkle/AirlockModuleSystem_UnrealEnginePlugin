// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWAirlockModuleUnlockButton.generated.h"

/*Button widget that the module will bind its TryUnlock function if its UnlockMethod is set to Button*/
UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetUnlockButton"))
class AIRLOCKMODULE_API UUWAirlockModuleUnlockButton : public UUserWidget
{
	GENERATED_BODY()

public:

	UUWAirlockModuleUnlockButton(const FObjectInitializer& ObjectInitializer);

	/*Return the Button component*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		class UButton* GetButton() { return Button; }

protected:

	/*Button component that the Module controller will bound its TryUnlock_Event to*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UButton* Button;

	/*"Press to unlock" text*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UTextBlock* Text;

private:
};
