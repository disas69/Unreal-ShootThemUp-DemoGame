// Shoot Them Up demo game project. Evgenii Esaulenko, 2024

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Input/InputDataConfig.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    bIsSprinting = false;
    bIsMovingForward = false;
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    const APlayerController* PlayerController = Cast<APlayerController>(GetController());

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMappingContext, 0);

    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    Input->BindAction(InputDataConfig->Move, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Move);
    Input->BindAction(InputDataConfig->Look, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Look);
    // Input->BindAction(InputDataConfig->Fire, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Fire);
    Input->BindAction(InputDataConfig->Jump, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Jump);
    Input->BindAction(InputDataConfig->Sprint, ETriggerEvent::Started, this, &ASTUBaseCharacter::StartSprint);
    Input->BindAction(InputDataConfig->Sprint, ETriggerEvent::Completed, this, &ASTUBaseCharacter::StopSprint);
}

bool ASTUBaseCharacter::IsSprintingForward() const
{
    return bIsSprinting && bIsMovingForward && !GetVelocity().IsZero();
}

void ASTUBaseCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    bIsMovingForward = MovementVector.Y > 0.0f;

    const FRotator CameraRotation = Camera->GetComponentRotation();
    const FRotator CameraYawRotation(0, CameraRotation.Yaw, 0);

    // Get the forward direction based on the camera orientation and apply movement
    const FVector ForwardDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.Y);

    // Get the right direction based on the camera orientation and apply movement
    const FVector RightDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void ASTUBaseCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    const float TurnAmount = LookAxisVector.X * CameraMovementRate * CameraSensitivity * GetWorld()->GetDeltaSeconds();
    AddControllerYawInput(TurnAmount);

    const float LookAmount = LookAxisVector.Y * CameraMovementRate * CameraSensitivity * GetWorld()->GetDeltaSeconds();
    AddControllerPitchInput(LookAmount);
}

void ASTUBaseCharacter::StartSprint()
{
    bIsSprinting = true;
}

void ASTUBaseCharacter::StopSprint()
{
    bIsSprinting = false;
}