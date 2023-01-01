// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "UWNumPadButtonBase.h"
#include "UWNumPadDeleteButton.generated.h"

/*NumPadButton child that implement the OnButtonClicked that delete player entries*/
UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetNumPadDeleteButton"))
class AIRLOCKSYSTEMCPP_API UUWNumPadDeleteButton : public UUWNumPadButtonBase
{
	GENERATED_BODY()


public:
	UUWNumPadDeleteButton(const FObjectInitializer& ObjectInitializer);


protected:

	/*Native implementation of the parent OnButtonClicked function, is bound in the Button OnPressed, call DeleteCode of the owning NumPad*/
	virtual void OnButtonClicked_Implementation() override;

private:

};
