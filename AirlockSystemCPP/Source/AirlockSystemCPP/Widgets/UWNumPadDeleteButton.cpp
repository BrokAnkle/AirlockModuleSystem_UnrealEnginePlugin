// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWNumPadDeleteButton.h"
#include "UWNumPadButtonBase.h"
#include "UWNumPad.h"

UUWNumPadDeleteButton::UUWNumPadDeleteButton(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
	TextToDisplay = FText::FromString("Del");
	bOverrideAccessibleDefaults = 0;	//Patch duplication of the OverrideAccessibleDefaults in the Designer
}

/*Bound to the Button OnPressed*/
void UUWNumPadDeleteButton::OnButtonClicked_Implementation()
{
	if (NumPadOwner)
		NumPadOwner->DeleteCode();
}
