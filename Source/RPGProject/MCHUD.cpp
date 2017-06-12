// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "MCHUD.h"


void AMCHUD::DrawHUD(){

    Super::DrawHUD();

    if (ShowCrosshairs){
        if (CrossHairTexture){
            //find center of canvas
            FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

            //offset by half the half of the texture's dimensions so the center of the texture alligns with the center of the screen
            FVector2D CrossHairDrawPosition(Center.X - (CrossHairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrossHairTexture->GetSurfaceHeight() * 0.5f));

            //draw cross hair at center point
            FCanvasTileItem TileItem(CrossHairDrawPosition, CrossHairTexture->Resource, FLinearColor::White);
            TileItem.BlendMode = SE_BLEND_Translucent;
            Canvas->DrawItem(TileItem);
        }
    }
}
