#pragma once

class FMemoryUObjectWriter : public FMemoryWriter
{
public:
	FMemoryUObjectWriter(TArray<uint8>& InBytes) : FMemoryWriter(InBytes) {}

	virtual FArchive& operator<<(UObject*& Res) override;
	virtual FArchive& operator<<(FLazyObjectPtr& Value) override;
	virtual FArchive& operator<<(FObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPath& Value) override;
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override;
};

class FMemoryUObjectReader : public FMemoryReader
{
public:
	FMemoryUObjectReader(const TArray<uint8>& InBytes) : FMemoryReader(InBytes) {}

	virtual FArchive& operator<<(FObjectPtr& Value) override;
	virtual FArchive& operator<<(UObject*& Res) override;
	virtual FArchive& operator<<(FLazyObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPtr& Value) override;
	virtual FArchive& operator<<(FSoftObjectPath& Value) override;
	virtual FArchive& operator<<(FWeakObjectPtr& Value) override;
};