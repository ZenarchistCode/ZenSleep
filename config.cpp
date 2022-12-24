/* 
   Huge credit to Lucian who wrote the foundation for this mod (https://steamcommunity.com/sharedfiles/filedetails/?id=1735691552)
   Also credit to the modder who updated Lucian's mod for the latest DayZ builds (https://steamcommunity.com/sharedfiles/filedetails/?id=2265389602)
   Massive thanks to Lucian for allowing his mod to be open-source to all:

   Lucian's Medical Attention Description (Updated 4 Mar, 2020):
   You ARE allowed to repack, reupload, modify or reproduce any portions of this mod.
   It would be appreciated if you include link to this original mod page.

   In the same spirit, so is mine. Use this code however you like with no credit needed for me - but please credit Lucian for the foundation :)



   24/12/22: I have become much more experienced at modding since creating this re-make while learning the ropes. The code here is 
			 "acceptable", but not optimal. By that, I mean it works, but it's like a functioning alcoholic. I plan to remake this mod
			 from the ground-up when I have time - but for now, it is what it is, and if you're like me and love immersion - it works!

			 Stay tuned for "ZenSleep v2.0". I've already spent so much time on this my girlfriend wants to leave me, but I'm no quitter.
*/

class CfgPatches
{
	class ZenSleep
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Scripts",
			"DZ_Sounds_Effects"
		};
	};
};
class CfgMods
{
	class ZenSleep
	{
		dir="ZenSleep";
		picture=""; 
		action="";
		hideName=1;
		hidePicture=1;
		name="ZenSleep";
		credits="Lucian"; 
		author="Zenarchist";
		authorID="0";  
		version="1.1";
		extra=0;
		type="mod";
		inputs="ZenSleep/data/inputs.xml";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"ZenSleep/scripts/common",
					"ZenSleep/scripts/3_game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"ZenSleep/scripts/common",
					"ZenSleep/scripts/4_world"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"ZenSleep/scripts/common",
					"ZenSleep/scripts/5_mission"
				};
			};
		};
	};
};
class CfgVehicles // Custom syringe definitions
{
	class Inventory_Base;
	class ZenSleep_Syringe : Inventory_Base
	{
		scope = 2;
		displayName = "Anaesthetic Syringe";
		descriptionShort = "A syringe filled with an anaesthetic drug. I could use it to put myself to sleep. Or perhaps another survivor...";
		model = "\dz\gear\medical\syringe_Full.p3d";
		debug_ItemCategory = 7;
		hiddenSelections[] =
		{
			"camo"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\medical\data\SyringeFluid_Grey_CA.paa"
		};
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 60;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\Syringe_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Syringe_out
				{
					soundSet = "Syringe_out_SoundSet";
					id = 201;
				};
				class Syringe_spear
				{
					soundSet = "Syringe_spear_SoundSet";
					id = 202;
				};
				class Syringe_splash
				{
					soundSet = "Syringe_splash_SoundSet";
					id = 203;
				};
			};
		};
	};
	class ZenSleep_Empty_Syringe : Inventory_Base
	{
		scope = 2;
		displayName = "Empty Syringe";
		descriptionShort = "An empty anaesthetic syringe.";
		model = "\dz\gear\medical\syringe_empty.p3d";
		debug_ItemCategory = 7;
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 60;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\Syringe_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Syringe_out
				{
					soundSet = "Syringe_out_SoundSet";
					id = 201;
				};
				class Syringe_spear
				{
					soundSet = "Syringe_spear_SoundSet";
					id = 202;
				};
				class Syringe_splash
				{
					soundSet = "Syringe_splash_SoundSet";
					id = 203;
				};
			};
		};
	};
	class ZenSleep_StimSyringe : Inventory_Base
	{
		scope = 2;
		displayName = "Stimulant Syringe";
		descriptionShort = "A syringe filled with a stimulant drug. I could use it to keep myself awake.";
		model = "\dz\gear\medical\syringe_Full.p3d";
		debug_ItemCategory = 7;
		hiddenSelections[] =
		{
			"camo"
		};
		hiddenSelectionsTextures[] =
		{
			"\ZenSleep\data\textures\syringefluid_yellow_ca.paa"
		};
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 60;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\Syringe_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Syringe_out
				{
					soundSet = "Syringe_out_SoundSet";
					id = 201;
				};
				class Syringe_spear
				{
					soundSet = "Syringe_spear_SoundSet";
					id = 202;
				};
				class Syringe_splash
				{
					soundSet = "Syringe_splash_SoundSet";
					id = 203;
				};
			};
		};
	};
	class ZenSleep_Empty_StimSyringe : Inventory_Base
	{
		scope = 2;
		displayName = "Empty Syringe";
		descriptionShort = "An empty stimulant syringe.";
		model = "\dz\gear\medical\syringe_empty.p3d";
		debug_ItemCategory = 7;
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 60;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\Syringe.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\Syringe_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\Syringe_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Syringe_out
				{
					soundSet = "Syringe_out_SoundSet";
					id = 201;
				};
				class Syringe_spear
				{
					soundSet = "Syringe_spear_SoundSet";
					id = 202;
				};
				class Syringe_splash
				{
					soundSet = "Syringe_splash_SoundSet";
					id = 203;
				};
			};
		};
	};
};
// TODO: Make sure all yawn sounds are equalized volume-wise
class CfgSoundShaders // Sound effect definitions
{
	class baseCharacter_SoundShader;
	// Male yawns
	class yawn1_Male_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\yawn1_m",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class yawn2_Male_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\yawn2_m",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class yawn3_Male_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\yawn3_m",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	// Female yawns
	class yawn1_Female_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\yawn1_f",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class yawn2_Female_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\yawn2_f",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class yawn3_Female_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\yawn3_f",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	// Male sleep sounds
	class sleep1_Male_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\sleep1_m",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class sleep2_Male_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\sleep2_m",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class sleep3_Male_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\sleep3_m",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	// Female sleep sounds
	class sleep1_Female_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\sleep1_f",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class sleep2_Female_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\sleep2_f",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
	class sleep3_Female_SoundShader : baseCharacter_SoundShader
	{
		samples[] = { {"ZenSleep\data\sounds\sleep3_f",1} };
		volume = 1;
		range = 30;
		rangeCurve[] = { {0,1},{0.5,0.7},{1,0} };
	};
};

class CfgSoundSets // Sound effect definitions
{
	class baseCharacter_SoundSet;
	// Male yawns
	class yawn1_Male_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "yawn1_Male_SoundShader" };
	};
	class yawn2_Male_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "yawn2_Male_SoundShader" };
	};
	class yawn3_Male_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "yawn3_Male_SoundShader" };
	};
	// Female yawns
	class yawn1_Female_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "yawn1_Female_SoundShader" };
	};
	class yawn2_Female_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "yawn2_Female_SoundShader" };
	};
	class yawn3_Female_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "yawn3_Female_SoundShader" };
	};
	// Male sleep sounds
	class sleep1_Male_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "sleep1_Male_SoundShader" };
	};
	class sleep2_Male_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "sleep2_Male_SoundShader" };
	};
	class sleep3_Male_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "sleep3_Male_SoundShader" };
	};
	// Female sleep sounds
	class sleep1_Female_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "sleep1_Female_SoundShader" };
	};
	class sleep2_Female_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "sleep2_Female_SoundShader" };
	};
	class sleep3_Female_SoundSet : baseCharacter_SoundSet
	{
		soundShaders[] = { "sleep3_Female_SoundShader" };
	};
};

class CfgSoundEffects
{
	class AttenuationsEffects
	{
		// Flashbang attenuation. TODO: Spend way too many hours playing with this instead of doing something productive.
		// Ideas: Increase attenuation and wacky sleep effects if player is very tired / has been sleeping a long time?
		class ZenSleepAttenuation
		{
			class Equalizer0
			{
				center[] = { 196,2000,8367,10000 };
				bandwidth[] = { 2,2,0.1,2 };
				gain[] = { 1.4125376,0.12589253,1.9952624,0.12589253 };
			};
			class Equalizer1
			{
				center[] = { 60,1000,2000,10000 };
				bandwidth[] = { 2,1,0.1,2 };
				gain[] = { 1,0.3548134,0.25118864,0.12589253 };
			};
			class Echo
			{
				WetDryMix = 0.25;
				Feedback = 0.2;
				Delay = 50;
			};
		};
	};
};