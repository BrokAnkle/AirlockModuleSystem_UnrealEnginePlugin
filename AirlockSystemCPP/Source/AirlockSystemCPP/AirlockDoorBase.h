// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "AirlockDoorBase.generated.h"

/*The differents types of behavior for the doors when opening/closing*/
UENUM(BlueprintType, Category = "AirlockSystem")
enum class EDoorType : uint8
{
	ERotatingDT UMETA(DisplayName = Rotating, Tooltip = "The door opens and closes by rotating on its Z axis"),
	ESlidingDT UMETA(DisplayName = Sliding, ToolTip = "The door opens and closes by sliding on its Y axis"),
	EElevatingDT UMETA(DisplayName = Elevating, ToolTip = "The door opens and closes by moving up and down on its Z axis"),
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorDelegate);

/*C++ Base abstract class of door for the Airlock system plugin*/
UCLASS(Abstract, AutoExpandCategories = (AirlockSystem), Blueprintable, BlueprintType, ClassGroup = AirlockSystemPlugin, meta = (DisplayName = "AirlockSystem_AbstractDoor"))
class AIRLOCKSYSTEMCPP_API AAirlockDoorBase : public AActor
{
	GENERATED_BODY()

public:

	AAirlockDoorBase();

#pragma region COMPONENTS

	/*Mesh of the door, with collisions*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* DoorMesh;
	/*Widget that represent a button that, on clicked, unlock the module and thus, open the door it is attached to*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UWidgetComponent* OpenButton;
	/*Collider used to detect the player when he overlap it, will check if module can open and according to the situation will open the door or not*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UBoxComponent* Collider;
	/*Presentation text above the door to indicate what type of door it is*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UTextRenderComponent* TextRender;
	/*Root Component, every other component are attached to it or attached to one attached to it*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USceneComponent* root;

#pragma endregion

	/*Function to destroy the open button of the door knob, used when the module owner auto pressurize*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem")
		void DestroyOpenButton();

#pragma region DELEGATES DECLARATION

	/*Delegate triggered by OnTryOpenDoor_Event, used to check some externals customs conditions to open the door when the player want to.
	If you want to prevent the opening of the door, set the m_bCanBeOpen of the ModuleOwner to false in your bound function.*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate TryOpenDoor_Delegate;
	/*Delegate triggered by OnStartOpen_Event, used when the door just started to open.*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate StartOpenDoor_Delegate;
	/*Delegate triggered by OnUpdateOpenDoor_Event, used by the door animation each time it is updated.*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate UpdateOpenDoor_Delegate;
	/*Delegate triggered by OnEndOpenDoor_Event, used when the door just finished to open (when the animation timeline end).*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate EndOpenDoor_Delegate;
	/*Delegate triggered by OnTryCloseDoor_Event, used to check some externals customs conditions to close the door.
	If you want to prevent the door from closing, you'll need to override OnTryCloseDoor_Event to add your conditions before it calls OnStartCloseDoor_Event.*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate TryCloseDoor_Delegate;
	/*Delegate triggered by OnStartCloseDoor_Event, used when the door just started to close*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate StartCloseDoor_Delegate;
	/*Delegate triggered by OnUpdateCloseDoor_Event, used by the door animation each time it is updated in reverse*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate UpdateCloseDoor_Delegate;
	/*Delegate triggered by OnEndCloseDoor_Event, useed when the door just finished to close (when time animation timeline end in reverse)*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AirlockSystem|Delegates")
		FDoorDelegate EndCloseDoor_Delegate;

#pragma endregion

	/*Reference to the module that manage this door*/
	UPROPERTY(BlueprintReadWrite, Category = "AirlockSystem")
		class AAirlockModuleControllerBase* ModuleOwner;

	/*Return the type of the door which determine the open and close animation*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		EDoorType GetDoorType() { return m_eDoorType; }

	/*Return the m_bWantToOpen value, does the player want to open the door - or keep it open - ?*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		bool WantToOpen() { return m_bWantToOpen; }

	/*Return the m_IsOpen value, is the door open ? Assigned but not used.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AirlockSystem")
		bool IsOpen() { return m_bIsOpen; }

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override; //BP construction script equivalent
#pragma region COLLISIONS

	/*Bound to the Collision box's OnBeginOverlap, if the player overlaps it, he wan't to open this door*/
	UFUNCTION(BlueprintNativeEvent, Category = "AirlockSystem")
		void OnColliderBeginOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*Bound to the Collision box's OnEndOverlap, if the player leaves it, he doesn't want to open this door anymore*/
	UFUNCTION(BlueprintNativeEvent, Category = "AirlockSystem")
		void OnColliderEndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Function to bind to the Collider's OnComponentEndOverlap delegate when OnEndOpenDoor_Event
	is called and the player is still in the collider so when the player leaves the collider,
	it calls the OnTryCloseDoor_Event. If the player was already out the collider, no need to bind this,
	OnTryCloseDoor_Event will be called at the OnEndOpenDoor_Event*/
	UFUNCTION()
		virtual void CloseDoorOnEndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion

#pragma region DOOR EVENTS

