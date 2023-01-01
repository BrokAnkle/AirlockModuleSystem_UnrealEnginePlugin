// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWNumPad.h"

#include "Binding/VisibilityBinding.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"

#include "UWAirlockModulePanel.h"
#include "UWNumPadButtonBase.h"
#include "UWNumPadDeleteButton.h"
#include "UWNumPadEntryButton.h"
#include "UWNumPadValidateButton.h"
#include "Components/CanvasPanelSlot.h"

UUWNumPad::UUWNumPad(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
	/*Centering the grid*/
	if (Grid)
	{
		Cast<UCanvasPanelSlot>(Grid->Slot)->SetAutoSize(true);
		Cast<UCanvasPanelSlot>(Grid->Slot)->SetAnchors(FAnchors(.5f));
		Cast<UCanvasPanelSlot>(Grid->Slot)->SetAlignment(FVector2D(.5f));
	}
}

/*Called by the owning panel in its InitPanel function*/
void UUWNumPad::InitPad_Implementation(UUWAirlockModulePanel* Panel)
{
	PanelOwner = Panel;
	CorrectCode = PanelOwner->GetCode();
	m_bAutoValidate = PanelOwner->GetModuleOwner()->m_bCodeAutoValidate;
}

/*Called when the player press the Delete button*/
void UUWNumPad::DeleteCode_Implementation()
{
	if (!CurrentCode.IsEmptyOrWhitespace())
	{
		FString sCode = CurrentCode.ToString();
		sCode = sCode.Mid(0, sCode.Len() - 1);
		CurrentCode = FText::FromString(sCode);
		DisplayText->SetText(CurrentCode);
	}
}

/*Called when player press an entry button*/
void UUWNumPad::EnterCode_Implementation(const FText& entry)
{
	if (CurrentCode.ToString().Len() < CorrectCode.ToString().Len())	//Can make a new entry only if the player is not entering a longer code that the correct one
	{

		FString newCode = CurrentCode.ToString() + entry.ToString();	//tale the old current code and add the new entry

		CurrentCode = FText::FromString(newCode);	//update the current code
		DisplayText->SetText(CurrentCode);	//update the panel displayed text
		if (m_bAutoValidate)	//validate now the code if auto validate is set
			ValidateCode();
	}
}

/*Called when the player press the Validate button, or by EnterCode if AutoValidate is set*/
void UUWNumPad::ValidateCode_Implementation()
{
	if (CorrectCode.EqualToCaseIgnored(CurrentCode))	//Current code and correct code are the same
		PanelOwner->ModuleTriggerTryUnlock();	//Try to unlock the module
	else if (!m_bAutoValidate)	//Current code and correct code are not the same and the code does not auto validate
	{
		PanelOwner->ModuleTriggerFailUnlock();	//We fail to unlock the module when tried
		DisplayText->SetText(CurrentCode = FText::GetEmpty());	//Empty the current entered code and update the displayed text
	}
}


void UUWNumPad::NativePreConstruct()
{
	Super::NativePreConstruct();

	//Keep button reference in check	
	if (Button0)
		EntryButtons.AddUnique(Button0);
	if (Button1)
		EntryButtons.AddUnique(Button1);
	if (Button2)
		EntryButtons.AddUnique(Button2);
	if (Button3)
		EntryButtons.AddUnique(Button3);
	if (Button4)
		EntryButtons.AddUnique(Button4);
	if (Button5)
		EntryButtons.AddUnique(Button5);
	if (Button6)
		EntryButtons.AddUnique(Button6);
	if (Button7)
		EntryButtons.AddUnique(Button7);
	if (Button8)
		EntryButtons.AddUnique(Button8);
	if (Button9)
		EntryButtons.AddUnique(Button9);

	//Give onwing pad reference
	for (auto btn : EntryButtons)
		if (btn)
			btn->NumPadOwner = this;

	if (DeleteButton)
		DeleteButton->NumPadOwner = this;

	if (ValidateButton)
		ValidateButton->NumPadOwner = this;
}


void UUWNumPad::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ValidateButton)
		ValidateButton->VisibilityDelegate.BindDynamic(this, &UUWNumPad::GetValidateButtonVisibility);
}

/*Bound to ValidateButton VisibilityDelegate*/
ESlateVisibility UUWNumPad::GetValidateButtonVisibility_Implementation()
{
	return m_bAutoValidate ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
}
