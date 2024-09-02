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

	template <typename UStruct>
	void SubscribePopupToNotificationType() const;

private:

	UFUNCTION()
	
	
	UPROPERTY()
	UNotificationSystem* NotificationSystem;
	
	TArray<UNotification*> NotificationQueue;
};

template <typename UStruct>
void UPopupSystem::SubscribePopupToNotificationType() const
{
	check(NotificationSystem)
	NotificationSystem->AssignOnNotificationAdded(UStruct::StaticStruct(),FNotificationSystemActionNative::CreateUObject(this, ));
}
