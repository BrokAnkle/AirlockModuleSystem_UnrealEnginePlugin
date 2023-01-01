// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWNumPadEntryButton.h"
#include "UWNumPad.h"

UUWNumPadEntryButton::UUWNumPadEntryButton(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
	TextToDisplay = FText::FromString("N");	//Placeholder text to edit in the Designer Editor
	bOverrideAccessibleDefaults = 0; //Patch duplication of the OverrideAccessibleDefaults in the Designer
}

/*Bound to the Button OnPressed*/
void UUWNumPadEntryButton::OnButtonClicked_Implementation()
{
	if (NumPadOwner)
		NumPadOwner->EnterCode(TextToDisplay);
}
