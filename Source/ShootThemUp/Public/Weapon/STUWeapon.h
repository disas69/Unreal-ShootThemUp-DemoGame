// Shoot Them Up demo game project. Evgenii Esaulenko, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUWeapon.generated.h"

class ASTUGameHUD;

DECLARE_MULTICAST_DELEGATE(FOnClipEmpty);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bIsUnlimited"))
    int32 Clips;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsUnlimited = false;
};

USTRUCT(BlueprintType)
struct FWeaponImageData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTexture2D* PreviewImage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairImage = nullptr;
};

UCLASS()
class SHOOTTHEMUP_API ASTUWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUWeapon();

    FOnClipEmpty OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();
    void Reload();
    bool IsFullAmmo() const;
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FAmmoData& GetDefaultAmmoData() { return DefaultAmmo; }

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FWeaponImageData& GetWeaponImageData() { return WeaponImageData; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 35.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Range = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Rate = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FWeaponImageData WeaponImageData;
    
    virtual void BeginPlay() override;
    virtual void FireInternal();
    
    void TraceWeapon(const FVector& SocketLocation, FHitResult& HitResult, FVector& TraceEndLocation);
    void ApplyDamage(const FHitResult& HitResult);
    FTransform GetMuzzleSocketTransform() const;

    void DecreaseAmmo();

private:
    UPROPERTY()
    ACharacter* Character = nullptr;

    UPROPERTY()
    APlayerController* Controller = nullptr;

    FAmmoData CurrentAmmo;

    APlayerController* GetPlayerController();
};
