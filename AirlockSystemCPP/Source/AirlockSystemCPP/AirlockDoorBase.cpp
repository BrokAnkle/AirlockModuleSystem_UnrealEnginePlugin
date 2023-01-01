// This code and project belongs to Arnaud GUYMONT. Modifications allowed but should not be redistributed as other name.
//You can submit pull request at the Github repo: https://github.com/BrokAnkle/AirlockModuleSystem_UnrealEnginePlugin


#include "AirlockDoorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "AirlockModuleControllerBase.h"
#include "Engine/EngineTypes.h"
#include "Widgets/UWOpenDoorButton.h"
#include "AirlockModuleControllerBase.h"

// Sets default values
AAirlockDoorBase::AAirlockDoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region COMPONENTS INITIALIZATION

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMeshFinder(TEXT("/Game/StarterContent/Props/SM_Door"));
	if (DoorMeshFinder.Succeeded())
		DoorMesh->SetStaticMesh(DoorMeshFinder.Object);

	OpenButton = CreateDefaultSubobject<UWidgetComponent>(TEXT("OpenButton"));
	OpenButton->SetupAttachment(DoorMesh);
	OpenButton->SetRelativeLocation(FVector(10, -80, 90));
	OpenButton->SetRelativeScale3D(FVector(.2, .2, .2));
	OpenButton->SetDrawSize(FVector2D(100, 50));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->InitBoxExtent(FVector(128, 45, 100));
	Collider->SetRelativeLocation(FVector(0, -45, 100));

	Collider->SetupAttachment(RootComponent);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetRelativeLocation(FVector(0, -50, 200));
	TextRender->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	TextRender->bHiddenInGame = true;
	TextRender->SetupAttachment(RootComponent);

#pragma endregion

	//Apply default AnimCurve from Game folder if found. Can be changed in the Editor after creation
	static ConstructorHelpers::FObjectFinder<UCurveFloat> AnimCurveFinder(TEXT("/Game/CPP/DoorAnimCurve"));
	if (AnimCurveFinder.Succeeded())
		AnimCurve = AnimCurveFinder.Object;	
}

// Called when the game starts or when spawned
void AAirlockDoorBase::BeginPlay()
{
	Super::BeginPlay();

	RemoveBoundFunctionsFromDelegates(this);	//Remove the bugged the bound function to the delegates that provokes a Stack Overflow when Broadcasted

	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAirlockDoorBase::OnColliderBeginOverlap);	//when the player enter the collision, says he want to open the door
	Collider->OnComponentEndOverlap.AddUniqueDynamic(this, &AAirlockDoorBase::OnColliderEndOverlap);	//when the player leaves the collision, say he doesn't want to open the door

	InitTL();	//Timeline initialization
	InitDoorButton(); //Give this door as reference to the door button owner
}

// Called every frame
void AAirlockDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bWantToOpen)
		OnTryOpenDoor_Event();

	DoorTL.TickTimeline(DeltaTime);

	//Debug
	//PrintBoundFunctionsToDelegates();
}

void AAirlockDoorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	TextRender->SetText(UEnum::GetDisplayValueAsText(m_eDoorType));
	TextRender->SetTextRenderColor(FColor::Blue);
}

void AAirlockDoorBase::DestroyOpenButton()
{
	OpenButton->DestroyComponent();
}

#pragma region COLLISIONS

//Bound to the Collision box's OnBeginOverlap, if the player overlaps it, he wan't to open this door
void AAirlockDoorBase::OnColliderBeginOverlap_Implementation
(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacter>(OtherActor) == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		m_bWantToOpen = true;
}

//Bound to the Collision box's OnEndOverlap, if the player leaves it, he doesn't want to open this door anymore
void AAirlockDoorBase::OnColliderEndOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacter>(OtherActor) == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		m_bWantToOpen = false;
}

//This function is called when the player exit the collider when the door has already finished to open. Triggers the OnTryCloseDoor_Event
void AAirlockDoorBase::CloseDoorOnEndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacter>(OtherActor) == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		OnTryCloseDoor_Event();
}

#pragma endregion

#pragma region DOOR EVENTS

//This function can be used to check a condition necessary to open the door
//Like a key in an inventory, a power switch to turn on, or any other objective
//In the base implementation, just check if the Module can be open
void AAirlockDoorBase::OnTryOpenDoor_Event_Implementation()
{
	TryOpenDoor_Delegate.Broadcast();
	if (IsValid(ModuleOwner))
		if (ModuleOwner->m_bCanBeOpen)
			OnStartOpenDoor_Event();
}

