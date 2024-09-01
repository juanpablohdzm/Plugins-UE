// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotificationHandle.h"
#include "Helpers/UserStructWrapper.h"
#include "UObject/Object.h"
#include "Notification.generated.h"

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

/**
 * Wrapper class to hold the payload struct and the identifier. 
 */
UCLASS(BlueprintType)
class NOTIFICATIONSYSTEMMODULE_API UNotification final : public UObject
{
	GENERATED_BODY()
private:
	UNotification() = default;

public:

	static UNotification* Create(UScriptStruct* PayloadStruct, uint8* PayloadData);

	/// Get Payload
	/// @param PayloadStruct Out information of the notification
	/// @return if it succeeded or not
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "PayloadStruct", ExpandBoolAsExecs = "ReturnValue"))
	bool GetPayload(UScriptStruct*& PayloadStruct);
	DECLARE_FUNCTION(execGetPayload);

	/// Native version of get payload (meant for C++)
	/// @tparam UStruct Type of the struct to get
	/// @param Payload Out information of the notification
	/// @return If it succeeded or not
	template <typename UStruct> bool GetPayload(UStruct& Payload) const;

	///
	/// Check if the payload of the notification is of the passed type
	/// @param PayloadStruct Struct type to be compared
	UFUNCTION(BlueprintCallable)
	bool IsPayloadOfType(UScriptStruct* PayloadStruct) const;
	
	/// Native version of IsPayloadOfType (meant for C++)
	/// @tparam UStruct Type to be compared 
	template <typename UStruct> bool IsPayloadOfType() const;

	/// GetHandle
	/// @return The handle identifier of the notification
	UFUNCTION(BlueprintCallable)
	FNotificationHandle GetHandle() const { return Handle;}

	/// GetPayloadTypeName
	/// @return The name of the payload
	UFUNCTION(BlueprintCallable)
	FName GetPayloadTypeName() const;
private:

	bool GetPayload(UScriptStruct* PayloadStruct, uint8* PayloadData) const;

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


