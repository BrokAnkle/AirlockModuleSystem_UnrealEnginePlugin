// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWOpenDoorButton.h"
#include "../AirlockDoorBase.h"
#include "../AirlockModuleControllerBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UUWOpenDoorButton::UUWOpenDoorButton(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
}

void UUWOpenDoorButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString("Open"));
		DisplayText->SetJustification(ETextJustify::Center);
	}
}

void UUWOpenDoorButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Button)
		Button->OnPressed.AddUniqueDynamic(this, &UUWOpenDoorButton::OnButtonClicked);
}

void UUWOpenDoorButton::OnButtonClicked_Implementation()
{
	if (DoorOwner)
		if (DoorOwner->ModuleOwner)
			DoorOwner->ModuleOwner->TriggerUnlockedEvent();
}