//Called in the tick when the player wants to open the door and the module allows it.
//Trigger the door open animation
void AAirlockDoorBase::OnStartOpenDoor_Event_Implementation()
{
	m_bIsOpen = true;
	TLDirection = ETimelineDirection::Forward;
	DoorTL.Play();
	StartOpenDoor_Delegate.Broadcast();
}

//This function is called every time the animation timeline is updated by its PostUpdateFunc delegate,
//Here, does nothing but can be used to customize more your Airlock System
void AAirlockDoorBase::OnUpdateOpenDoor_Event_Implementation()
{
	UpdateOpenDoor_Delegate.Broadcast();
}

//This function check if the player is still in the collider when the open animation is done
//If so, add CloseDoorOnEndOverlap to the collider's EndOverlap delegate
//else, Try to close the door
void AAirlockDoorBase::OnEndOpenDoor_Event_Implementation()
{
	if (!m_bWantToOpen) //When the door finish to open, if the player is still inside the collider, try to close the door when he exit it
		OnTryCloseDoor_Event();
	else	//Player is still inside the collider
		Collider->OnComponentEndOverlap.AddUniqueDynamic(this, &AAirlockDoorBase::CloseDoorOnEndOverlap);
	EndOpenDoor_Delegate.Broadcast();
}

//This function can be used to check a conditions necessary to close the door
//Here, just start to close the door
void AAirlockDoorBase::OnTryCloseDoor_Event_Implementation()
{
	if (Collider->OnComponentEndOverlap.Contains(this, FName("CloseDoorOnEndOverlap")))
		Collider->OnComponentEndOverlap.RemoveDynamic(this, &AAirlockDoorBase::CloseDoorOnEndOverlap);

	TryCloseDoor_Delegate.Broadcast();

	OnStartCloseDoor_Event();
}

//This function launch the close animation of the door
void AAirlockDoorBase::OnStartCloseDoor_Event_Implementation()
{
	TLDirection = ETimelineDirection::Backward;
	DoorTL.Reverse();

	StartCloseDoor_Delegate.Broadcast();
}

//This function is called each time the Door timeline is updated, by the PostUpdateFunc of the DoorTL
void AAirlockDoorBase::OnUpdateCloseDoor_Event_Implementation()
{
	UpdateCloseDoor_Delegate.Broadcast();
}

//Function called when the door just finished to close (closing animation done)
//The broadcast will tell the module to lock itself,
//preventing every door linked to it to not open until unlocked
void AAirlockDoorBase::OnEndCloseDoor_Event_Implementation()
{
	m_bIsOpen = false;
	EndCloseDoor_Delegate.Broadcast();
}

#pragma endregion

//Give this door as the owner of the OpenButton
void AAirlockDoorBase::InitDoorButton()
{
	UUWOpenDoorButton* b = Cast<UUWOpenDoorButton>(OpenButton->GetWidget());
	if (b)
		b->DoorOwner = this;
}

#pragma region TIMELINE

//Function called by the DoorTL's PostUpdateFunc, at the end of each update
void AAirlockDoorBase::UpdateTimeline()
{
	if (TLDirection == ETimelineDirection::Forward)
		OnUpdateOpenDoor_Event();

	else if (TLDirection == ETimelineDirection::Backward)
		OnUpdateCloseDoor_Event();
}

//Rotate the Door on the Z axis
void AAirlockDoorBase::RotateDoor(float delta)
{
	float NewZRotation = FMath::Lerp(m_fAnimationValueStart, m_fAnimationValueGoal, delta);	//Get the new Z rotation read by the Timeline in the curve
	FRotator rot = FRotator(DoorMesh->GetRelativeRotation());	//Temp rotation, duplicate of the current Relative rotation of the DoorMesh
	rot.Yaw = NewZRotation;	//Give the new Z rotation to the temp

	//Set the relative rotation of the DoorMesh the temp one
	DoorMesh->SetRelativeRotation(rot);
}

//Translate the Door on the Y axis
void AAirlockDoorBase::SlideDoor(float delta)
{
	float NewYLocation = FMath::Lerp(m_fAnimationValueStart, m_fAnimationValueGoal, delta);	//Get the new Y location read by the Timeline in the curve	
	FVector loc = FVector(DoorMesh->GetRelativeLocation());	//Temp location, duplicate of the current Relative location of the DoorMesh	
	loc.Y = NewYLocation;	//Give the new Y location to the temp

	//Set the relative location of the DoorMesh the temp one
	DoorMesh->SetRelativeLocation(loc);
}

