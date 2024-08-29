// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotificationHandle.h"
#include "Helpers/UserStructWrapper.h"
#include "UObject/Object.h"
#include "Notification.generated.h"

UENUM(BlueprintType)
enum class ENotificationActionType : uint8
{
	Accept,
	Decline,
	Dismiss
};

USTRUCT()
struct FNotificationPayload
{
	GENERATED_BODY()

	void SetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData);
	bool GetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData) const;
	bool IsOfType(const UScriptStruct* StructValue) const;
	FName GetPayloadTypeName() const;

private:

	UPROPERTY(Transient)
	FUserStructWrapper Payload;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotificationActionDelegate);
DECLARE_DYNAMIC_DELEGATE(FNotificationAction);
/**
 * 
 */
UCLASS(BlueprintType)
class NOTIFICATIONSYSTEMMODULE_API UNotification : public UObject
{
	GENERATED_BODY()
private:
	UNotification() = default;

public:

	static UNotification* Create(UScriptStruct* PayloadStruct, uint8* PayloadData);

	UFUNCTION(BlueprintCallable)
	void AddNotificationAction(ENotificationActionType ActionType, FNotificationAction Action);

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool RemoveNotificationAction(ENotificationActionType ActionType, FNotificationAction Action);

	UFUNCTION(BlueprintCallable)
	void BroadcastNotificationAction(ENotificationActionType ActionType);

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ClearNotificationAction(ENotificationActionType ActionType);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "PayloadStruct", ExpandBoolAsExecs = "ReturnValue"))
	bool GetPayload(UScriptStruct*& PayloadStruct);
	DECLARE_FUNCTION(execGetPayload);

	template <typename UStruct> bool GetPayload(UStruct& Payload) const;

	UFUNCTION(BlueprintCallable)
	bool IsPayloadOfType(UScriptStruct* PayloadStruct) const;
	template <typename UStruct> bool IsPayloadOfType() const;

	UFUNCTION(BlueprintCallable)
	FNotificationHandle GetHandle() const { return Handle;}

	FName GetPayloadTypeName() const;
private:

	bool GetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData) const;

	UPROPERTY(Transient)
	TMap<ENotificationActionType, FNotificationActionDelegate> NotificationActions;

	UPROPERTY(Transient)
	FNotificationPayload NotificationPayload;
	
	UPROPERTY(Transient)
	FNotificationHandle Handle;
};

template <typename UStruct>
bool UNotification::GetPayload(UStruct& Payload) const
{
	return GetPayload(UStruct::StaticStruct(), reinterpret_cast<uint8*>(&Payload));
}

template <typename UStruct>
bool UNotification::IsPayloadOfType() const
{
	return IsPayloadOfType(UStruct::StaticStruct());
}


