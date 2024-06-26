// Shoot Them Up demo game project. Evgenii Esaulenko, 2024

#include "Components/STUCameraZoomComponent.h"

USTUCameraZoomComponent::USTUCameraZoomComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USTUCameraZoomComponent::BeginPlay()
{
    Super::BeginPlay();

    DefaultFieldOfView = CameraFOV;
    DefaultCameraSensitivity = CameraSensitivity;

    APlayerCameraManager* CameraManager = GetPlayerCameraManager();
    if (CameraManager != nullptr)
    {
        PlayerCameraManager = CameraManager;
        PlayerCameraManager->SetFOV(CameraFOV);
    }
}

APlayerCameraManager* USTUCameraZoomComponent::GetPlayerCameraManager() const
{
    const APlayerController* PlayerController = Cast<APawn>(GetOwner())->GetController<APlayerController>();
    if (PlayerController != nullptr)
    {
        return PlayerController->PlayerCameraManager;
    }

    return nullptr;
}

void USTUCameraZoomComponent::ResetCameraFOV()
{
    bZooming = false;
    
    if (PlayerCameraManager != nullptr)
    {
        PlayerCameraManager->SetFOV(DefaultFieldOfView);
    }
}

void USTUCameraZoomComponent::ZoomIn(float TargetFOV)
{
    bZooming = true;
    TargetFieldOfView = TargetFOV;
    UE_LOG(LogTemp, Warning, TEXT("USTUCameraZoomComponent::ZoomIn: TargetFOV = %f"), TargetFOV);
}

void USTUCameraZoomComponent::ZoomOut()
{
    bZooming = true;
    TargetFieldOfView = DefaultFieldOfView;
    UE_LOG(LogTemp, Warning, TEXT("USTUCameraZoomComponent::ZoomOut: TargetFOV = %f"), TargetFieldOfView);
}

void USTUCameraZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (PlayerCameraManager == nullptr || !IsValid(PlayerCameraManager))
    {
        PlayerCameraManager = GetPlayerCameraManager();
    }

    if (bZooming)
    {
        const float CurrentFOV = PlayerCameraManager->GetFOVAngle();
        const float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFieldOfView, DeltaTime, ZoomSpeed);

        PlayerCameraManager->SetFOV(NewFOV);

        if (FMath::IsNearlyEqual(NewFOV, TargetFieldOfView, 0.01f))
        {
            bZooming = false;
        }
    }
}
