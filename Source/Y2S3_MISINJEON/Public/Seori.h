// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Seori.generated.h"

UCLASS()
class Y2S3_MISINJEON_API ASeori : public ACharacter
{
	GENERATED_BODY()

	const int LIMIT_HP = 5;
	const int INVENTORY_SIZE = 5;

public:
	// Sets default values for this character's properties
	ASeori();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UCapsuleComponent* capsuleComponent;
	UCameraComponent* PlayerCamera;
	int HP = 5;
	TArray<int> inventory; // itemKey∏¶ ¿˙¿Â

	bool canInteract = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	bool isCanInteract() { return canInteract; };
	UFUNCTION(BlueprintCallable)
	void setCanInteract(bool tmp) { canInteract = tmp; };

	UFUNCTION(BlueprintCallable)
	void Interact();
	UFUNCTION(BlueprintCallable)
	TArray<int> getInventory() { return inventory; };

};
