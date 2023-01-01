// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "UWPressurizationProgress.h"
#include "UWAirlockModulePanel.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

UUWPressurizationProgress::UUWPressurizationProgress(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
}

void UUWPressurizationProgress::InitPressurizationProgress_Implementation(UUWAirlockModulePanel* ModulePanel)
{
	PanelOwner = ModulePanel;
	m_bShowProgressBar = PanelOwner->GetShowPressurizationBar();
	m_bShowProgressTimer = PanelOwner->GetShowPressurizationTimer();
	m_fTimer = PanelOwner->GetModuleOwner()->m_fPressurizationTime;
	SetVisibility(ESlateVisibility::Collapsed);
}


void UUWPressurizationProgress::UpdateTimer(float newTimer)
{
	m_fPressurizationRemainingTime = newTimer;
}

void UUWPressurizationProgress::NativePreConstruct()
{
	Super::NativePreConstruct();

	/*Center the bar and the text*/
	if (ProgressBar)
	{
		Cast<UCanvasPanelSlot>(ProgressBar->Slot)->SetSize(FVector2D(200, 30));
		Cast<UCanvasPanelSlot>(ProgressBar->Slot)->SetAnchors(FAnchors(.5f));
		Cast<UCanvasPanelSlot>(ProgressBar->Slot)->SetAlignment(FVector2D(.5f, .5f));
	}
	if (TimerText)
	{
		Cast<UCanvasPanelSlot>(TimerText->Slot)->SetAutoSize(true);
		Cast<UCanvasPanelSlot>(TimerText->Slot)->SetAnchors(FAnchors(.5f));
		Cast<UCanvasPanelSlot>(TimerText->Slot)->SetAlignment(FVector2D(.5f, .5f));
	}
}

void UUWPressurizationProgress::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ProgressBar)
	{
		ProgressBar->VisibilityDelegate.BindDynamic(this, &UUWPressurizationProgress::GetBarVisibility);
		ProgressBar->PercentDelegate.BindDynamic(this, &UUWPressurizationProgress::GetPercent);
		ProgressBar->FillColorAndOpacityDelegate.BindDynamic(this, &UUWPressurizationProgress::GetColor);
	}
	if (TimerText)
	{
		TimerText->VisibilityDelegate.BindDynamic(this, &UUWPressurizationProgress::GetTextVisibility);
		TimerText->TextDelegate.BindDynamic(this, &UUWPressurizationProgress::GetCurrentTimerText);
	}

}

FText UUWPressurizationProgress::GetCurrentTimerText()
{
	FNumberFormattingOptions opts;
	opts.SetMaximumFractionalDigits(1);

	return FText::AsNumber(m_fPressurizationRemainingTime, &opts);
}

float UUWPressurizationProgress::GetPercent()
{
	return m_fPressurizationRemainingTime / m_fTimer;
}

FLinearColor UUWPressurizationProgress::GetColor_Implementation()
{
	//The more the pressurization progress the more the bar goes from red to green
	return FLinearColor(GetPercent(), 1 - GetPercent(), 0, 1);
}

ESlateVisibility UUWPressurizationProgress::GetBarVisibility()
{
	return m_bShowProgressBar ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

ESlateVisibility UUWPressurizationProgress::GetTextVisibility()
{
	return m_bShowProgressTimer ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}
