// Copyright Juan Pablo Hernandez Mosti
#pragma once

#include "CoreMinimal.h"
#include "UObject/StructOnScope.h"
#include "UserStructWrapper.generated.h"

/**
 * Helper class to serialize and deserialize a USTRUCT
 */
USTRUCT()
struct NOTIFICATIONSYSTEMMODULE_API FUserStructWrapper
{
	GENERATED_BODY()
public:
	void SetStructClass(UScriptStruct* StructClass);
	const UScriptStruct* GetStructClass() const  { return UserStructClass; }
	
	bool CopyFromStruct(FStructOnScope& StructValue);
	bool CopyToStruct(FStructOnScope& StructValue) const;

private:
	void ResetToDefaults();

	UPROPERTY()
	UScriptStruct* UserStructClass = nullptr;

	UPROPERTY()
	TArray<uint8> Data;
};
