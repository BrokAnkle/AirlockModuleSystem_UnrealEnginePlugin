// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWNumPad.generated.h"

/*Widget that contains a grid of button to enter a code to unlock the owning panel's owning module*/
UCLASS(AutoExpandCategories = (AirlockSystem), Blueprintable, BlueprintType, ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_WidgetNumPad"))
class AIRLOCKMODULE_API UUWNumPad : public UUserWidget
{
	GENERATED_BODY()

		UUWNumPad(const FObjectInitializer& ObjectInitializer);

public:

	/*Return the current player entered code*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		FText GetCode() { return CurrentCode; }

	/*Called by the panel in its InitPanel function, set the owning panel et store some properties*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void InitPad(UUWAirlockModulePanel* Panel);

	/*When the player press the Delete button, delete the last entered character of the current code*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void DeleteCode();

	/*When the player press one of the entry buttons, add the associated value to the current code*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void EnterCode(const FText& entry);

	/*When the player press the Validate button or each time he makes an entry if code auto validate is set to true, check if the current entered code is the same as the correct code and acts accordingly*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void ValidateCode();

	/*Does the current entered code check by itself if it is the correct one at each entry ?*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem", meta = (DisplayName = "AutoValidate?"))
		bool m_bAutoValidate;

protected:

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	/*Bound to the Visibility Delegate of the Validate button. The validate button is hidden when AutoValidate is set because it is not needed*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlolckSystem")
		ESlateVisibility GetValidateButtonVisibility();

private:

	/*Current player entered code*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FText CurrentCode;

	/*Correct code to enter and validate to unlock the module*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FText CorrectCode;

	/*Reference to the owning panel*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		class UUWAirlockModulePanel* PanelOwner;

#pragma region ENTRY BUTTONS

	/*Button that will enter the 0 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button0;
	/*Button that will enter the 1 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button1;
	/*Button that will enter the 2 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button2;
	/*Button that will enter the 3 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button3;
	/*Button that will enter the 4 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button4;
	/*Button that will enter the 5 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button5;
	/*Button that will enter the 6 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button6;
	/*Button that will enter the 7 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button7;
	/*Button that will enter the 8 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button8;
	/*Button that will enter the 9 value in the current code when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadEntryButton* Button9;

#pragma endregion

	/*Array of all the Entry Buttons for easier iterations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		TArray<class UUWNumPadEntryButton*> EntryButtons;

	/*Button widget to delete the last entered entry when pressed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UUWNumPadDeleteButton* DeleteButton;

	/*Button widget to valide the current entered code when pressed*/
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true))
		class UUWNumPadValidateButton* ValidateButton;

	/*Text to display the current entered code on the panel*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget, DesignerRebuild, AllowPrivateAccess = true))
		class UTextBlock* DisplayText;

	/*Grid to organize the num pad buttons*/
	UPROPERTY(meta = (BindWidget))
		class UUniformGridPanel* Grid;

	/*Vertical box to organize the display text and the grid*/
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* VerticalBox;
};
