class PPERequester_SleepEffect extends PPERequester_GameplayBase
{
	protected ref array<float> m_OverlayColor;

	void ZS_SetEffectValues(float blur, float vignette, float color_overlay_factor)
	{
		if (!IsRequesterRunning())
			Start();

		m_OverlayColor = {0.0, 0.0, 0.0, 1.0};
		if (color_overlay_factor > 0.0)
			m_OverlayColor = {0.1, 0.1, 0.1, 1.0};

		SetTargetValueFloat(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, true, blur, PPEGaussFilter.L_0_SHOCK, PPOperators.ADD_RELATIVE);
		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTE, false, vignette, PPEGlow.L_25_SHOCK, PPOperators.ADD);
		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_OVERLAYFACTOR, true, color_overlay_factor, PPEGlow.L_20_SHOCK, PPOperators.HIGHEST);
		SetTargetValueColor(PostProcessEffectType.Glow, PPEGlow.PARAM_OVERLAYCOLOR, m_OverlayColor, PPEGlow.L_21_SHOCK, PPOperators.SET);
	}
}