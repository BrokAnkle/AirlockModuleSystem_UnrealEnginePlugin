// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWStartPressurizationButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ButtonSlot.h"

UUWStartPressurizationButton::UUWStartPressurizationButton(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
}

void UUWStartPressurizationButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString("Start pressurization"));
		Cast<UButtonSlot>(DisplayText->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	}
	if (DisplayText)
		DisplayText->SetJustification(ETextJustify::Center);
}

void UUWStartPressurizationButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button)
		Button->OnClicked.AddUniqueDynamic(this, &UUWStartPressurizationButton::OnButtonClicked);
}

void UUWStartPressurizationButton::OnButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
