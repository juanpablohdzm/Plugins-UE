#include "NotificationSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogNotificationSystem, All, All)

UNotificationSystem::UNotificationSystem() :
OnNotificationAdded({{FName("All"), FNotificationCallback()}}),
OnNotificationCleared({{FName("All"), FNotificationCallback()}})
{
	
}

UNotificationSystem* UNotificationSystem::Get(const UObject* WorldContext)
{
	if (const UWorld* world = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
	{
		return UGameInstance::GetSubsystem<UNotificationSystem>(world->GetGameInstance());
	}

	UE_LOG(LogNotificationSystem, Warning, TEXT("Can not get subsystem because UWorld pointer is null"));
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
	FNotificationSystemActionNative nativeAction = FNotificationSystemActionNative::CreateUFunction(Action.GetUObject(), Action.GetFunctionName());
	AssignOnNotificationAddedImplementation(payloadTypeName, MoveTemp(nativeAction));
}

void UNotificationSystem::AssignOnNotificationCleared(const UScriptStruct* PayloadType, FNotificationSystemAction Action)
{
	const FName payloadTypeName = PayloadType ? PayloadType->GetFName() : FName(TEXT("All"));
	FNotificationSystemActionNative nativeAction = FNotificationSystemActionNative::CreateUFunction(Action.GetUObject(), Action.GetFunctionName());
	AssignOnNotificationClearedImplementation(payloadTypeName, MoveTemp(nativeAction));
}

void UNotificationSystem::BroadcastOnNotificationAdded(const FName& Name, const FNotificationHandle& Handle)
{
	if (OnNotificationAdded.Contains(Name) && OnNotificationAdded[Name].IsBound())
	{
		OnNotificationAdded[Name].Broadcast(Handle);
	}

	if (OnNotificationAdded[FName(TEXT("All"))].IsBound())
	{
		OnNotificationAdded[FName(TEXT("All"))].Broadcast(Handle);
	}
}

void UNotificationSystem::BroadcastOnNotificationCleared(const FName& Name, const FNotificationHandle& Handle)
{
	if (OnNotificationCleared.Contains(Name) && OnNotificationCleared[Name].IsBound())
	{
		OnNotificationAdded[Name].Broadcast(Handle);
	}

	if (OnNotificationCleared[FName(TEXT("All"))].IsBound())
	{
		OnNotificationCleared[FName(TEXT("All"))].Broadcast(Handle);
	}
}

void UNotificationSystem::AssignOnNotificationAddedImplementation(const FName& PayloadTypeName, FNotificationSystemActionNative&& Action)
{
	if (!OnNotificationAdded.Contains(PayloadTypeName))
	{
		FNotificationCallback callback;
		callback.Add(Action);
		OnNotificationAdded.Add(PayloadTypeName, callback);
	}
	else
	{
		OnNotificationAdded[PayloadTypeName].Add(Action);
	}
}

void UNotificationSystem::AssignOnNotificationClearedImplementation(const FName& PayloadTypeName, FNotificationSystemActionNative&& Action)
{
	if (!OnNotificationAdded.Contains(PayloadTypeName))
	{
		FNotificationCallback callback;
		callback.Add(Action);
		OnNotificationCleared.Add(PayloadTypeName, callback);
	}
	else
	{
		OnNotificationCleared[PayloadTypeName].Add(Action);
	}
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
