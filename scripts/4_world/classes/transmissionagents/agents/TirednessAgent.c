class TirednessAgent: AgentBase
{
	static int MAX_COUNT = 1000;

	override void Init()
	{
		m_Type 					= ZenSleep_Agents.TIREDNESS;
		m_Invasibility 			= 0.08;
		m_TransferabilityIn		= 0;
		m_TransferabilityOut	= 0;
		m_AntibioticsResistance = 1;
		m_MaxCount 				= MAX_COUNT;
		m_Potency				= EStatLevels.GREAT;
		m_DieOffSpeed			= 1;
	}

	override float GetInvasibilityEx(PlayerBase player)
	{
		if (!player || player.IsPlayerSleeping() || !player.IsAlive())
			return 0;

		if (GetZenSleepConfig().MaxWakeTime > 0)
		{
			m_Invasibility = (float)m_MaxCount / (GetZenSleepConfig().MaxWakeTime * 60);

			if (!GetZenSleepConfig().EnableMovementPenalties)
				return m_Invasibility;
		}

		float modifier = 1.0;

		if (!player.m_MovementState)
			return m_Invasibility;

		// No penalty for walking or crouch-walking (or any other behaviour - todo, add more behaviours? Or is this enough to cover most bases?)

		if (player.IsSwimming()) // Swimming
		{
			if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT) // Swimming fast
			{
				modifier = GetZenSleepConfig().SwimFastPenalty;
			}
			else 
			if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_RUN) // Swimming slow
			{
				modifier = GetZenSleepConfig().SwimSlowPenalty;
			}
		} else
		if (player.IsClimbingLadder()) // Climbing ladder
		{
			if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_RUN) // Climbing fast
			{
				modifier = GetZenSleepConfig().ClimbFastPenalty;
			}
			else
			if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_WALK) // Climbing slow
			{
				modifier = GetZenSleepConfig().ClimbSlowPenalty;
			}
		} else
		if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT) // Sprinting
		{
			if (player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT) // While erect IF YOU KNOW WHAT I MEAN WINK WINK
			{
				modifier = GetZenSleepConfig().SprintUprightPenalty;
			}
			else if (player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH) // While crouched
			{
				modifier = GetZenSleepConfig().SprintCrouchedPenalty;
			}
		}
		else
		if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_RUN) // Jogging
		{
			modifier = GetZenSleepConfig().JoggingPenalty;
		}
		else
		if (player.m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_WALK) // Walking
		{
			// With heavy item in hands
			ItemBase item = ItemBase.Cast(player.GetItemInHands());
			if (item)
			{
				if (item.IsHeavyBehaviour())
				{
					modifier = GetZenSleepConfig().WalkingHeavyItemPenalty;
				}
			}
		}

		//player.ZS_SendMessage("Current Movement State=" + player.m_MovementState.m_iMovement + " / StanceIndex=" + player.m_MovementState.m_iStanceIdx + " - invasibility=" + (m_Invasibility * modifier) + " (Swimming=" + player.IsSwimming() + ", Climbing=" + player.IsClimbingLadder() + ")");

		return m_Invasibility * modifier;
	}
}

/*
MovementIndex:
	Idle = 0
	Walk = 1
	Jog = 2
	Sprint = 3

StanceIndex:
	Erect = 0
	Crouched = 1
	Prone = 2
*/