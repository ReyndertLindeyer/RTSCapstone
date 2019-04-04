// Fill out your copyright notice in the Description page of Project Settings.

#include "I_ScriptedEvent.h"

SE_PARAMS* SE_PARAMS::BuildParams(II_Player* targetPlayer_)
{
	SE_PARAMS* params = new SE_PARAMS();
	params->targetPlayer = targetPlayer_;

	return params;
}

void II_ScriptedEvent::TriggerEvent(II_Player* params)
{
	// Virtual Function
}

