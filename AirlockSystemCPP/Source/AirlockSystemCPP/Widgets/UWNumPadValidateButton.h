// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "UWNumPadButtonBase.h"
#include "UWNumPadValidateButton.generated.h"

/*NumPadButton child that implement the OnButtonClicked that validate player entered code*/
UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetNumPadValidateButton"))
class AIRLOCKSYSTEMCPP_API UUWNumPadValidateButton : public UUWNumPadButtonBase
{
	GENERATED_BODY()


public:
	UUWNumPadValidateButton(const FObjectInitializer& ObjectInitializer);

protected:

	/*Native implementation of the parent OnButtonClicked function, is bound in the Button OnPressed, call ValidateCode of the owning NumPad*/
	virtual void OnButtonClicked_Implementation() override;

private:
};
