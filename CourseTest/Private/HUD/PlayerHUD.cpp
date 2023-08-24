#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"

void APlayerHUD::BeginPlay()
{
	Overlay = CreateWidget<UPlayerOverlay>(GetWorld()->GetFirstPlayerController(), ClassPlayerOverlay);

	if (Overlay)
	{
		Overlay->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlay not set!"));
	}
}
