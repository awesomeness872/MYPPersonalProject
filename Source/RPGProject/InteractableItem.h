// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableItem.generated.h"

UCLASS()
class RPGPROJECT_API AInteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
		FName ActionText;

	//called manually, returns ActionText
	UFUNCTION(BlueprintCallable)
		FName GetActionText();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Activate();

	UFUNCTION()
		void Activate_Implementation();
	
};
