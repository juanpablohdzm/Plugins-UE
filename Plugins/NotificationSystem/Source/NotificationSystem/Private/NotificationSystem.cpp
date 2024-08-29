#include "NotificationSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogNotificationSystem, All, All)

UNotificationSystem* UNotificationSystem::Get(const UObject* WorldContext)
{
	if (const UWorld* world = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
	{
		return UGameInstance::GetSubsystem<UNotificationSystem>(world->GetGameInstance());
	}

	UE_LOG(LogNotificationSystem, Warning, TEXT("Can not get subsystem because Uworld pointer is null"));
	return nullptr;
}

DEFINE_FUNCTION(UNotificationSystem::execCreateNotification)
{
	Stack.Step(Stack.Object, nullptr);
	FStructProperty* structProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	uint8* structMemoryStart = Stack.MostRecentPropertyAddress;
	P_GET_STRUCT_REF(FNotificationHandle, OutMessageHandle);
	P_FINISH;

	P_NATIVE_BEGIN
	*(UNotification**)RESULT_PARAM = P_THIS->CreateNotification(structProperty->Struct, structMemoryStart, OutMessageHandle);
	P_NATIVE_END
}

UNotification* UNotificationSystem::GetNotification(const FNotificationHandle& Handle) const
{
	if (ExistsNotification(Handle))
	{
		return Notifications[Handle];
	}
	return nullptr;
}

TArray<FNotificationHandle> UNotificationSystem::GetNotificationHandlesByType(UScriptStruct* PayloadStruct) const
{
	TArray<FNotificationHandle> outHandles;
	for (const auto& pair : Notifications)
	{
		if (pair.Value->IsPayloadOfType(PayloadStruct))
		{
			outHandles.Add(pair.Key);
		}
	}
	return outHandles;
}

TArray<UNotification*> UNotificationSystem::GetNotificationsByHandles(const TArray<FNotificationHandle>& Handles)
{
	TArray<UNotification*> outNotifications;
	for (const FNotificationHandle& handle : Handles)
	{
		if (UNotification* notification = GetNotification(handle))
		{
			outNotifications.Add(notification);
		}
	}
	return outNotifications;
}

bool UNotificationSystem::ClearNotification(const FNotificationHandle& Handle)
{
	if (!ExistsNotification(Handle))
	{
		return false;
	}

	const FName notificationType = Notifications[Handle]->GetPayloadTypeName();
	Notifications.Remove(Handle);

	BroadcastOnNotificationCleared(notificationType, Handle);

	return true;
}

bool UNotificationSystem::ExistsNotification(const FNotificationHandle& Handle) const
{
	return Notifications.Contains(Handle);
}

void UNotificationSystem::AssignOnNotificationAdded(const UScriptStruct* PayloadType, FNotificationSystemAction Action)
{
	const FName payloadTypeName = PayloadType ? PayloadType->GetFName() : FName(TEXT("All"));
	const FNotificationSystemActionNative nativeAction = FNotificationSystemActionNative::CreateUFunction(Action.GetUObject(), Action.GetFunctionName());
	if (!OnNotificationAdded.Contains(payloadTypeName))
	{
		FNotificationCallback callback;
		callback.Add(nativeAction);
		OnNotificationAdded.Add(payloadTypeName, callback);
	}
	else
	{
		OnNotificationAdded[payloadTypeName].Add(nativeAction);
	}
}

void UNotificationSystem::AssignOnNotificationCleared(const UScriptStruct* PayloadType, FNotificationSystemAction Action)
{
	const FName payloadTypeName = PayloadType ? PayloadType->GetFName() : FName(TEXT("All"));
	const FNotificationSystemActionNative nativeAction = FNotificationSystemActionNative::CreateUFunction(Action.GetUObject(), Action.GetFunctionName());
	if (!OnNotificationAdded.Contains(payloadTypeName))
	{
		FNotificationCallback callback;
		callback.Add(nativeAction);
		OnNotificationCleared.Add(payloadTypeName, callback);
	}
	else
	{
		OnNotificationCleared[payloadTypeName].Add(nativeAction);
	}
}

void UNotificationSystem::BroadcastOnNotificationAdded(const FName& Name, const FNotificationHandle& Handle)
{
	if (OnNotificationAdded.Contains(Name))
	{
		OnNotificationAdded[Name].Broadcast(Handle);
	}
	OnNotificationAdded[FName(TEXT("All"))].Broadcast(Handle);
}

void UNotificationSystem::BroadcastOnNotificationCleared(const FName& Name, const FNotificationHandle& Handle)
{
	if (OnNotificationCleared.Contains(Name))
	{
		OnNotificationAdded[Name].Broadcast(Handle);
	}
	OnNotificationCleared[FName(TEXT("All"))].Broadcast(Handle);
}

UNotification* UNotificationSystem::CreateNotification(UScriptStruct* ScriptStruct, uint8* StructData, FNotificationHandle& OutNotificationHandle)
{
	UNotification* notification = UNotification::Create(ScriptStruct, StructData);
	const FName payloadTypeName = notification->GetPayloadTypeName();
	OutNotificationHandle = FNotificationHandle();

	Notifications.Add(OutNotificationHandle, notification);
	BroadcastOnNotificationAdded(payloadTypeName, OutNotificationHandle);

	return notification;
}
