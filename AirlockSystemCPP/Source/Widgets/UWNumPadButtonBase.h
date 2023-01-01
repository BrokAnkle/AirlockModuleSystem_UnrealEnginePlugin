// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWNumPadButtonBase.generated.h"

/*Abstract widget with base properties and implementable methods to interact with its owning pad*/
UCLASS(Abstract, Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetAbstractNumPadButton"))
class AIRLOCKMODULE_API UUWNumPadButtonBase : public UUserWidget
{
	GENERATED_BODY()


public:
	UUWNumPadButtonBase(const FObjectInitializer& ObjectInitializer);

	/*Editable text the DisplayText will displayed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		FText TextToDisplay;

	/*Reference to the owning NumPad*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		class UUWNumPad* NumPadOwner;

protected:

	virtual void NativePreConstruct() override;

	/*Implementable function bound to the Button OnPressed*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnButtonClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UTextBlock* DisplayText;

private:

};
