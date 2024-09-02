// Copyright Juan Pablo Hernandez Mosti
#pragma once

#include "CoreMinimal.h"
#include "Notification.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NotificationSystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FNotificationSystemAction, FNotificationHandle, Handle);
using FNotificationSystemActionNative = TDelegate<void(FNotificationHandle)>;

/*
 * Notification system to handle creation, clear and holding of the notifications 
 */
UCLASS()
class NOTIFICATIONSYSTEMMODULE_API UNotificationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UNotificationSystem();
	
	static UNotificationSystem* Get(const UObject* WorldContext);

	/// Creates a notification with a payload, this will trigger OnNotificationAdded
	/// @param PayloadStruct The payload to be added to the notification 
	/// @param OutMessageHandle Out handle identifier for the notification
	/// @return The created notification
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "PayloadStruct"))
	UNotification* CreateNotification(UScriptStruct* PayloadStruct, FNotificationHandle& OutMessageHandle);
	DECLARE_FUNCTION(execCreateNotification);
	
	/// Native version of CreateNotification (meant to be used in C++), this will trigger OnNotificationAdded
	/// @tparam UStruct The type of the notification's payload
	/// @param Payload The payload to be added to the notification
	/// @param OutMessageHandle Out handle identifier for the notification
	/// @return The created notification
	template <typename UStruct>
	UNotification* CreateNotification(UStruct& Payload, FNotificationHandle& OutMessageHandle);

	/// Get notification by handle
	/// @param Handle Handle must be valid
	/// @return notification if found, null in case handle is not valid
	UFUNCTION(BlueprintCallable)
	UNotification* GetNotification(const FNotificationHandle& Handle) const;

	/// Native version of GetNotificationHandlesByType
	/// @tparam UStruct The type of the payload
	/// @return Array with all notifications of type UStruct found
	template <typename UStruct>
	TArray<FNotificationHandle> GetNotificationHandlesByType() const;

	/// GetNotificationHandlesByType
	/// @param PayloadStruct The type of the payload
	/// @return Array with all notifications of type UStruct found
	UFUNCTION(BlueprintCallable)
	TArray<FNotificationHandle> GetNotificationHandlesByType(UScriptStruct* PayloadStruct) const;

	/// GetNotificationsByHandles
	/// @param Handles Array of handles
	/// @return Notifications found by the handles
	UFUNCTION(BlueprintCallable)
	TArray<UNotification*> GetNotificationsByHandles(const TArray<FNotificationHandle>& Handles);

	/// ClearNotification, this will trigger OnNotificationCleared
	/// @param Handle Handle of the notification to be cleared
	/// @return If it succeeded or not
	UFUNCTION(BlueprintCallable)
	bool ClearNotification(const FNotificationHandle& Handle);

	/// Check by handle if the notification exists
	/// @param Handle 
	/// @return 
	UFUNCTION(BlueprintCallable)
	bool ExistsNotification(const FNotificationHandle& Handle) const;

	/// Assign a callback for when the notification is added
	/// @param PayloadType The type of the payload to subscriber to, NOTE: You can receive the event for everyone if left nullptr
	/// @param Action The callback to be called.
	UFUNCTION(BlueprintCallable)
	void AssignOnNotificationAdded(const UScriptStruct* PayloadType, FNotificationSystemAction Action);
	template <typename UStruct>
	void AssignOnNotificationAddedNative(FNotificationSystemActionNative Action);

	/// Assign a callback for when a notification is cleared
	/// @param PayloadType The type of the payload to subscriber to, NOTE: You can receive the event for everyone if left nullptr
	/// @param Action The callback to be called. 
	UFUNCTION(BlueprintCallable)
	void AssignOnNotificationCleared(const UScriptStruct* PayloadType, FNotificationSystemAction Action);
	template <typename UStruct>
	void AssignOnNotificationClearedNative(FNotificationSystemActionNative Action);
private:

	using FNotificationCallback = TMulticastDelegate<void(FNotificationHandle)>;
	UPROPERTY(Transient)
	TMap<FNotificationHandle, UNotification*> Notifications;
	TMap<FName, FNotificationCallback> OnNotificationAdded;
	TMap<FName, FNotificationCallback> OnNotificationCleared;

	void BroadcastOnNotificationAdded(const FName& Name, const FNotificationHandle& Handle);
	void BroadcastOnNotificationCleared(const FName& Name, const FNotificationHandle& Handle);

	void AssignOnNotificationAddedImplementation(const FName& PayloadTypeName, const FNotificationSystemActionNative&& Action);
	void AssignOnNotificationClearedImplementation(const FName& PayloadTypeName, const FNotificationSystemActionNative&& Action);

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

template <typename UStruct>
void UNotificationSystem::AssignOnNotificationAddedNative(FNotificationSystemActionNative Action)
{
	AssignOnNotificationAddedImplementation(UStruct::StaticStruct()->GetFName(), MoveTemp(Action));
}

template <typename UStruct>
void UNotificationSystem::AssignOnNotificationClearedNative(FNotificationSystemActionNative Action)
{
	AssignOnNotificationClearedImplementation(UStruct::StaticStruct()->GetFName(), MoveTemp(Action));
}
