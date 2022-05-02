modded class MissionGameplay extends MissionBase
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		// Get player
		autoptr PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		// Get input controller
		autoptr Input zenInput = Input.Cast(GetGame().GetInput());

		if (!player || !zenInput)
			return;

		if (zenInput.LocalPress("ZenSleep_RefreshConfig")) // Sends request to server to reload json config if player is admin
		{
			player.RequestServerConfigReload();
		} else
		if (zenInput.LocalPress("ZenSleep_SleepKey")) // Triggers the lie-down emote if the player can lie down ingame
		{
			bool canSleep = player.GetEmoteManager().CanPlayEmote(EmoteConstants.ID_EMOTE_LYINGDOWN);

			if (canSleep)
			{
				if (player.GetEmoteManager())
				{
					player.GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_LYINGDOWN);
				}
			}
		}
	}
}