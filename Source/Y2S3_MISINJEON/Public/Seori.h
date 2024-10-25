// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "Camera/CameraComponent.h"
#include "Conversation.h"
#include "Seori.generated.h"
UENUM(BlueprintType)
enum class CameraMode : uint8 {
	TPS UMETA(DisplayName = "TPS"),
	CCTV UMETA(DisplayNmae = "CCTV")
};


UCLASS()
class Y2S3_MISINJEON_API ASeori : public ACharacter, public IConversation
{
	GENERATED_BODY()

	const int LIMIT_HP = 5;
	const int INVENTORY_SIZE = 9;

public:
	// Sets default values for this character's properties
	ASeori();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UCapsuleComponent* capsuleComponent;
	UCameraComponent* PlayerCamera;
	int HP = 5;

	
	//ü�� ���� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	int SeoriHP = LIMIT_HP;


	//�κ��丮
	TArray<int> inventory; // itemKey�� ����

	bool canInteract = false;
	bool Talking = false;
	FVector RestCameraPos;
	FRotator ResetPlayerRot;

	bool openTurnstile = false;
	CameraMode camMode = CameraMode::TPS;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TalkStart(FVector CameraPos, FRotator LookRotate);
	UFUNCTION(BlueprintCallable)
	virtual void Talk() override;
	UFUNCTION(BlueprintCallable)
	virtual void Listen() override;



	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(int DamageAmount);

	// ü�� UI ������Ʈ�� ���� �������Ʈ �Լ�
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	int GetHealthUI() {return SeoriHP;};

	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//CanInteract ���� ����
	UFUNCTION(BlueprintCallable)
	bool isCanInteract() { return canInteract; };
	UFUNCTION(BlueprintCallable)
	void setCanInteract(bool tmp) { canInteract = tmp; };

	UFUNCTION(BlueprintCallable)
	void Interact();

	// Inventory ���� ����
	UFUNCTION(BlueprintCallable)
	TArray<int> getInventory() { return inventory; };
	UFUNCTION(BlueprintCallable)
	void setInventoryItemKey(int index, int value) { inventory[index] = value; }


	UFUNCTION(BlueprintCallable)
	ConversationState getState() { return state; };
	// Talking ���� ����
	UFUNCTION(BlueprintCallable)
	bool isTalking() { return Talking; };
	UFUNCTION(BlueprintCallable)
	void setTalking(bool tmp);
	UFUNCTION(BlueprintImplementableEvent)
	void onTalking();


	UFUNCTION(BlueprintCallable)
	bool getOpenTurnstile() { return openTurnstile; }
	UFUNCTION(BlueprintCallable)
	void setOpenTurnstile(bool tmp) { openTurnstile = tmp; }

	UFUNCTION(BlueprintCallable)
	void setCameraMode(CameraMode tmp) { camMode = tmp; };
};