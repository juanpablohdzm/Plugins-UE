// Copyright Juan Pablo Hernandez Mosti
#pragma once

#include "CoreMinimal.h"
#include "Notification.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NotificationSystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FNotificationSystemAction, FNotificationHandle, Handle);
using FNotificationSystemActionNative = TDelegate<void(FNotificationHandle)>;

UCLASS()
class NOTIFICATIONSYSTEMMODULE_API UNotificationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UNotificationSystem();
	
	static UNotificationSystem* Get(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "PayloadStruct"))
	UNotification* CreateNotification(UScriptStruct* PayloadStruct, FNotificationHandle& OutMessageHandle);
	DECLARE_FUNCTION(execCreateNotification);
	template <typename UStruct>
	UNotification* CreateNotification(UStruct& Payload, FNotificationHandle& OutMessageHandle);

	UFUNCTION(BlueprintCallable)
	UNotification* GetNotification(const FNotificationHandle& Handle) const;

	template <typename UStruct>
	TArray<FNotificationHandle> GetNotificationHandlesByType() const;
	UFUNCTION(BlueprintCallable)
	TArray<FNotificationHandle> GetNotificationHandlesByType(UScriptStruct* PayloadStruct) const;

	UFUNCTION(BlueprintCallable)
	TArray<UNotification*> GetNotificationsByHandles(const TArray<FNotificationHandle>& Handles);

	UFUNCTION(BlueprintCallable)
	bool ClearNotification(const FNotificationHandle& Handle);

	UFUNCTION(BlueprintCallable)
	bool ExistsNotification(const FNotificationHandle& Handle) const;

	UFUNCTION(BlueprintCallable)
	void AssignOnNotificationAdded(const UScriptStruct* PayloadType, FNotificationSystemAction Action);

	UFUNCTION(BlueprintCallable)
	void AssignOnNotificationCleared(const UScriptStruct* PayloadType, FNotificationSystemAction Action);

private:

	using FNotificationCallback = TMulticastDelegate<void(FNotificationHandle)>;
	UPROPERTY(Transient)
	TMap<FNotificationHandle, UNotification*> Notifications;
	TMap<FName, FNotificationCallback> OnNotificationAdded;
	TMap<FName, FNotificationCallback> OnNotificationCleared;

	void BroadcastOnNotificationAdded(const FName& Name, const FNotificationHandle& Handle);
	void BroadcastOnNotificationCleared(const FName& Name, const FNotificationHandle& Handle);

	UNotification* CreateNotification(UScriptStruct* ScriptStruct, uint8* StructData, FNotificationHandle& OutNotificationHandle);
};

template <typename UStruct>
UNotification* UNotificationSystem::CreateNotification(UStruct& PayloadStruct, FNotificationHandle& OutMessageHandle)
{
	return CreateNotification(UStruct::StaticStruct(), reinterpret_cast<uint8*>(&PayloadStruct), OutMessageHandle);
}

template <typename UStruct>
TArray<FNotificationHandle> UNotificationSystem::GetNotificationHandlesByType() const
{
	return GetNotificationHandlesByType(UStruct::StaticStruct());
}