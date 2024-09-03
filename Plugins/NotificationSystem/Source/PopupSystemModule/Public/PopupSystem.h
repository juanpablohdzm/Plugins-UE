//Copyright Juan Pablo Hernandez Mosti
#pragma once

#include "CoreMinimal.h"
#include "NotificationSystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PopupSystem.generated.h"

class UNotification;

UENUM(BlueprintType)
enum class EPopupAction : uint8
{
	Accept,
	Decline,
	Cancel,
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FPopupActionCallback, EPopupAction, PopupAction);
/**
 * 
 */
UCLASS()
class POPUPSYSTEMMODULE_API UPopupSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UPopupSystem* Get(const UObject* WorldObject);

private:
	//TODO: Find a way to connect the modularity of UNotification with the Popup system, limiting the use of interfaces or rules in general

	UPROPERTY(Transient)
	TArray<UNotification*> NotificationQueue;
};
