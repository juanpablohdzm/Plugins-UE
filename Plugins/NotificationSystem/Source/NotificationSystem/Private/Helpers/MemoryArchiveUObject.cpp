
#include "MemoryArchiveUObject.h"

#include "UObject/CoreRedirects.h"

bool FixupRedirectors(FString& Path)
{
	const FCoreRedirectObjectName oldName = FCoreRedirectObjectName(Path);

	const bool isNative = Path.StartsWith(TEXT("/Script/"));
	const FCoreRedirectObjectName newName = FCoreRedirects::GetRedirectedName(isNative ? ECoreRedirectFlags::Type_AllMask : ECoreRedirectFlags::Type_Object, oldName);

	if (oldName != newName)
	{
		Path = newName.ToString();
		return true;
	}
	return false;
}

bool FixupRedirectors(FSoftObjectPath& SoftObjectPath)
{
	FString path  = SoftObjectPath.ToString();
	if (FixupRedirectors(path))
	{
		SoftObjectPath.SetPath(path);
		return true;
	}
	return false;
}

bool FixupRedirectors(FSoftObjectPtr& SoftObjectPtr)
{
	FSoftObjectPath softObjectPath = SoftObjectPtr.ToSoftObjectPath();
	if (FixupRedirectors(softObjectPath))
	{
		SoftObjectPtr = FSoftObjectPtr(softObjectPath);
		return true;
	}
	return false;
}

FArchive& FMemoryUObjectWriter::operator<<(UObject*& Res)
{
	FString path = Res->GetPathName();
	(*this) << path;
	return *this;
}

FArchive& FMemoryUObjectWriter::operator<<(FLazyObjectPtr& Value)
{
	return FArchiveUObject::SerializeLazyObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectWriter::operator<<(FObjectPtr& Value)
{
	return FArchiveUObject::SerializeObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectWriter::operator<<(FSoftObjectPtr& Value)
{
	FixupRedirectors(Value);
	return FArchiveUObject::SerializeSoftObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectWriter::operator<<(FSoftObjectPath& Value)
{
	FixupRedirectors(Value);
	return FArchiveUObject::SerializeSoftObjectPath(*this, Value);
}

FArchive& FMemoryUObjectWriter::operator<<(FWeakObjectPtr& Value)
{
	return FArchiveUObject::SerializeWeakObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectReader::operator<<(FObjectPtr& Value)
{
	return FArchiveUObject::SerializeObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectReader::operator<<(UObject*& Res)
{
	FString path;
	(*this) << path;
	Res = StaticLoadObject(UObject::StaticClass(), nullptr, *path);

	if (!Res && !path.IsEmpty())
	{
		if (FixupRedirectors(path))
		{
			Res = StaticLoadObject(UObject::StaticClass(), nullptr, *path);
		}
	}
	return *this;
}

FArchive& FMemoryUObjectReader::operator<<(FLazyObjectPtr& Value)
{
	return FArchiveUObject::SerializeLazyObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectReader::operator<<(FSoftObjectPtr& Value)
{
	return FArchiveUObject::SerializeSoftObjectPtr(*this, Value);
}

FArchive& FMemoryUObjectReader::operator<<(FSoftObjectPath& Value)
{
	return FArchiveUObject::SerializeSoftObjectPath(*this, Value);
}

FArchive& FMemoryUObjectReader::operator<<(FWeakObjectPtr& Value)
{
	return FArchiveUObject::SerializeWeakObjectPtr(*this, Value);
}