// This class handles the updating of the player's tiredness, makes them yawn and makes them go uncon randomly if they're too sleepy
class ZenTirednessModifier: ModifierBase
{
	private bool m_SentUnconMessageLastUpdate = false;
	private float m_TirednessThreshold = -1;
	private float m_UnconThreshold = -1;
	private int m_UpdatesSinceLastYawn = 2; // I set this to initialize at 2 because otherwise using the sleep syringe on a fully awake player won't play the yawn sound

	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= ZenSleep_Modifiers.ZEN_TIREDNESS;
		m_TickIntervalInactive 	= 6;
		m_TickIntervalActive 	= 6;
		m_TirednessThreshold    = PlayerBase.MAX_TIREDNESS * ((float)(100.0 - GetZenSleepConfig().YawnPercentStart) / 100.0); // Converts yawn % start into a tiredness value/1000 (max tiredness)
		m_UnconThreshold        = PlayerBase.MAX_TIREDNESS * ((float)(100.0 - GetZenSleepConfig().UnconPercentStart) / 100.0); // Converts uncon % start into a tiredness value/1000 (max tiredness)
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		float tiredness = player.GetSingleAgentCount(ZenSleep_Agents.TIREDNESS);

		// If we are fully rested 100%, add some tiredness and stop here
		if (tiredness == 0)
		{
			player.InsertAgent(ZenSleep_Agents.TIREDNESS, 1);
			return;
		}

		// If the player is asleep or unconscious, stop here
		if (player.IsPlayerSleeping() || player.IsUnconscious())
		{
			return;
		}

		// Check if player is beyond their tiredness threshold and if they're not tired enough for yawning or uncon, stop here to prevent unncessary calculations.
		if (tiredness < m_TirednessThreshold)
		{
			if (GetZenSleepConfig().DebugOn == 1)
			{
				player.ZS_SendMessage("Yawn/Uncon: N/A, not tired enough");
			}

			return;
		}

		// Prepare tired "depth" values (how deep are we into tiredness beyond our yawn percent start threshold? This is used to increase the chance of yawns etc)
		float tiredDepth = (player.MAX_TIREDNESS * ((GetZenSleepConfig().YawnPercentStart) / 100.0) - (player.MAX_TIREDNESS - tiredness); // How far into sleepiness are we past our yawn percentage?
		float tiredDepthStep = tiredDepth / 50.0;
		float tiredDepthPenalty = 0;
		if (tiredDepth > 0)
		{
			tiredDepthPenalty = tiredDepthStep / 100; // With the default settings this means: Convert increments of > 50 steps past 800 tiredness into +1% chance of yawn and +0.5% chance of uncon
		}

		// Calculate percentage chances of yawning & falling uncon
		float yawnChance = GetZenSleepConfig().YawnPercentChance + tiredDepthPenalty;
		float unconChance = GetZenSleepConfig().UnconPercentChance + (tiredDepthPenalty / 2.0);
		float rand = Math.RandomFloat01();
		bool fallUnconscious = tiredness >= m_UnconThreshold && rand <= unconChance && (!GetZenSleepConfig().CanUnconInVehicle || !player.IsInVehicle());

		// Player is max tired, set them uncon
		if (tiredness >= PluginTransmissionAgents.GetAgentMaxCount(ZenSleep_Agents.TIREDNESS))
		{
			fallUnconscious = true;
			unconChance = 100.0;

			if (GetZenSleepConfig().DebugOn)
			{
				player.ZS_SendMessage("Fall uncon forced due to tiredness: " + tiredness + "/" + PluginTransmissionAgents.GetAgentMaxCount(ZenSleep_Agents.TIREDNESS));
			}

			// Sometimes the client won't tell the server it's ready to fall uncon after yawning, this overrides that if necessary (TODO: Find a better way to fix this strange issue!)
			if (fallUnconscious && m_SentUnconMessageLastUpdate && !player.IsUnconscious())
			{
				player.Zen_SetPlayerUncon();
				player.m_IsUnconsciousFromTiredness = true;
				return;
			}
		}

		// If random trigger, or player is falling uncon, or it's been > 100 updates since our last yawn, play a yawn. 
		// But don't play the sound if player.m_FallUnconsciousFromTiredness is already true or we yawned last update (prevents playing lots of yawns in a row).
		if ((rand <= GetZenSleepConfig().YawnPercentChance || fallUnconscious || m_UpdatesSinceLastYawn >= 100) && !player.m_FallUnconsciousFromTiredness && m_UpdatesSinceLastYawn > 1)
		{
			player.MakeYawnSound();
			m_UpdatesSinceLastYawn = 0;
		}
		else
		{
			m_UpdatesSinceLastYawn++;
		}

		if (fallUnconscious)
		{
			if (GetZenSleepConfig().DebugOn)
			{
				player.ZS_SendMessage("Fall uncon!");
			}

			player.SetUnconsciousFromTiredness();
			m_SentUnconMessageLastUpdate = true;
		}
		else
		{
			m_SentUnconMessageLastUpdate = false;
		}

		// Debug message
		if (GetZenSleepConfig().DebugOn == 1)
		{
			string debugStr = "";
			if (tiredness >= m_TirednessThreshold)
			{
				debugStr = " | tiredDepthPenalty=" + tiredDepthPenalty + " | yawnChance=" + yawnChance + " | unconChance=" + unconChance + " | rand=" + rand + " yawn=" + (rand <= yawnChance) + " uncon=" + fallUnconscious + " FallUncon=" + player.m_FallUnconsciousFromTiredness + " LastYawn=" + m_UpdatesSinceLastYawn + "/100 updates";
			}

			player.ZS_SendMessage("Tiredness=" + tiredness + debugStr);
		}
	}
}