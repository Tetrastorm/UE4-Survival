// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Item.h"
#include "DrawDebugHelpers.h"

// Construtor

APlayerCharacter::APlayerCharacter()
{
	FName HeadBone = TEXT("head");
	bIsFPS = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(GetMesh(), HeadBone);
	PlayerCamera->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
}

// Protected Function

//Virtual Function

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GLog->Log("PlayerCharacter Intialised");
	FRotator TestRotator = GetMesh()->GetSocketRotation("head");
	GLog->Log(TestRotator.ToString());

	GetWorldTimerManager().SetTimer(ManagerTimer, this, &APlayerCharacter::Manager, 0.5f, true);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerCharacter::ForwardBackMovement);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerCharacter::RightLeftMovement);

	PlayerInputComponent->BindAxis("LookX", this, &APlayerCharacter::CameraYaw);
	PlayerInputComponent->BindAxis("LookY", this, &APlayerCharacter::CameraPitch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);

	PlayerInputComponent->BindAction("DownPosition", IE_Pressed, this, &APlayerCharacter::DownPosition);
	PlayerInputComponent->BindAction("UpPosition", IE_Pressed, this, &APlayerCharacter::UpPosition);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &APlayerCharacter::StartAction);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &APlayerCharacter::StopAction);

	PlayerInputComponent->BindAction("WeaponSight", IE_Pressed, this, &APlayerCharacter::StartWeaponSight);
	PlayerInputComponent->BindAction("WeaponSight", IE_Released, this, &APlayerCharacter::StopWeaponSight);

	PlayerInputComponent->BindAction("View", IE_Pressed, this, &APlayerCharacter::ChangeCamera);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::StopAttack);

	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &APlayerCharacter::DropItem);
}

// Movement Function

void APlayerCharacter::ForwardBackMovement(float fValue)
{
	if (Controller != nullptr)
	{
		FRotator Rotation = Controller->GetControlRotation();
		Rotation.Pitch = 0.0f;
		FVector vDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(vDirection, fValue);
	}
}

void APlayerCharacter::RightLeftMovement(float fValue)
{
	if (Controller != nullptr)
	{
		FRotator Rotation = Controller->GetControlRotation();
		Rotation.Pitch = 0.0f;
		FVector vDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(vDirection, fValue);
	}
}

void APlayerCharacter::CameraPitch(float scale)
{
	if (bIsFPS)
	{
		AddControllerPitchInput(scale);
	}
	else
	{
		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			AddControllerPitchInput(scale);
		}
		else
		{
			
		}
	}
}

void APlayerCharacter::CameraYaw(float scale)
{
	if (bIsFPS)
	{
		AddControllerYawInput(scale);
	}
	else
	{
		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			AddControllerYawInput(scale);
		}
		else
		{

		}
	}
}

// Action Function

void APlayerCharacter::ChangeCamera()
{
	GLog->Log("Camera changing mode");
	bIsFPS = !bIsFPS;

	if (bIsFPS)
	{
		PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("head"));
		PlayerCamera->SetRelativeLocationAndRotation(FPSCameraLocation, FRotator::ZeroRotator);
		GLog->Log("Player Camera are in FPS mode");
	}
	else
	{
		PlayerCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PlayerCamera->SetRelativeLocationAndRotation(TPSCameraLocation, FRotator::ZeroRotator);
		GLog->Log("Player Camera are in TPS mode");
	}
	GLog->Log(PlayerCamera->GetAttachSocketName().ToString());
	GLog->Log(PlayerCamera->GetComponentLocation().ToString());
	GLog->Log(PlayerCamera->GetComponentRotation().ToString());
}

void APlayerCharacter::StartWeaponSight()
{
	if (WeaponEquiped)
	{
		SetIronsight(true);
		//PlayerCamera->AttachToComponent(WeaponEquiped->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponCam"));
	}
}

void APlayerCharacter::StopWeaponSight()
{
	SetIronsight(false);

	if (bIsFPS)
	{
		PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Head"));
		PlayerCamera->SetRelativeLocationAndRotation(FPSCameraLocation, FRotator::ZeroRotator);
	}
	else
	{
		PlayerCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PlayerCamera->SetRelativeLocationAndRotation(TPSCameraLocation, FRotator::ZeroRotator);
	}
}

void APlayerCharacter::ViewRaycast()
{
	FHitResult Hit;
	float fLenght = 500.0f;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FVector StartLocation = PlayerCamera->GetComponentLocation();
	FVector EndLocation = PlayerCamera->GetComponentLocation() + ((PlayerCamera->GetComponentRotation()).Vector() * fLenght);

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionParams);

	if (Hit.GetActor())
	{
		ANPC* Character = Cast<ANPC> (Hit.GetActor());

		if (Character)
		{
			GLog->Log("Character Detected");

			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, TEXT("Ennemy Health : " + FString::SanitizeFloat(Character->GetHealth())));
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, TEXT("Ennemy Stamina : " + FString::SanitizeFloat(Character->GetStamina())));
		}

		AItem* Item = Cast<AItem>(Hit.GetActor());
		
		if (Item)
		{
			GLog->Log("Item Detected");
			
			if (this->GetAction())
			{
				Item->PickUp(this);
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT(" Added in inventory"));
			}
		}
	}
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 0.5f, 0, 0.25f);
}

// Manage Function

void APlayerCharacter::Manager()
{
	StaminaManager();
	//StealthManager();
	//ViewRaycast();
}