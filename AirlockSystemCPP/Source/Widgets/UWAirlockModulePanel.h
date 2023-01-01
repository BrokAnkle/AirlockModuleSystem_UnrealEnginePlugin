// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../AirlockModuleControllerBase.h"
#include "UWAirlockModulePanel.generated.h"

/**
 *
 */

/*Module panel widget of the Airlock System Plugin, group every pieces of widgets that will be used to interact with and show state of the module*/
UCLASS(Blueprintable, BlueprintType, ClassGroup = AirlockSystemPlugin, AutoExpandCategories = (AirlockSystem), meta = (DisplayName = "Airlock_WidgetModulePanel"))
class AIRLOCKMODULE_API UUWAirlockModulePanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UUWAirlockModulePanel(const FObjectInitializer& ObjectInitializer);

	/*Function called by its owning module controller that give the reference to itself in order to the panel to bind some functions to its delegates. Also, Init its own components too*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void InitPanel(class AAirlockModuleControllerBase* ModuleController);

#pragma region GETS

	/*Return the value of m_bShowPressurizationBar. Should the pressurization bar be displayed when pressurizing ?*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		bool GetShowPressurizationBar() { return m_bShowPressurizationBar; }

	/*Return the value of m_bShowPressurizationTimer. Should the pressurization timer be displayed when pressurizing ?*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		bool GetShowPressurizationTimer() { return m_bShowPressurizationTimer; }

	/*Return the reference of the Owning ModuleController*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		class AAirlockModuleControllerBase* GetModuleOwner() { return ModuleOwner; }

	/*Return the corect code to enter to unlock the module, *WHEN UNLOCK METHOD = CODE**/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		FText GetCode() { return Code; }

	/*Return the UnlockButton widget component, responsible for unlocking the module *WHEN UNLOCK METHOD = BUTTON*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		UUWAirlockModuleUnlockButton* GetUnlockButton() { return UnlockButton; }

	/*Return the PressurizeButton widget component, responsible for launching the pressurization *WHEN MODULECONTROLLER IS NOT AUTO PRESSURIZE**/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		UUWStartPressurizationButton* GetPressurizeButton() { return PressurizeButton; }

#pragma endregion

	/*Public function to trigger the TryUnlockEvent of the Owning module, used by its subcomponents. Safe.*/
	UFUNCTION()
		void ModuleTriggerTryUnlock() { if (ModuleOwner)ModuleOwner->TriggerTryUnlockEvent(); }

	/*Public function to trigger the FailUnlockEvent of the Owning module, used by its subcomponents. Safe.*/
	UFUNCTION()
		void ModuleTriggerFailUnlock() { if (ModuleOwner)ModuleOwner->TriggerFailUnlockEvent(); }

protected:

	virtual void NativePreConstruct() override;

#pragma region BOUND FUNCTIONS

	/*Bound to DoorOpen_Delegate of owning module controller, collapse the panel switcher and display Door Open text*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnDoorOpen();
	/*Bound to DoorClosed_Delegate of owning module controller, show the panel switcher and set its active widget as the PressurizeButton*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnDoorClosed();
	/*Bound to Locked_Delegate of owning module controller, just show the message on the panel that it's waiting for pressurization*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnModuleLocked();
	/*Bound to StartPressurization_Delegate of owning module controller, update the timer to the initial one, hide the pressurize button then show the pressurization progress widget*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnStartPressurize();
	/*Bound to UpdatePressurization_Delegate of owning module controller, keep showing the pressurization is in progress and update the timer*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnPressurizationUpdate();
	/*Bound to EndPressurization_Delegate of owning module controller, say the pressurization is over and collapse the pressuriation progress widget*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnEndPressurization();
	/*Bound to EndPressurization_Delegate of owning module controller if the Unlock Method is Time, collapse the pannel switcher and trigger the TryUnlock of the module*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void UnlockByTime();
	/*Bound to EndPressurization_Delegate of owning module controller if the Unlock Method is Button, just set the Unlock Button as the active widget of the panel switcher and request the player to press*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void DisplayUnlockButton();
	/*Bound to EndPressurization_Delegate of owning module controller if the Unlock Method is Code, set the active widget of the panel switcher as the numpad, and request the player to enter the code*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void DisplayNumPad();
	/*Bound to TryUnlock_Delegate of owning module controller, just change the display text*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnTryUnlock();
	/*Bound to UnlockFailed_Delegate of owning module controller is the Unlock Method is Time, just change the display text as a time over*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void TimerFailure();
	/*Bound to UnlockFailed_Delegate of owning module controller is the Unlock Method is Time, just change the display text as unlock failure*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void ButtonFailure();
	/*Bound to UnlockFailed_Delegate of owning module controller is the Unlock Method is Time, just change the display text as wrong code*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void CodeFailure();
	/*Bound to Unlocked_Delegate of owning module controller, collapse the panel switcher*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void OnUnlockSuccess();

#pragma endregion

	/*Simple text block to display some messages on the pannel indicating the state of the module*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UTextBlock* DisplayText;
	/*Progress bar and timer that track the progress of the module pressurization*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UUWPressurizationProgress* PressurizationProgress;

	/*Widget switcher that contains the differents interactible widgets used by the player to pressurize and unlock the module*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UWidgetSwitcher* PanelSwitcher;
	/*Button that launch the pressurization, shown only if the module is not auto pressurized*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UUWStartPressurizationButton* PressurizeButton;
	/*Button to unlock the module, shown only if the unlock method is by Button*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
		class UUWAirlockModuleUnlockButton* UnlockButton;
	/*Grid of button used to enter and validate the unlock code, shown only if the unlock method is by Code*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
		class UUWNumPad* NumPad;

	/*Vertical box to organize the Display Text, the pressurization progress tracker and the Panel switcher*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild))
		class UVerticalBox* VerticalBox;
	/*Reference to the owning module controller*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		class AAirlockModuleControllerBase* ModuleOwner;
	/*Is the ProgressBar of the PressurizationProgress displayed while pressurization is in progress ?*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		bool m_bShowPressurizationBar;
	/*Is the Timer text of the PressurizationProgress displayed while pressurization is in progress ?*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		bool m_bShowPressurizationTimer;
	/*Storage of the UnlockMethod of the owning module controller*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AirlockSystem")
		EModuleUnlockMethod m_eModuleUnlockMethod;
	/*Storage of the code the player should be entering if the unlock method is by code. Comes from the ownning module controller*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem")
		FText Code;

private:

	/*Canvas panel that wrap all the other widgets*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem", meta = (BindWidget))
		class UCanvasPanel* CanvasPanel;

	/*Private function that bind the events to the owning module controller delegates in the InitPanel public function*/
	UFUNCTION()
		void BindDelegates();
	/*Private function that init some properties that come from the owning module controller in the InitPanel public function*/
	UFUNCTION()
		void InitProperties();

#pragma region DEBUG

	inline void PrintScreen(FString string, float time = 0, FColor color = FColor::Blue);

#pragma endregion
};
