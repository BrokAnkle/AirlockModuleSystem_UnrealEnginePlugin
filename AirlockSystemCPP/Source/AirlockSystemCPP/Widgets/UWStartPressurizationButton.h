// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWStartPressurizationButton.generated.h"

/**
 *
 */
/*Button widget that launch the pressurization when pressed if the module has bound to it*/
UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetStartPressurizationButton"))
class AIRLOCKSYSTEMCPP_API UUWStartPressurizationButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UUWStartPressurizationButton(const FObjectInitializer& ObjectInitializer);

	/*Return the Button component*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		class UButton* GetButton() { return Button; }

	/*Return the DisplayText TextBlock component*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		class UTextBlock* GetDisplayText() { return DisplayText; }

protected:

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

private:
		
	UPROPERTY(EditAnywhere, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UButton* Button;

	UPROPERTY(EditAnywhere, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UTextBlock* DisplayText;

	/*Bound to the Button OnClicked*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void OnButtonClicked();
};
