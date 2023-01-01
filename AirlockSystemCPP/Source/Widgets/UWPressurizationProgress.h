// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPressurizationProgress.generated.h"

/**
 *
 */

/*Widget that tracks the progress of the pressurization while it's going on*/
UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = (AirlockSystem), ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetPressurizationProgress"))
class AIRLOCKMODULE_API UUWPressurizationProgress : public UUserWidget
{
	GENERATED_BODY()

public:

	UUWPressurizationProgress(const FObjectInitializer& ObjectInitializer);

	/*Called by the owning panel in its InitPanel function, store some properties*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void InitPressurizationProgress(class UUWAirlockModulePanel* ModulePannel);

	/*Set the current timer as the newTimer, called by the panel when its owning module's UpdatePressurization_Delegate is fired*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void UpdateTimer(float newTimer);

	/*Returns the CurrentTimer value*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		float GetCurrentTime() { return m_fPressurizationRemainingTime; }

	/*Initial timer value when the pressurization starts*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		float m_fTimer;

protected:

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	/*Bound to the TimerText's TextDelegate*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		FText GetCurrentTimerText();

	/*Bound to the ProgressBar's PercentDelegate*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		float GetPercent();

	/*Bound to the ProgressBar's FillColorAndOpacityDelegate*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "AirlockSystem")
		FLinearColor GetColor();

	/*Bound to the ProgressBar's VisibilityDelegate*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		ESlateVisibility GetBarVisibility();

	/*Bound to the TimerText's VisibilityDelegate*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		ESlateVisibility GetTextVisibility();

	/*Reference to the owning panel*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem")
		class UUWAirlockModulePanel* PanelOwner;

private:

	/*Progress bar that deplete as the pressurization progresses*/
	UPROPERTY(EditAnywhere, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UProgressBar* ProgressBar;

	/*Text that show how many time is remained before the pressurization is over*/
	UPROPERTY(EditAnywhere, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UTextBlock* TimerText;

	/*Determine if the ProgressBar is shown*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem")
		bool m_bShowProgressBar;

	/*Determin if the TimerText is shown*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem")
		bool m_bShowProgressTimer;

	/*Remaining pressurization time*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem")
		float m_fPressurizationRemainingTime;

	/*Canvas the ProgressBar and TimerText are put together*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UCanvasPanel* CanvasPanel;
};
