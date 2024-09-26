// Fill out your copyright notice in the Description page of Project Settings.


#include "Seori.h"
#include "Interactable.h"
#include "NPC.h"
#include "Item.h"

// Sets default values
ASeori::ASeori()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	inventory.Init(-1, INVENTORY_SIZE);
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


// 체력 감소 함수 (TakeDamage 함수 구현) 
void ASeori::TakeDamage(int DamageAmount)
{
	HP -= DamageAmount;
	if (HP < 0)
	{
		HP = 0;
	}

	// 블루프린트에서 구현된 함수 호출
	UpdateHealthUI(HP);
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
		Talking = false;

	}
		
}

void ASeori::Interact() {
	if (!canInteract) return;
	if (Talking) return;
	FHitResult HitResult;

	FVector StartTrace = PlayerCamera->GetComponentLocation();
	FVector EndTrace = StartTrace + (PlayerCamera->GetForwardVector() * 1000);
	FCollisionQueryParams traceParams;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, traceParams);
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 2.0f);

	if (HitResult.GetActor() != nullptr) {
		
		AActor* target = HitResult.GetActor();

		IInteractable* targetInteract = Cast<IInteractable>(target);
		if (targetInteract == nullptr)return;
		
		InteractType type = targetInteract->getType();
		switch (type)
		{
		case InteractType::NONE: {
			//UE_LOG(LogTemp, Log, TEXT("This is Exception"));
			break;
			}
		case InteractType::STATIC: {
			//UE_LOG(LogTemp, Log, TEXT("Interact Static"));
			break;
			}
		case InteractType::ITEM:{
			// 인벤토리에 추가 or 인벤토리가 꽉찼습니다 처리
			AItem* item = Cast<AItem>(targetInteract);
			for (int i = 0; i < INVENTORY_SIZE; i++) {
				if (inventory[i] != -1) continue;
				inventory[i] = item->getItemKey();
				break;
			}
			break;
			}
		case InteractType::NPC:{
			// 대화하기
			ANPC* npc = Cast<ANPC>(targetInteract);
			FVector cameraPos = npc->getCameraPos();
			FVector seoriPos = npc->getSeoriPos();
			RestCameraPos = PlayerCamera->GetComponentLocation();
			Talking = true;
			TalkStart(cameraPos, seoriPos);
			break;
			}
		}
		targetInteract->Interact();
	}
}


void ASeori::TalkStart(FVector CameraPos, FVector SeoriPos) {
	UE_LOG(LogTemp, Log, TEXT("SeoriPos : %d, %d, %d"), SeoriPos.X, SeoriPos.Y, SeoriPos.Z);
	UE_LOG(LogTemp, Log, TEXT("Camera : %d, %d, %d"), CameraPos.X, CameraPos.Y, CameraPos.Z);
	PlayerCamera->SetWorldLocation(CameraPos);
	PlayerCamera->AddRelativeRotation(FQuat(FVector(0, 0, 1), -90));
}


void ASeori::Talk(){

}

void ASeori::Listen() {

}

void ASeori::setTalking(bool tmp) {
	Talking = tmp;
	if (!Talking) {
		PlayerCamera->SetWorldLocation(RestCameraPos);
		PlayerCamera->AddRelativeRotation(FQuat(FVector(0, 0, 1), 90));
	}
}