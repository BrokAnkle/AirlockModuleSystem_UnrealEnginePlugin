// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name. You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "AirlockModuleControllerBase.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/Button.h"

#include "AirlockDoorBase.h"
#include "Widgets/UWAirlockModulePanel.h"
#include "Widgets/UWAirlockModuleUnlockButton.h"
#include "Widgets/UWStartPressurizationButton.h"

// Sets default values
AAirlockModuleControllerBase::AAirlockModuleControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region COMPONENTS CREATION

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	LightsPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LightsPivot"));
	LightsPivot->SetupAttachment(RootComponent);
	LightsPivot->bWantsInitializeComponent = true;
	LightsPivot->SetRelativeLocation(FVector(2, 0, 40));


	Light1 = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light1"));
	Light1->SetupAttachment(LightsPivot);
	Light1->SetRelativeRotation(FRotator(0, 90, 0));
	Light1->OuterConeAngle = 60;
	Light1->SetLightColor(FLinearColor::Yellow);

	Light2 = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light2"));
	Light2->SetupAttachment(LightsPivot);
	Light2->SetRelativeRotation(FRotator(0, -90, 0));
	Light2->OuterConeAngle = 60;
	Light2->SetLightColor(FLinearColor::Yellow);

	PanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PannelMesh"));
	PanelMesh->SetupAttachment(RootComponent);
	PanelMesh->SetRelativeLocation(FVector(0, -20, 0));
	PanelMesh->SetRelativeRotation(FRotator(0, 90, 0));
	PanelMesh->SetRelativeScale3D(FVector(.1, .1, .1));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PanelMeshFinder(TEXT("/Game/StarterContent/Architecture/Wall_400x400"));
	if (PanelMeshFinder.Succeeded())
		PanelMesh->SetStaticMesh(PanelMeshFinder.Object);

	PanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PannelWidget"));
	PanelWidget->SetupAttachment(PanelMesh);
	PanelWidget->SetRelativeLocation(FVector(200, -15, 180));
	PanelWidget->SetRelativeRotation(FRotator(0, -90, 0));
	PanelWidget->SetRelativeScale3D(FVector(2, 2, 2));
	static ConstructorHelpers::FClassFinder<UUserWidget> PanelWidgetFinder(TEXT("/Game/CPP/Widgets/MyModulePanel"));
	if (PanelWidgetFinder.Succeeded())
		PanelWidget->SetWidgetClass(PanelWidgetFinder.Class);

#pragma endregion

}

// Called when the game starts or when spawned
void AAirlockModuleControllerBase::BeginPlay()
{
	Super::BeginPlay();

	RemoveBoundFunctionsFromDelegates(this);	//Stack overflow bug provoked by Broadcasting a delegate patch

	InitDoors();
	InitPanel();
}

// Called every frame
void AAirlockModuleControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bIsPressurizing)
		OnUpdatePressurization_Event();
	else if (m_bIsOpen)
		LightsPivot->AddRelativeRotation(FRotator(0, m_fLightRotationSpeed * DeltaTime, 0));

	//Debug
	//PrintBoundFunctionsToDelegates()
}

//Construction script
void AAirlockModuleControllerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for (auto door : Doors)
		door->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

#pragma region MODULE EVENTS

//Event triggered when one the door in the Doors array Broadcast its OpenDoor_Delegate
void AAirlockModuleControllerBase::OnDoorOpen_Event_Implementation()
{
	m_bIsOpen = true;
	m_bCanBeOpen = false;
	
	Light1->SetLightColor(FLinearColor(FColor::Orange));
	Light2->SetLightColor(FLinearColor(FColor::Orange));

	DoorOpen_Delegate.Broadcast();
}

//Event triggered when the open door in the Doors Broadcast its DoorClosed_Delegate
void AAirlockModuleControllerBase::OnDoorClosed_Event_Implementation()
{
	m_bIsOpen = false;

	Light1->SetLightColor(FLinearColor(FColor::Red));
	Light2->SetLightColor(FLinearColor(FColor::Red));

	DoorClosed_Delegate.Broadcast();

	OnLocked_Event(); //in the base case, closing a door also locks the module
}

//Event triggered by OnDoorClosed_Event
void AAirlockModuleControllerBase::OnLocked_Event_Implementation()
{
	m_bIsLocked = true;
	m_bCanBeUnlocked = false;
	
	//If the module launch autmatically the pressurization, launch now
	if (m_bAutoPressurize)
		OnStartPressurization_Event();

	Locked_Delegate.Broadcast();
}

//Event triggered either by the OnLocked_Event if the module is AutoPressurize, or by the Start Pressurization button widget
void AAirlockModuleControllerBase::OnStartPressurization_Event_Implementation()
{
	m_fPressurizationRemainingTime = m_fPressurizationTime;
	m_bIsPressurizing = true;

	StartPressurization_Delegate.Broadcast();
}

