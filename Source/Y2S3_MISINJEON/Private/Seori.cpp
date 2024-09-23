// Fill out your copyright notice in the Description page of Project Settings.


#include "Seori.h"
#include "Interactable.h"
#include "NPC.h"

// Sets default values
ASeori::ASeori()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	inventory.Init(0, INVENTORY_SIZE);

	inventory[0] = 0;
}

// Called when the game starts or when spawned
void ASeori::BeginPlay()
{
	Super::BeginPlay();
	capsuleComponent = GetCapsuleComponent();
	capsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ASeori::OnOverlapBegin);
	capsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ASeori::OnOverlapEnd);

	PlayerCamera = Cast<UCameraComponent>(GetDefaultSubobjectByName("Camera"));
	if (PlayerCamera == nullptr)
		UE_LOG(LogTemp, Log, TEXT("Can't Find Camera"));
}

// Called every frame
void ASeori::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASeori::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASeori::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->Tags.Contains("Interactable")) {
		//UE_LOG(LogTemp, Log, TEXT("Can Interact"));
		canInteract = true;
	}
		
		
}

void ASeori::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->Tags.Contains("Interactable")) {
		//UE_LOG(LogTemp, Log, TEXT("Can't Interact"));
		canInteract = false;
	}
		
}

void ASeori::Interact() {
	if (!canInteract) return;
	FHitResult HitResult;

	FVector StartTrace = PlayerCamera->GetComponentLocation();
	FVector EndTrace = StartTrace + (PlayerCamera->GetForwardVector() * 1000);
	FCollisionQueryParams traceParams;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, traceParams);
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 2.0f);

	if (HitResult.GetActor() != nullptr) {
		
		AActor* target = HitResult.GetActor();

		IInteractable* targetInteract = Cast<IInteractable>(target);
		if (targetInteract == nullptr)return;
		
		InteractType type = targetInteract->getType();
		switch (type)
		{
		case InteractType::NONE:
			//UE_LOG(LogTemp, Log, TEXT("This is Exception"));
			break;
		case InteractType::STATIC:
			//UE_LOG(LogTemp, Log, TEXT("Interact Static"));
			break;
		case InteractType::ITEM:
			// 인벤토리에 추가 or 인벤토리가 꽉찼습니다 처리
			//UE_LOG(LogTemp, Log, TEXT("Picked Item"));
			break;
		case InteractType::NPC:
			// 대화하기
			ANPC* npc = Cast<ANPC>(targetInteract);
			FVector cameraPos = npc->getCameraPos();
			break;
		}
		
	}
}