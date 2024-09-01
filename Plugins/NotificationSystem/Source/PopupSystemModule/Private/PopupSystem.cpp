// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogPopupSystem, All, All);

UPopupSystem* UPopupSystem::Get(const UObject* WorldContext)
{
	if (const UWorld* world = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
	{
		return UGameInstance::GetSubsystem<UPopupSystem>(world->GetGameInstance());
	}

	UE_LOG(LogPopupSystem, Warning, TEXT("Can not get subsystem because UWorld pointer is null"));
	return nullptr;
}
