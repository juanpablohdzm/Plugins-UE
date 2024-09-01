//Copyright Juan Pablo Hernandez Mosti
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PopupSystem.generated.h"

/**
 * 
 */
UCLASS()
class POPUPSYSTEMMODULE_API UPopupSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UPopupSystem* Get(const UObject* WorldObject);
};
