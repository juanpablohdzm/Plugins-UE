#include "NotificationSystem.h"
#include "TestStructs.h"
#include "Misc/AutomationTest.h"

BEGIN_DEFINE_SPEC(AutomationSpec, "Gameplay.NotificationSystem", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
UNotificationSystem* NotificationSystem;
END_DEFINE_SPEC(AutomationSpec)

void AutomationSpec::Define()
{
	Describe("Notification System native", [this]()
	{
		BeforeEach([this]()
		{
			UGameInstance* gameInstance  = NewObject<UGameInstance>();
			gameInstance->Init();
			NotificationSystem = gameInstance->GetSubsystem<UNotificationSystem>();
		});

		Describe("Notification creation", [this]()
		{
			It("should created a simple notification", [this]()
			{
				FSimpleStruct simpleStruct;
				FNotificationHandle handle;
				UNotification* notification = NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				TestEqual("Notification should not be nullptr", notification != nullptr, true);
			});

			It("notification should exist after creation", [this]()
			{
				FSimpleStruct simpleStruct;
				FNotificationHandle handle;
				const UNotification* notification = NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				TestEqual("Notification should not be nullptr", notification != nullptr, true);
				TestEqual("Exists should return true", NotificationSystem->ExistsNotification(handle), true);
			});
			
			Describe("Handle", [this]()
			{
				It("handle should be valid after creation", [this]()
				{
					FSimpleStruct simpleStruct;
					FNotificationHandle handle;
					UNotification* notification = NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
					TestEqual("Handle should be valid", handle.IsValid(), true);
				});
				
				It("notification should return not null with valid handle", [this]()
				{
					FSimpleStruct simpleStruct;
					FNotificationHandle handle;
					NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
					const UNotification* notification = NotificationSystem->GetNotification(handle);
					TestEqual("Notification should not be nullptr", notification != nullptr, true);
				});

				It("notification should return null with invalid handle", [this]()
				{
					const FNotificationHandle& handle = FNotificationHandle::InvalidHandle();
					const UNotification* notification = NotificationSystem->GetNotification(handle);
					TestEqual("Notification should be nullptr", notification == nullptr, true);
				});
			});
		});

		Describe("notification payload", [this]()
		{
			It("notification payload should be the same type after creation", [this]()
			{
				FSimpleStruct simpleStruct;
				simpleStruct.IntProp = 10;
				simpleStruct.StringProp = "My struct";
				FNotificationHandle handle;
				NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				const UNotification* notification = NotificationSystem->GetNotification(handle);
				TestEqual("notification payload should be simple struct", notification->IsPayloadOfType<FSimpleStruct>(), true);
			});

			It("notification payload should return false if pass wrong payload", [this]()
			{
				FSimpleStruct simpleStruct;
				FNotificationHandle handle;
				NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				const UNotification* notification = NotificationSystem->GetNotification(handle);
				TestEqual("notification payload should not be compound struct", notification->IsPayloadOfType<FCompoundStruct>(), false);
			});

			It("notification payload should not change if simple after creation", [this]()
			{
				FSimpleStruct simpleStruct;
				simpleStruct.IntProp = 10;
				simpleStruct.StringProp = "My struct";
				FNotificationHandle handle;
				NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				const UNotification* notification = NotificationSystem->GetNotification(handle);
				FSimpleStruct getStruct;
				notification->GetPayload<FSimpleStruct>(getStruct);
				TestEqual("payload values should be the same", getStruct.StringProp == simpleStruct.StringProp, true);
			});

			It("notification payload should have values even in compound structs", [this]()
			{
				FSimpleStruct simpleStruct;
				simpleStruct.IntProp = 10;
				simpleStruct.StringProp = "My struct";
				FCompoundStruct compoundStruct;
				compoundStruct.SimpleStructProp = simpleStruct;
				compoundStruct.NameProp = "Compound struct";
				FNotificationHandle handle;
				NotificationSystem->CreateNotification<FCompoundStruct>(compoundStruct, handle);
				const UNotification* notification = NotificationSystem->GetNotification(handle);
				FCompoundStruct gotStruct;
				notification->GetPayload<FCompoundStruct>(gotStruct);
				TestEqual("payload values should be the same", gotStruct.NameProp == compoundStruct.NameProp && gotStruct.SimpleStructProp.StringProp == compoundStruct.SimpleStructProp.StringProp, true);
			});
		});

		AfterEach([this]()
		{
			NotificationSystem->Deinitialize();
		});
	});
}