//Event called each frame when m_bIsPressurizing is true, decrement the remaining time of pressurizing then calls OnEndPressurization_Event when it reach 0
void AAirlockModuleControllerBase::OnUpdatePressurization_Event_Implementation()
{
	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	LightsPivot->AddRelativeRotation(FRotator(0, m_fLightRotationSpeed * deltaTime, 0));
	m_fPressurizationRemainingTime -= deltaTime;

	UpdatePressurization_Delegate.Broadcast();

	if (m_fPressurizationRemainingTime <= 0.f)
		OnEndPressurization_Event();
}

//Event triggered as soon as the pressurization timer reach 0
void AAirlockModuleControllerBase::OnEndPressurization_Event_Implementation()
{
	m_bCanBeUnlocked = true;
	m_bIsPressurizing = false;

	EndPressurization_Delegate.Broadcast();
}

//Event called when an action required to unlock the module is performed, may result in a failure or success depending on the Unlock conditions
void AAirlockModuleControllerBase::OnTryUnlock_Event_Implementation()
{
	TryUnlock_Delegate.Broadcast();	//To prevent the Unlock success, bound to TryUnlock_Delegate a function that will check a condition, set m_bCanBeUnlocked to false if the condition is not met

	if (m_bCanBeUnlocked)	//After checking condition, the module still can be unlocked
		OnUnlocked_Event();
	else	//After checking condition, the module finally not be unlocked
		OnFailUnlock_Event();
}

//Event triggered by OnTryUnlock_Event, when the module can't be unlocked
void AAirlockModuleControllerBase::OnFailUnlock_Event_Implementation()
{
	UnlockFailed_Delegate.Broadcast();
}

//Event triggered by OnTryUnlock_Event, when the module can be unlocked
void AAirlockModuleControllerBase::OnUnlocked_Event_Implementation()
{
	m_bCanBeOpen = true;
	m_bIsLocked = false;

	Light1->SetLightColor(FLinearColor(FColor::Green));
	Light2->SetLightColor(FLinearColor(FColor::Green));

	Unlocked_Delegate.Broadcast();
}

//Method called in the BeginPlay
void AAirlockModuleControllerBase::InitDoors_Implementation()
{
	for (auto door : Doors)
	{
		door->ModuleOwner = this;
		door->StartOpenDoor_Delegate.AddUniqueDynamic
		(this, &AAirlockModuleControllerBase::OnDoorOpen_Event);
		door->EndCloseDoor_Delegate.AddUniqueDynamic
		(this, &AAirlockModuleControllerBase::OnDoorClosed_Event);

		if (m_bAutoPressurize)
			door->DestroyOpenButton();
	}
}

//Method called in the BeginPlay
void AAirlockModuleControllerBase::InitPanel_Implementation()
{
	UUWAirlockModulePanel* PanelRef = Cast<UUWAirlockModulePanel>(PanelWidget->GetWidget());

	if (IsValid(PanelRef))
	{
		PanelRef->InitPanel(this);

		PanelRef->GetPressurizeButton()->GetButton()->OnClicked.AddUniqueDynamic
		(this, &AAirlockModuleControllerBase::TriggerStartPressurizationEvent);

		switch (m_eUnlockMethod)
		{
		case EModuleUnlockMethod::ETimeMUM:
			break;
		case EModuleUnlockMethod::EButtonMUM:
			PanelRef->GetUnlockButton()->GetButton()->OnClicked.AddUniqueDynamic
			(this, &AAirlockModuleControllerBase::TriggerTryUnlockEvent);
			break;
		case EModuleUnlockMethod::ECodeMUM:
			if (m_tCode.IsEmptyOrWhitespace())
				m_eUnlockMethod = EModuleUnlockMethod::ETimeMUM;
			break;
		default:break;
		}
	}
}

#pragma endregion

#pragma region DEBUG

void AAirlockModuleControllerBase::PrintScreen(FString string, float time, FColor color)
{
	GEngine->AddOnScreenDebugMessage(-1, time, color, string);
}

void AAirlockModuleControllerBase::PrintBoundFunctionsToDelegates()
{
	for (auto it : DoorOpen_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Open door : " + it->GetName());
	for (auto it : DoorClosed_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Close door : " + it->GetName());
	for (auto it : Locked_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Locked : " + it->GetName());
	for (auto it : StartPressurization_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Start pressurize : " + it->GetName());
	for (auto it : UpdatePressurization_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Update pressurize : " + it->GetName());
	for (auto it : EndPressurization_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("End pressurize : " + it->GetName());
	for (auto it : TryUnlock_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Try unlock : " + it->GetName());
	for (auto it : UnlockFailed_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Fail unlock : " + it->GetName());
	for (auto it : Unlocked_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Unlocked : " + it->GetName());
}

void AAirlockModuleControllerBase::RemoveBoundFunctionsFromDelegates(const UObject* from)
{
	DoorOpen_Delegate.RemoveAll(this);
	DoorClosed_Delegate.RemoveAll(this);
	Locked_Delegate.RemoveAll(this);
	StartPressurization_Delegate.RemoveAll(this);
	UpdatePressurization_Delegate.RemoveAll(this);
	EndPressurization_Delegate.RemoveAll(this);
	TryUnlock_Delegate.RemoveAll(this);
	UnlockFailed_Delegate.RemoveAll(this);
	Unlocked_Delegate.RemoveAll(this);
}

#pragma endregion