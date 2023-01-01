// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWNumPadButtonBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


UUWNumPadButtonBase::UUWNumPadButtonBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
	bOverrideAccessibleDefaults = 0;	//Patch duplication of the OverrideAccessibleDefaults in the Designer
	
	if (DisplayText)
		DisplayText->SetJustification(ETextJustify::Center);
}

void UUWNumPadButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DisplayText)
		DisplayText->SetText(TextToDisplay);
	if (Button)
		Button->OnClicked.AddUniqueDynamic(this, &UUWNumPadButtonBase::OnButtonClicked);
}

/*Bound to the Button OnPressed, implemented in the child classes*/
void UUWNumPadButtonBase::OnButtonClicked_Implementation()
{
	//Implementation in child classes
}
