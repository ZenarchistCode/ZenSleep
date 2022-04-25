enum ZenSleep_Modifiers
{
	ZEN_TIREDNESS = 1000
}

modded class ModifiersManager
{
	override void Init()
	{
		super.Init();
		AddModifier(new TirednessModifier);
	}
}
