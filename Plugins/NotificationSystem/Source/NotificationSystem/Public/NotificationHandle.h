//Copyright Juan Pablo Hernandez Mosti

#pragma once

#include "CoreMinimal.h"
#include "NotificationHandle.generated.h"

/*
 * Wrapper class to be used as identifier for notifications.  
 */
USTRUCT(BlueprintType)
struct NOTIFICATIONSYSTEMMODULE_API FNotificationHandle
{
	GENERATED_BODY()

	FNotificationHandle() : Identifier(FGuid::NewGuid()) {}

	const static FNotificationHandle& InvalidHandle()
	{
		static const FNotificationHandle invalidHandle;
		return invalidHandle;
	}

	/// Checks if the current handle is still valid or not
	bool IsValid() const { return Identifier.IsValid();}

	/// Invalidates the current handle
	void Invalidate() { Identifier.Invalidate(); }

	FString ToString() const
	{
		return Identifier.ToString();
	}

	friend uint32 GetTypeHash(const FNotificationHandle& other)
	{
		return GetTypeHash(other.Identifier);
	}

	bool operator==(const FNotificationHandle& other) const
	{
		return Identifier == other.Identifier;
	}

	bool operator!=(const FNotificationHandle& other) const
	{
		return !(*this == other);
	}

private:
	
	UPROPERTY(Transient)
	FGuid Identifier;
};