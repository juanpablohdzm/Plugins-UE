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

			It("handle should be valid after creation", [this]()
			{
				FSimpleStruct simpleStruct;
				FNotificationHandle handle;
				UNotification* notification = NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				TestEqual("Handle should be valid", handle.IsValid(), true);
			});
			
			It("notification should exist after creation", [this]()
			{
				FSimpleStruct simpleStruct;
				FNotificationHandle handle;
				const UNotification* notification = NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				TestEqual("Notification should not be nullptr", notification != nullptr, true);
				TestEqual("Exists should return true", NotificationSystem->ExistsNotification(handle), true);
			});

			It("notification should return not null with valid handle", [this]()
			{
				FSimpleStruct simpleStruct;
				FNotificationHandle handle;
				NotificationSystem->CreateNotification<FSimpleStruct>(simpleStruct, handle);
				const UNotification* notification = NotificationSystem->GetNotification(handle);
				TestEqual("Notification should not be nullptr", notification != nullptr, true);
			});

		});

		AfterEach([this]()
		{
			NotificationSystem->Deinitialize();
		});
	});
}
