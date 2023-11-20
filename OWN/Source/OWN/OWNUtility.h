// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"

template<typename T>
T* GetDefaultComponent(const AActor* Target)
{
	if (Target == nullptr)
	{
		return nullptr;
	}

	if (Target->HasAllFlags(RF_ClassDefaultObject))
	{
		if (T* Component = Target->FindComponentByClass<T>())
		{
			return Component;
		}

		for (UActorComponent* Component : Target->BlueprintCreatedComponents)
		{
			if (T* MyComponent = Cast<T>(Component))
			{
				return MyComponent;
			}
		}

		return nullptr;
	}
	else
	{
		return GetDefaultComponent<T>(CastChecked<AActor>(Target->GetClass()->ClassDefaultObject.Get()));
	}
}

#define IsFunctionImplementedInBlueprint(FuncName) GetClass()->FindFunctionByName(#FuncName) && ensure( GetClass()->FindFunctionByName(#FuncName)->GetOuter()) &&  GetClass()->FindFunctionByName(#FuncName)->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass())