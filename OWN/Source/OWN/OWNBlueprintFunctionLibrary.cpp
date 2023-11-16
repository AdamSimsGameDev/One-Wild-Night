// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors


#include "OWNBlueprintFunctionLibrary.h"
#include "TargetingSystem/TargetingSubsystem.h"
#include "Tasks/TargetingTask.h"
#include "Types/TargetingSystemTypes.h"

UTargetingSubsystem* UOWNBlueprintFunctionLibrary::GetTargetingSubsystemFromRequest(const UTargetingTask* Task, const FTargetingRequestHandle& TargetingHandle)
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			return UTargetingSubsystem::Get(SourceContext->SourceActor->GetWorld());
		}

		if (SourceContext->InstigatorActor)
		{
			return UTargetingSubsystem::Get(SourceContext->InstigatorActor->GetWorld());
		}
	}
	return nullptr;
}