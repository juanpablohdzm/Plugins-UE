#include "Helpers/UserStructWrapper.h"

#include "MemoryArchiveUObject.h"

void FUserStructWrapper::SetStructClass(UScriptStruct* StructClass)
{
	ResetToDefaults();
	UserStructClass = StructClass;
}

bool FUserStructWrapper::CopyToStruct(FStructOnScope& StructValue) const
{
	if (StructValue.GetStruct() != UserStructClass)
	{
		return false;
	}

	if (Data.Num() > 0)
	{
		FMemoryUObjectReader reader { Data };
		UserStructClass->SerializeItem(reader, StructValue.GetStructMemory(), nullptr);
	}
	
	return true;
}

bool FUserStructWrapper::CopyFromStruct(FStructOnScope& StructValue)
{
	if (StructValue.GetStruct() != UserStructClass)
	{
		return false;
	}

	FMemoryUObjectWriter writer { Data };
	UserStructClass->SerializeItem(writer, StructValue.GetStructMemory(), nullptr);

	return true;
}

void FUserStructWrapper::ResetToDefaults()
{
	Data.Reset();
}
