// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Seori.h"
#include "Interactable.h"
#include "NPC.h"
#include "Item.h"
#include "MisinjeonPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "MisinjeonPlayerController.h"



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
		//UE_LOG(LogTemp, Log, TEXT("Can't Find Camera"));
	
	inventory[0] = 1; // Ƽ���� �⺻���� ������ ����
}

// Called every frame
void ASeori::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ü�� ���� �Լ� (TakeDamage �Լ� ����) 
void ASeori::TakeDamage(int DamageAmount)
{
	
	SeoriHP -= DamageAmount;
	if (SeoriHP < 0)
	{
		SeoriHP = 0;
	}

	//UE_LOG(LogTemp, Warning, TEXT("SeoriHP: %d, Damage : %d"), SeoriHP, DamageAmount);

	if (SeoriHP <= 0)
	{
<<<<<<< HEAD
		UE_LOG(LogTemp, Warning, TEXT("Seori is Dead!"));
		OnDead();
=======
		//UE_LOG(LogTemp, Warning, TEXT("Seori is Dead!"));
>>>>>>> origin/develop
	}
	// �������Ʈ���� ������ �Լ� ȣ��
	GetHealthUI();
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
	FVector StartTrace = FVector();
	FVector EndTrace = FVector();
	switch (camMode)
	{
	case CameraMode::TPS:
		StartTrace = PlayerCamera->GetComponentLocation();
		EndTrace = StartTrace + (PlayerCamera->GetForwardVector() * 1000);
		break;
	case CameraMode::CCTV:
		StartTrace = GetActorLocation();
		EndTrace = StartTrace + (GetActorForwardVector() * 1000);
		break;
	}
	
	FCollisionQueryParams traceParams;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, traceParams);
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 2.0f);

	if (HitResult.GetActor() != nullptr) {
		
		AActor* target = HitResult.GetActor();
		UE_LOG(LogTemp, Log, TEXT("%s"), *target->GetName());

		IInteractable* targetInteract = Cast<IInteractable>(target);
		if (targetInteract == nullptr) return;

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
			// �κ��丮�� �߰� or �κ��丮�� ��á���ϴ� ó��
			AItem* item = Cast<AItem>(targetInteract);
			for (int i = 0; i < INVENTORY_SIZE; i++) {
				if (inventory[i] != -1) continue;
				inventory[i] = item->getItemKey();
				break;
			}
			break;
			}
		case InteractType::NPC:{
			// ��ȭ�ϱ�
			ANPC* npc = Cast<ANPC>(targetInteract);
			// ī�޶� ��ǥ ���ϱ�
			FVector cameraPos = npc->getCameraPos();
			FVector LookPos = (npc->GetActorLocation() + GetActorLocation()) * 0.5f;
			FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(cameraPos,LookPos);

			//FVector seoriPos = npc->getSeoriPos();
			RestCameraPos = PlayerCamera->GetComponentLocation();
			Talking = true;

			TalkStart(cameraPos, lookRotation);
			break;
			}
		}
		targetInteract->Interact();
	}
}


void ASeori::TalkStart(FVector CameraPos , FRotator LookRotate) {
	//UE_LOG(LogTemp, Log, TEXT("SeoriPos : %d, %d, %d"), SeoriPos.X, SeoriPos.Y, SeoriPos.Z);
	//UE_LOG(LogTemp, Log, TEXT("Camera : %d, %d, %d"), CameraPos.X, CameraPos.Y, CameraPos.Z);
	PlayerCamera->SetWorldLocation(CameraPos);
	PlayerCamera->SetWorldRotation(LookRotate);
}


void ASeori::Talk(){
	state = ConversationState::TALK;
}

void ASeori::Listen() {
	state = ConversationState::LISTEN;
}

void ASeori::setTalking(bool tmp) {
	Talking = tmp;
	if (!Talking) {
		PlayerCamera->SetRelativeLocation(FVector(0,0,0));
		PlayerCamera->SetRelativeRotation(FRotator(-15.0, 0.0, 0.0));
		//SetActorRotation(ResetPlayerRot);
	}
}