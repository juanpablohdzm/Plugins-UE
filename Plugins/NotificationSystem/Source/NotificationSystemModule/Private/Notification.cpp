#include "Notification.h"

void FNotificationPayload::SetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData)
{
	Payload.SetStructClass(PayloadStruct);
	FStructOnScope structValue{PayloadStruct, PayloadData};
	Payload.CopyFromStruct(structValue);
}

bool FNotificationPayload::GetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData) const
{
	FStructOnScope structValue{PayloadStruct, PayloadData};
	return Payload.CopyToStruct(structValue);
}

bool FNotificationPayload::IsOfType(const UScriptStruct* StructValue) const
{
	if (const UScriptStruct* structClass = Payload.GetStructClass())
	{
		return structClass == StructValue;
	}
	return false;
}

FName FNotificationPayload::GetPayloadTypeName() const
{
	const UScriptStruct* scriptStruct = Payload.GetStructClass();
	return scriptStruct ? scriptStruct->GetFName() : NAME_None;
}

UNotification* UNotification::Create(UScriptStruct* PayloadStruct, uint8* PayloadData)
{
	UNotification* notification = NewObject<UNotification>();
	notification->NotificationPayload.SetPayload(PayloadStruct, PayloadData);
	return notification;
}

void UNotification::AddNotificationAction(ENotificationActionType ActionType, FNotificationAction Action)
{
	if (NotificationActions.Contains(ActionType))
	{
		NotificationActions[ActionType].Add(Action);
		return;
	}

	FNotificationActionDelegate delegate;
	delegate.Add(Action);
	NotificationActions.Add(ActionType, delegate);
}

bool UNotification::RemoveNotificationAction(ENotificationActionType ActionType, FNotificationAction Action)
{
	if (NotificationActions.Contains(ActionType))
	{
		NotificationActions[ActionType].Remove(Action);
		return true;
	}
	return false;
}

void UNotification::BroadcastNotificationAction(ENotificationActionType ActionType)
{
	NotificationActions[ActionType].Broadcast();
}

bool UNotification::ClearNotificationAction(ENotificationActionType ActionType)
{
	if (NotificationActions.Contains(ActionType))
	{
		NotificationActions[ActionType].Clear();
		return true;
	}
	return false;
}

DEFINE_FUNCTION(UNotification::execGetPayload)
{
	Stack.Step(Stack.Object, nullptr);
	FStructProperty* structProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	uint8* structMemoryStart = Stack.MostRecentPropertyAddress;
	P_FINISH;

	P_NATIVE_BEGIN
	*static_cast<bool*>(RESULT_PARAM) = P_THIS->GetPayload(structProperty->Struct, structMemoryStart);
	P_NATIVE_END
}

bool UNotification::IsPayloadOfType(UScriptStruct* PayloadStruct) const
{
	return NotificationPayload.IsOfType(PayloadStruct);
}

bool UNotification::GetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData) const
{
	return NotificationPayload.GetPayload(PayloadStruct, PayloadData);
}

FName UNotification::GetPayloadTypeName() const
{
	return NotificationPayload.GetPayloadTypeName();
}
