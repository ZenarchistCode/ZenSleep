modded class EmoteManager
{
	// Triggered if our sleep emote is interrupted or ended
	override bool CanPlayEmote( int id )
	{
		bool canplay = super.CanPlayEmote( id );

		if (id == CALLBACK_CMD_END || id == CALLBACK_CMD_GESTURE_INTERRUPT)
		{
			m_Player.CancelSleep();
		}

		return canplay;
	}

	// Triggered on both client & server when player uses an emote
	override bool PlayEmote(int id)
	{
		bool playEmote = super.PlayEmote(id);

		// If the player has used the lie down emote, prepare for sleep
		if (playEmote && id == EmoteConstants.ID_EMOTE_LYINGDOWN)
		{
			m_Player.ResetSleep(); // Reset all sleep variables for a fresh sleep
			m_Player.CheckIfPlayerIsInside(); // Send a roof raycast to see if the player is inside (for applying sleep accelerators)
			if (m_Player.m_AllowInventoryWhileSleep) // Allow inventory access if permitted by server config
			{
				m_Player.SetInventorySoftLock(false);
			}
		}

		return playEmote;
	}
}
