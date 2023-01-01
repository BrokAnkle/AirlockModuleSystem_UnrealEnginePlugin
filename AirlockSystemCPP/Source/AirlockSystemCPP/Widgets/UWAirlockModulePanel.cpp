// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWAirlockModulePanel.h"
#include "../AirlockModuleControllerBase.h"


#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"


#include "UWPressurizationProgress.h"
#include "UWNumPad.h"
#include "UWStartPressurizationButton.h"
#include "UWAirlockModuleUnlockButton.h"
#include "Components/CanvasPanelSlot.h"

UUWAirlockModulePanel::UUWAirlockModulePanel(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
}

//Called by the owning controller to bind delegates and init the whole widget panel, will dispatch other init to its components
void UUWAirlockModulePanel::InitPanel_Implementation(AAirlockModuleControllerBase* ModuleController)
{
	ModuleOwner = ModuleController;

	InitProperties();

	PressurizationProgress->InitPressurizationProgress(this);
	NumPad->InitPad(this);
	PanelSwitcher->SetVisibility(ESlateVisibility::Collapsed);

	BindDelegates();
}


void UUWAirlockModulePanel::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (VerticalBox)
		Cast<UCanvasPanelSlot>(VerticalBox->Slot)->SetAutoSize(true);
	if (DisplayText)
		DisplayText->SetJustification(ETextJustify::Center);
}

void UUWAirlockModulePanel::OnDoorOpen_Implementation()
{
	DisplayText->SetText(FText::FromString("Door open !"));
	PanelSwitcher->SetVisibility(ESlateVisibility::Collapsed);
}

void UUWAirlockModulePanel::OnDoorClosed_Implementation()
{
	DisplayText->SetText(FText::FromString("Module closed but not locked"));

	PanelSwitcher->SetActiveWidget(PressurizeButton->GetRootWidget());
	PanelSwitcher->SetVisibility(ESlateVisibility::Visible);
	PressurizeButton->SetVisibility(ESlateVisibility::Visible);
}

void UUWAirlockModulePanel::OnModuleLocked_Implementation()
{
	DisplayText->SetText(FText::FromString("Waiting for pressurization to start"));
}

void UUWAirlockModulePanel::OnStartPressurize_Implementation()
{
	PressurizationProgress->UpdateTimer(ModuleOwner->GetLockTime());
	PressurizeButton->SetVisibility(ESlateVisibility::Collapsed);
	PressurizationProgress->SetVisibility(ESlateVisibility::Visible);
}

void UUWAirlockModulePanel::OnPressurizationUpdate_Implementation()
{
	DisplayText->SetText(FText::FromString("Pressurization in progress"));
	PressurizationProgress->UpdateTimer(ModuleOwner->GetCurrentLockTime());
}

void UUWAirlockModulePanel::OnEndPressurization_Implementation()
{
	DisplayText->SetText(FText::FromString("Pressurization finished"));
	PressurizationProgress->SetVisibility(ESlateVisibility::Collapsed);
}

void UUWAirlockModulePanel::UnlockByTime_Implementation()
{
	PanelSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	ModuleOwner->TriggerTryUnlockEvent();
}

void UUWAirlockModulePanel::DisplayUnlockButton_Implementation()
{
	PanelSwitcher->SetActiveWidget(UnlockButton);
	DisplayText->SetText(FText::FromString("Press to unlock"));
}

void UUWAirlockModulePanel::DisplayNumPad_Implementation()
{
	PanelSwitcher->SetActiveWidget(NumPad);
	DisplayText->SetText(FText::FromString("Enter code to unlock"));
	NumPad->m_bAutoValidate = ModuleOwner->m_bCodeAutoValidate;
}

void UUWAirlockModulePanel::OnTryUnlock_Implementation()
{
	DisplayText->SetText(FText::FromString("Unlocking..."));
}

void UUWAirlockModulePanel::TimerFailure_Implementation()
{
	DisplayText->SetText(FText::FromString("Time over. Unlock failed"));
}

void UUWAirlockModulePanel::ButtonFailure_Implementation()
{
	DisplayText->SetText(FText::FromString("Unlock failure"));
}

void UUWAirlockModulePanel::CodeFailure_Implementation()
{
	DisplayText->SetText(FText::FromString("Wrong code !"));
}

void UUWAirlockModulePanel::OnUnlockSuccess_Implementation()
{
	PanelSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	DisplayText->SetText(FText::FromString("Unlocked !"));
}

void UUWAirlockModulePanel::BindDelegates()
{
	ModuleOwner->DoorOpen_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnDoorOpen);
	ModuleOwner->DoorClosed_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnDoorClosed);
	ModuleOwner->Locked_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnModuleLocked);

	ModuleOwner->StartPressurization_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnStartPressurize);
	ModuleOwner->UpdatePressurization_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnPressurizationUpdate);
	ModuleOwner->EndPressurization_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnEndPressurization);
	switch (m_eModuleUnlockMethod)
	{
	case EModuleUnlockMethod::ETimeMUM:
		ModuleOwner->EndPressurization_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::UnlockByTime);
		ModuleOwner->UnlockFailed_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::TimerFailure);
		break;
	case EModuleUnlockMethod::EButtonMUM:
		ModuleOwner->EndPressurization_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::DisplayUnlockButton);
		ModuleOwner->UnlockFailed_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::ButtonFailure);
		break;
	case EModuleUnlockMethod::ECodeMUM:
		ModuleOwner->EndPressurization_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::DisplayNumPad);
		ModuleOwner->UnlockFailed_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::CodeFailure);
		break;
	default:break;
	}
	ModuleOwner->TryUnlock_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnTryUnlock);
	ModuleOwner->Unlocked_Delegate.AddUniqueDynamic(this, &UUWAirlockModulePanel::OnUnlockSuccess);
}

void UUWAirlockModulePanel::InitProperties()
{
	m_eModuleUnlockMethod = ModuleOwner->m_eUnlockMethod;
	Code = ModuleOwner->GetCode();
	m_bShowPressurizationBar = ModuleOwner->m_bShowPressurizationProgressBar;
	m_bShowPressurizationTimer = ModuleOwner->m_bShowPressurizationTimer;

	FString t;	
	t.Append("Airlock system plugin\nUnlock by " + UEnum::GetDisplayValueAsText(m_eModuleUnlockMethod).ToString() + " exemple");
	
	DisplayText->SetText(FText::FromString(t));
}

void UUWAirlockModulePanel::PrintScreen(FString string, float time /*= 0*/, FColor color /*= FColor::Blue*/)
{
	GEngine->AddOnScreenDebugMessage(-1, time, color, string);
}