//Translate the Door on the Z axis
void AAirlockDoorBase::ElevateDoor(float delta)
{
	float NewZLocation = FMath::Lerp(m_fAnimationValueStart, m_fAnimationValueGoal, delta);	//Get the new Z location read by the Timeline in the curve	
	FVector loc = FVector(DoorMesh->GetRelativeLocation());	//Temp location, duplicate of the current Relative location of the DoorMesh	
	loc.Z = NewZLocation;	//Give the new Z location to the temp

	//Set the relative location of the DoorMesh the temp one
	DoorMesh->SetRelativeLocation(loc);
}

void AAirlockDoorBase::EndTimeline()
{
	if (TLDirection == ETimelineDirection::Forward)
		OnEndOpenDoor_Event();
	else if (TLDirection == ETimelineDirection::Backward)
		OnEndCloseDoor_Event();
}


void AAirlockDoorBase::InitTL()
{
	if (AnimCurve)
	{
		FOnTimelineFloat TLFloatUpdate;
		DoorTL.SetDirectionPropertyName(FName("TLDirection"));
		//Bind to the timeline the appropriate function according to the resulting case (ex: if doortype is rotating, bind the rotating function to the timeline)
		switch (m_eDoorType)
		{
		case EDoorType::ERotatingDT:
			TLFloatUpdate.BindDynamic(this, &AAirlockDoorBase::RotateDoor);
			m_fAnimationValueStart = DoorMesh->GetRelativeRotation().Yaw;
			if (m_fAnimationValueGoal == 0.f)	//if no goal was set to the rotation
				m_fAnimationValueGoal = 90.f;	//set the rotation goal to 90° by default
			break;
		case EDoorType::ESlidingDT:
			TLFloatUpdate.BindDynamic(this, &AAirlockDoorBase::SlideDoor);
			m_fAnimationValueStart = DoorMesh->GetRelativeLocation().Y;
			if (m_fAnimationValueGoal == 0.f)   //if no goal was set to the slide
				m_fAnimationValueGoal = 100.f;  //set the slide distance to 1 meter by default
			break;
		case EDoorType::EElevatingDT:
			TLFloatUpdate.BindDynamic(this, &AAirlockDoorBase::ElevateDoor);
			m_fAnimationValueStart = DoorMesh->GetRelativeLocation().Z;
			if (m_fAnimationValueGoal == 0.f)	//if no goal was set to the elevation
				m_fAnimationValueGoal = 200.f;	//set the elevation goal to 2 meters by default;
			break;
		}

		//Set the timeline to pass to TLFloatUpdate the AnimCurve value read over time
		DoorTL.AddInterpFloat(AnimCurve, TLFloatUpdate);

		//Regular function to bind to the Timeline triggered after each update
		FOnTimelineEvent TLUpdate{};
		TLUpdate.BindDynamic(this, &AAirlockDoorBase::UpdateTimeline);
		DoorTL.SetTimelinePostUpdateFunc(TLUpdate);

		//Event called when the timeline is finished
		FOnTimelineEvent TLFinished;
		TLFinished.BindDynamic(this, &AAirlockDoorBase::EndTimeline);
		DoorTL.SetTimelineFinishedFunc(TLFinished);
	}
}

#pragma endregion

#pragma region DEBUG

void AAirlockDoorBase::PrintScreen(FString string, float time, FColor color)
{
	GEngine->AddOnScreenDebugMessage(-1, time, color, string);
}

void AAirlockDoorBase::PrintBoundFunctionsToDelegates()
{
	for (auto it : TryOpenDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Try Open door : " + it->GetName());
	for (auto it : StartOpenDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Start open door : " + it->GetName());
	for (auto it : UpdateOpenDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Update open door : " + it->GetName());
	for (auto it : EndOpenDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("EndOpen door : " + it->GetName());
	for (auto it : TryCloseDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Try close door : " + it->GetName());
	for (auto it : StartCloseDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Start close door : " + it->GetName());
	for (auto it : UpdateCloseDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("Update close door : " + it->GetName());
	for (auto it : EndCloseDoor_Delegate.GetAllObjectsEvenIfUnreachable())
		PrintScreen("EndClose door : " + it->GetName());
}

void AAirlockDoorBase::RemoveBoundFunctionsFromDelegates(const UObject* from)
{
	//TryOpenDoor_Delegate.RemoveAll(this);
	StartOpenDoor_Delegate.RemoveAll(this);	//Only need this one
	/*UpdateOpenDoor_Delegate.RemoveAll(this);
	EndOpenDoor_Delegate.RemoveAll(this);
	TryCloseDoor_Delegate.RemoveAll(this);
	StartCloseDoor_Delegate.RemoveAll(this);
	UpdateCloseDoor_Delegate.RemoveAll(this);
	EndCloseDoor_Delegate.RemoveAll(this);*/
}



#pragma endregion
