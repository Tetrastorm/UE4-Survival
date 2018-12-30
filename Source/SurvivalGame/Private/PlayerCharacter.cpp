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
	PlayerCamera->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
	PlayerCamera->SetRelativeLocation(FVector(10.0, 20.0, 0.0));
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
}

// Public Function

//Getter

void APlayerCharacter::GetPlayerHUD()
{
	/*APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerCon -> MyHUD();*/
}

// Protected Function

//Virtual Function

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GLog->Log("PlayerCharacter Intialised");
	FRotator TestRotator = GetMesh()->GetSocketRotation("head");
	GLog->Log(TestRotator.ToString());

	GetWorldTimerManager().SetTimer(ManagerTimer, this, &APlayerCharacter::Manager, 0.5, true);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerCharacter::ForwardBackMovement);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerCharacter::RightLeftMovement);

	PlayerInputComponent->BindAxis("LookX", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookY", this, &APlayerCharacter::AddControllerPitchInput);

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
	FVector vDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(vDirection, fValue);
}

void APlayerCharacter::RightLeftMovement(float fValue)
{
	FVector vDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(vDirection, fValue);
}

// Action Function

void APlayerCharacter::ChangeCamera()
{
	GLog->Log("Camera changing mode");

	bIsFPS = !bIsFPS;

	if (bIsFPS)
	{
		PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("head"));
		PlayerCamera->SetRelativeRotation(FRotator::ZeroRotator);
		PlayerCamera->SetRelativeLocation(FPSCameraLocation);

		GLog->Log("Player Camera are in FPS mode");
	}
	else
	{
		PlayerCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PlayerCamera->SetRelativeRotation(FRotator::ZeroRotator);
		PlayerCamera->SetRelativeLocation(TPSCameraLocation);

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
		PlayerCamera->SetRelativeRotation(FRotator::ZeroRotator);
		PlayerCamera->SetRelativeLocation(FPSCameraLocation);
	}
	else
	{
		PlayerCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PlayerCamera->SetRelativeRotation(FRotator::ZeroRotator);
		PlayerCamera->SetRelativeLocation(TPSCameraLocation);
	}
}

void APlayerCharacter::ViewRaycast()
{
	FHitResult Hit;
	float fLenght = 500.0;

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

			GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Blue, TEXT("Ennemy Health : " + FString::SanitizeFloat(Character->GetHealth())));
			GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Blue, TEXT("Ennemy Stamina : " + FString::SanitizeFloat(Character->GetStamina())));
		}

		AItem* Item = Cast<AItem>(Hit.GetActor());
		
		if (Item)
		{
			GLog->Log("Item Detected");
			
			if (this->GetAction())
			{
				Item->PickUp(this);
				GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Blue, TEXT(" Added in inventory"));
			}
		}
	}

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 0.5, 0, 0.25);
}

// Manage Function

void APlayerCharacter::Manager()
{
	StaminaManager();
	//StealthManager();
	//ViewRaycast();
}