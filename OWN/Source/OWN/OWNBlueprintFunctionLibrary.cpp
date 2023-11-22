// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors


#include "OWNBlueprintFunctionLibrary.h"
#include "TargetingSystem/TargetingSubsystem.h"
#include "Tasks/TargetingTask.h"
#include "Types/TargetingSystemTypes.h"
#include "OWN/GameFramework/WeaponDefinition.h"

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

void UOWNBlueprintFunctionLibrary::AddStatTagStack(UPARAM(Ref)FGameplayTagStackContainer& Stack, FGameplayTag Tag, int32 Count)
{
	Stack.AddStack(Tag, Count);
}

void UOWNBlueprintFunctionLibrary::RemoveStatTagStack(UPARAM(Ref)FGameplayTagStackContainer& Stack, FGameplayTag Tag, int32 Count)
{
	Stack.RemoveStack(Tag, Count);
}

void UOWNBlueprintFunctionLibrary::SetStatTagStackCount(UPARAM(Ref)FGameplayTagStackContainer& Stack, FGameplayTag Tag, int32 Count)
{
	Stack.SetStackCount(Tag, Count);
}

bool UOWNBlueprintFunctionLibrary::HasStatTagStack(const FGameplayTagStackContainer& Stack, FGameplayTag Tag)
{
	return Stack.GetStackCount(Tag) != 0;
}

int32 UOWNBlueprintFunctionLibrary::GetStatTagStackCount(const FGameplayTagStackContainer& Stack, FGameplayTag Tag)
{
	return Stack.GetStackCount(Tag);
}

int32 UOWNBlueprintFunctionLibrary::GetWeaponData_Int(const UWeaponDefinition* Definition, FGameplayTag Key)
{
	return Definition ? Definition->GetStat<int32>(Key) : 0;
}

bool UOWNBlueprintFunctionLibrary::GetWeaponData_Bool(const UWeaponDefinition* Definition, FGameplayTag Key)
{
	return Definition ? Definition->GetStat<bool>(Key) : false;
}

float UOWNBlueprintFunctionLibrary::GetWeaponData_Float(const UWeaponDefinition* Definition, FGameplayTag Key)
{
	return Definition ? Definition->GetStat<float>(Key) : 0.0f;
}

FGameplayTag UOWNBlueprintFunctionLibrary::GetWeaponData_Tag(const UWeaponDefinition* Definition, FGameplayTag Key)
{
	return Definition ? Definition->GetStat<FGameplayTag>(Key) : FGameplayTag::EmptyTag;
}
