// Copyright 2017-2018 Paul Murray GPLv3
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MCHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMCHUD : public AHUD
{
	GENERATED_BODY()
	

public:
    //primary draw call
    virtual void DrawHUD() override;

    UPROPERTY(BlueprintReadWrite)
    bool ShowCrosshairs = true;

protected:
    //this will be drawn at center screen
    UPROPERTY(EditDefaultsOnly)
    UTexture2D* CrossHairTexture;


	
	
};
