// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AirlockModuleControllerBase.generated.h"

UENUM(BlueprintType, Category = "ArlockSystem", meta = (ToolTip = "How the module unlock itself"))
enum class EModuleUnlockMethod : uint8
{
	ETimeMUM UMETA(DisplayName = Time, ToolTip = "The module unlocks just by time when the pressurization end (default)"),
	EButtonMUM UMETA(DisplayName = Button, ToolTip = "The module unlocks by pressing a button once the room has been pressurized"),
	ECodeMUM UMETA(DisplayName = Code, ToolTip = "The module unlocks by entering a code once the room has been pressurized"),
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EModuleState : uint8
{
	EIsLocked UMETA(DisplayName = IsLocked),
	EIsOpen UMETA(DisplayName = IsOpen),
	ECanBeUnlocked UMETA(DisplayName = CanBeUnlocked),
	EIsPressurizing UMETA(DisplayName = IsPressurizing),
	ECanBeOpen UMETA(DisplayName = CanBeOpen),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModuleDelegate);

/*C++ Base abstract class of Module controller for the Airlock system plugin*/
UCLASS(Abstract, AutoExpandCategories = (AirlockSystem), Blueprintable, BlueprintType, ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_AbstractModuleController"))
class AIRLOCKSYSTEMCPP_API AAirlockModuleControllerBase : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AAirlockModuleControllerBase();

	/*Can the module be open ? (no door open, no pressurization and unlocked)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem|State", meta = (DisplayName = "CanBeOpen?"))
		bool m_bCanBeOpen = true;

	/*Should the pressurization progress bar be displayed when pressurizing ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Lock", meta = (DisplayName = "ShowPressuriationProgressBar?"))
		bool m_bShowPressurizationProgressBar = true;

	/*Should the pressurization remaining time be displayed when pressurizing ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Lock", meta = (DisplayName = "ShowPressurizationTimer?", ToolTip = ""))
		bool m_bShowPressurizationTimer = true;

	/*If the unlock method is by code, does the code check itself at each entry ?*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem|Code", meta = (DisplayName = "CodeAutoValidate?"))
		bool m_bCodeAutoValidate = false;

	/*All the doors this module manages*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AirlockSystem")
		TArray<class AAirlockDoorBase*> Doors;

	/*How the module is unlocked*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem|Lock", meta = (DisplayName = "UnlockMethod"))
		EModuleUnlockMethod m_eUnlockMethod = EModuleUnlockMethod::ETimeMUM;

	/*Return the LockTime*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		float GetLockTime() { return m_fPressurizationTime; }

	/*Return the current lock timer*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		float GetCurrentLockTime() { return m_fPressurizationRemainingTime; }

	/*Return the code the player need to enter *if the unlock method is CODE**/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		FText GetCode() { return m_tCode; }

	/*Return m_bIsLocked value. Know if the module is currently locked*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		bool IsLocked() { return m_bIsLocked; }

	/*Public function to trigger the OnUnlocked_Event. Used by the door's Open button.*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void TriggerUnlockedEvent() { OnUnlocked_Event(); }

	/*Public function to trigger the OnTryUnlock_Event. Useed by the AirlockModulePanel*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void TriggerTryUnlockEvent() { OnTryUnlock_Event(); }

	/*Public function to trigger the OnFailUnlock_Event. Used by the AirlockModulePanel*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void TriggerFailUnlockEvent() { OnFailUnlock_Event(); }

#pragma region DELEGATES

	/*Delegate triggered by OnDoorOpen_Event, when one of the linked doors from the Doors aray Sjust start to open*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate DoorOpen_Delegate;
	/*Delegate triggered by OnDoorClosed_Event, when the open door is finally closed*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate DoorClosed_Delegate;
	/*Delegate triggered by OnLocked_Event, when the module just locks itself*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate Locked_Delegate;
	/*Delegate triggered by OnStartPressurization_Event, when the module just starts the pressurization of the room*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate StartPressurization_Delegate;
	/*Delegate triggered by OnUpdatePressurization_Event, each time the pressurization progresses*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate UpdatePressurization_Delegate;
	/*Delegate triggered by OnEndPressurization_Event, when the pressurization just ended*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate EndPressurization_Delegate;
	/*Delegate triggered by OnTryUnlock_Event, when an action required to unlock the module is performed*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate TryUnlock_Delegate;
	/*Delegate triggered by OnUnlockFailed_Event, when trying to unlock the module fail by a certain external condition*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate UnlockFailed_Delegate;
	/*Delegate triggered by OnUnlocked_Event, when the module is finally unlocked and the doors can be open again*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FModuleDelegate Unlocked_Delegate;

#pragma endregion

	/*The time the module take to pressurize the room before it can be unlocked*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Lock", meta = (DisplayName = "PressurizationTime", min = 0.f))
		float m_fPressurizationTime = 2.f;

	/*Does the module launch the pressurization automatically when a door closes ? If not the pressurization has to be triggered manually*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Lock", meta = (DisplayName = "AutoPressurize?"))
		bool m_bAutoPressurize = false;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;	//BP construction script equivalent

#pragma region COMPONENTS

	/*Root component*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem|Components")
		class USceneComponent* Root;
	/*Simple static mesh that serve as the physical representation in the world*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Components")
		class UStaticMeshComponent* PanelMesh;
	/*Pivot point for the lights to turn around it*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AirlockSystem|Components")
		class USceneComponent* LightsPivot;
	/*First spot light attached to the Light pivot, oriented towards the other side of Light2*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Components")
		class USpotLightComponent* Light1;
	/*Second spot light attached to the Light pivot, oriented towards the other side of Light1*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Components")
		class USpotLightComponent* Light2;
	/*Interactable widget that serve as a monitor to control the module*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Components")
		class UWidgetComponent* PanelWidget;

#pragma endregion

	/*Is the module locked ? Means, does the doors, referenced in the Doors array, can be open ?*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem|State", meta = (DisplayName = "IsLocked?"))
		bool m_bIsLocked = false;

	/*Is the module open ? Means, one of the doors, referenced in the Doors array, is open ?*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem|State", meta = (DisplayName = "IsOpen?"))
		bool m_bIsOpen = false;

	/*Can the module be unlocked ?*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem|State", meta = (DisplayName = "CanBeUnlocked?"))
		bool m_bCanBeUnlocked = true;

	/*Is the module pressurizing air ? (Module is also currently locked)*/
	UPROPERTY(VisibleAnywhere, Category = "AirlockSystem|State", meta = (DisplayName = "IsPressurizing?"))
		bool m_bIsPressurizing = false;

	/*Remaining time of the pressurization. When reach 0, pressurizing is over and the module can be unlocked*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AirlockSystem|Lock", meta = (DisplayName = "PressurizationRemainingTime", min = 0.f))
		float m_fPressurizationRemainingTime = m_fPressurizationTime;

	/*The code that the player would need to enter if the module unlock method is Code, if not, ignore*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem|Code", meta = (DisplayName = "Code"))
		FText m_tCode;

#pragma region MODULE EVENTS

	/*Function called when one the door in the Doors array just opens
	Set the module to open, and can't be open anymore and light up the lights to orange*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnDoorOpen_Event();
	/*Function called when the open door in the Doors array finally closes
	Set the module to not open, and the lights become red, then lock the module*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnDoorClosed_Event();
	/*Function called that locks the module once the door is closed
	Set the module to locked and can't be unlocked then launch the pressurization if it's auto pressurized*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnLocked_Event();
	/*Function called when the pressurization just started,
	Tell the module is pressurizing and the Pressurizing remaing time is set to the Pressurizing time*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnStartPressurization_Event();
	/*Function called each time the pressurization progresses,
	Make the LightPivot rotates, and decrement the Pressurization remaining time, then ends the pressurization if it reach 0*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnUpdatePressurization_Event();
	/*Function called when the pressurization finally ends
	Set the module can be unlocked and is not pressurizing anymore*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnEndPressurization_Event();
	/*Function called when an action required to unlock the module is performed
	Will check if the module can be unlocked then decide to make the Unlock a success or a failure*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnTryUnlock_Event();
	/*Function called when the unlock is a failure*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnFailUnlock_Event();
	/*Function called when the unlock is a success
	Set the module can be open and not locked anymore, also make the lights green*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnUnlocked_Event();

#pragma endregion
	
	/*Function that calls Init to all the doors in the Doors array with a this reference, and bound to their delegates, also destroy their OpenButton if the module Auto Pressurize itself*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void InitDoors();
	/*Function that call Init to the panel wiith a this reference, bound to some button clicks*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AirlockSystem")
		void InitPanel();

	/*Speed of the Light pivot when rotating*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AirlockSystem", meta = (DisplayName = LightRotationSpeed))
		float m_fLightRotationSpeed = 500.f;
private:

	/*Function bound to the OnStartOpenDoor_Delegate of all the doors in the Doors array that will launch OnDoorOpen_Event*/
	void BindOnDoorOpen() { OnDoorOpen_Event(); }

	/*Function bound to the OnEndCloseDoor_Delegate of all the doors in the Doors array that will launch OnDoorClose_Event*/
	void BindOnDoorClosed() { OnDoorClosed_Event(); }

	/*Function bound to the Start Pressurize Button of the Pannel when clicked, that will launch OnStartPressurization_Event*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void TriggerStartPressurizationEvent() { OnStartPressurization_Event(); };
	
#pragma region DEBUG

	inline void PrintScreen(FString string, float time = 0.f, FColor color = FColor::Green);
	inline void PrintBoundFunctionsToDelegates();
	inline void RemoveBoundFunctionsFromDelegates(const UObject* from);

#pragma endregion

};