	/*First function to call when the player want to open the door. Called by overlaping the Collider component*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnTryOpenDoor_Event();
	/*Function called when player try to open the doorand the module allows it, trigger the door to open*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnStartOpenDoor_Event();
	/*Function called each time the door openning animation is updated, called by UpdateTimeline*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnUpdateOpenDoor_Event();
	/*Function called when the opening animation is over and the door is fully open, called by EndTimeline*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnEndOpenDoor_Event();
	/*Function that will trigger the OnStartCloseDoor_Event if the conditions are met*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnTryCloseDoor_Event();
	/*Function called when the conditions to close the door are met when TryCloseDoor event is called*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnStartCloseDoor_Event();
	/*Function called each time the door closing animation is updated, called by UptadeTimeline*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnUpdateCloseDoor_Event();
	/*Function called when the closing animation is over and the door is fully closed, called by EndTimeline*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AirlockSystem|Events")
		void OnEndCloseDoor_Event();

#pragma endregion

	/*Door Timeline, used to animate(move / rotate) the door mesh over time when triggered*/
	FTimeline DoorTL;

	/*Starting value (location or rotation depending on the DoorType) taken when the animation starts. Going to be interpolated to the AnimationValueGoal*/
	UPROPERTY(EditAnywhere, DisplayName = "AnimationValueStart", Category = "AirlockSystem")
		float m_fAnimationValueStart = 0.f;

	/*End value that the animation is reaching by interpolation from AnimationValueStart*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "AnimationValueGoal", Category = "AirlockSystem")
		float m_fAnimationValueGoal = 0.f;

private:

	/*Type of the door which determine if the door open and close by rotation, elevation or slide (can be extended)*/
	UPROPERTY(EditAnywhere, DisplayName = "Door Type", Category = "AirlockSystem")
		EDoorType m_eDoorType = EDoorType::ERotatingDT;

	/*Do the player wants to open this door ? Used to request the door to open and prevent it to close*/
	UPROPERTY(VisibleAnywhere, DisplayName = "Want To Open ?", Category = "AirlockSystem", meta = (ToolTip = "Do the player wants to open this door ? Used to request the door to open and prevent it to close"))
		bool m_bWantToOpen = false;

	/*Is the door open ? Assigned but not used*/
	UPROPERTY(VisibleAnywhere, DisplayName = "Is Open ?", Category = "AirlockSystem", meta = (ToolTip = "Is the door open ? Used???"))
		bool m_bIsOpen = false;

	void InitDoorButton();

#pragma region TIMELINE

	/*Curve that goes from 0 to 1 in 2 seconds by default. Used to interpolate the AnimationValueStart to AnimationValueGoal in the timeline*/
	UPROPERTY(EditAnywhere, Category = "AirlockSystem", meta = (ToolTip = "Curve that goes from 0 to 1 in 2 seconds by default. Used to interpolate the AnimationValueStart to AnimationValueGoal in the timeline"))
		class UCurveFloat* AnimCurve;

	/*Call UpdateOpenDoor or UpdateCloseDoor depending on the Timeline direction, at the end of each DoorTL's update by it's PostUpdateFunc*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem", meta = (ToolTip = "Call UpdateOpenDoor or UpdateCloseDoor depending on the Timeline direction, each time the timeline is updated"))
		void UpdateTimeline();

	/*Function that will be bound to the UpdateTimeline event if the DoorType is Rotating*/
	UFUNCTION()
		void RotateDoor(float delta);

	/*Function that will be bound to the UpdateTimeline event if the DoorType is Sliding*/
	UFUNCTION()
		void SlideDoor(float delta);

	/*Function that will be bound to the UpdateTimeline event if the DoorType is Elevating*/
	UFUNCTION()
		void ElevateDoor(float delta);

	/*Call EndOpenDoor or EndCloseDoor depending on the Timeline direction, when the timeline ends*/
	UFUNCTION(BlueprintCallable, Category = "AirlockSystem", meta = (ToolTip = "Call EndOpenDoor or EndCloseDoor depending on the Timeline direction, when the timeline ends"))
		void EndTimeline();

	/*Function to initialize DoorTL properties and delegates*/
	void InitTL();

	/*The direction the DoorTL is going*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TEnumAsByte<ETimelineDirection::Type> TLDirection = ETimelineDirection::Forward;

#pragma endregion

#pragma region DEBUG
	inline void PrintScreen(FString string, float time = 0, FColor color = FColor::Red);
	inline void PrintBoundFunctionsToDelegates();
	inline void RemoveBoundFunctionsFromDelegates(const UObject* from);
#pragma endregion
};