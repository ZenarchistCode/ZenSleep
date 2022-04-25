modded class EmoteManager
{
	override bool CanPlayEmote( int id )
	{
		bool canplay = super.CanPlayEmote( id );

		if (id == CALLBACK_CMD_END || id == CALLBACK_CMD_GESTURE_INTERRUPT)
		{
			m_Player.CancelSleep();
		}

		return canplay;
	}

	override bool PlayEmote(int id)
	{
		bool playEmote = super.PlayEmote(id);

		if (playEmote && id == EmoteConstants.ID_EMOTE_LYINGDOWN)
		{
			m_Player.ResetSleep(); // Reset all sleep variables for a fresh sleep
			m_Player.CheckIfPlayerIsInside(); // Send a roof raycast to see if the player is inside (for applying sleep accelerators)
			if (m_Player.m_AllowInventoryWhileSleep) // Allow inventory access if allowed by server config
			{
				m_Player.SetInventorySoftLock(false);
			}
		}

		return playEmote;
	}
}
