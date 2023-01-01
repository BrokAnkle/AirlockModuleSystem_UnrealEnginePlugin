// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWNumPadValidateButton.h"
#include "UWNumPadButtonBase.h"
#include "UWNumPad.h"

UUWNumPadValidateButton::UUWNumPadValidateButton(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
	TextToDisplay = FText::FromString("Val");
}

/*Bound in the Button OnPressed*/
void UUWNumPadValidateButton::OnButtonClicked_Implementation()
{
	if (NumPadOwner)
		NumPadOwner->ValidateCode();
}
