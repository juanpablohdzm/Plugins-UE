//Copyright Juan Pablo Hernandez Mosti

#pragma once

#include "CoreMinimal.h"
#include "NotificationHandle.generated.h"

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

	bool IsValid() const { return Identifier.IsValid();}

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