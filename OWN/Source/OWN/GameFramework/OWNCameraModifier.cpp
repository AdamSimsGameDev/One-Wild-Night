// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "OWNCameraModifier.h"

bool UOWNCameraModifier::ModifySpringArm(float DeltaTime, FSpringArmProperties& Props)
{	
	// let BP do what it wants
	BlueprintModifySpringArm(DeltaTime, Props.TargetArmLength, Props.SocketOffset, Props.TargetOffset, Props.RelativeLocation, Props.RelativeRotation, 
		Props.TargetArmLength, Props.SocketOffset, Props.TargetOffset, Props.RelativeLocation, Props.RelativeRotation);

	// return false so subsequent modifiers can modify
	return false;
}
