// Shoot Them Up demo game project. Evgenii Esaulenko, 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerStatsWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerName(const FText& Text);
    void SetKills(const FText& Text);
    void SetDeaths(const FText& Text);
    void SetTeamIcon(UTexture2D* Texture, const FLinearColor& Color);
    void SetPlayerIndicatorVisibility(bool bVisible);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsText;

    UPROPERTY(meta = (BindWidget))
    UImage* TeamIcon;
    
    UPROPERTY(meta = (BindWidget))
    UImage* PlayerIndicatorImage;
};
