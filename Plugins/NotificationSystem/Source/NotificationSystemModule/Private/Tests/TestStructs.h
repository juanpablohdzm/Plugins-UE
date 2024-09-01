#pragma once

#include "CoreMinimal.h"
#include "TestStructs.generated.h"

USTRUCT()
struct FSimpleStruct
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 IntProp;

	UPROPERTY()
	float FloatProp;

	UPROPERTY()
	FString StringProp;
};

USTRUCT()
struct FCompoundStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FName NameProp;

	UPROPERTY()
	FSimpleStruct SimpleStructProp;
};
