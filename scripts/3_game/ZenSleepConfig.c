// TODO: Organize these settings better using classes, so we can have categories in the JSON file (eg. MaxRestConfig: {} )
class ZenSleepConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenSleepConfig.json";

	// Main config data
	int MaxWakeTime = 240; // Max time you can stay awake before you fall unconscious
	bool AllowInventoryWhileSleep = false; // Enable/disable the player accessing their inventory while sleeping
	bool CanUnconInVehicle = true; // Enable/disable falling uncon from tiredness in vehicles
	int SleepBlackScreen = 1; // Enable/disable screen going black when lying down
	bool TextNotificationOn = true; // Enable/disable text notifications of rest level
	int TextNotificationFrequency = 5; // Sends a text notification update every 5 rest ticks
	bool HideHudWhileSleeping = true; // Hide irrelevant HUD actions when sleeping (ie. stamina, item in hands, nearby objets)?
	bool OnlyShowSleepOnInventory = false; // Enable/disable only showing sleep meter while inventory is open
	int OnlyShowSleepAbovePercent = 0; // Sets what sleep level to show the sleep meter at (optional - 0 = disabled)
	// Random chance config
	float SleepSoundPercentChance = 0.08; // Percentage chance of playing a sleep sound while player is sleeping
	int YawnPercentStart = 20; // Percentage of tiredness that you start yawning & blinking
	float YawnPercentChance = 0.065; // Percentage chance of yawning AFTER we are below YawnPercentStart tiredness
	int UnconPercentStart = 10; // Percentage of tiredness that you start randomly falling uncon
	float UnconPercentChance = 0.01; // Percentage chance of falling uncon AFTER we are below UnconPercentStart
	// Influenza config
	int InfluenzaInjectNoFire = 5; // How much influenza to inject the player with if they're not sleeping near a fire (this = random/1000)
	float InfluenzaMultiplierNightNoFire = 1.5; // How much more influenza to inject if there is no fire nearby
	float InfluenzaMultiplierWetNoFire = 2.0; // How much more influenza to inject if there is no fire nearby AND the player is wet
	// Max rest config
	int MaxRestDayNoFire = 50; // Max sleep % you can gain while sleeping during the day with no fire or rest object that overrides this setting
	int MaxRestDayWithFire = 75; // Max sleep % you can gain while sleeping during the day with a fire
	int MaxRestNightNoFire = 80; // Max sleep % you can gain while sleeping during the night with no fire
	int MaxRestNightWithFire = 100; // Max sleep % you can gain while sleeping during the night with a fire nearby
	int MaxRestWhenWetDay = 40; // Max amount of sleep you can get if your clothes are wet during the day
	int MaxRestWhenWetNight = 75; // Max amount of sleep you can get if your clothes are wet at night
	// Temperature config
	bool HeatBuffCountsAsFire = false; // Whether or not to count the player's heat buff as sleeping near a fire
	int OutsideFireTemp = 28; // How hot a fire typically is when lit outside (this varies depending on the map & winter - default value is for regular chern/livonia)
	int InsideFireTemp = 38; // How hot a fire typically is when lit inside (this varies depending on the map & winter - default value is for regular chern/livonia)
	// Sleep accelerator config
	bool WetnessCancelsFireAccelerator = true; // If player is wet, then the player will not sleep any faster next to a fire
	float BaseSleepAccelerator = 1.0; // How fast the player sleeps with no accelerator condition
	float AsleepAccelerator = 0.001; // How much faster you sleep each rest tick (rewarding you for staying asleep longer)
	int OutsideFireAcceleratorPercent = 5; // How much quicker you sleep outside next to a fire (as a percentage)
	int InsideNoFireAcceleratorPercent = 10; // How much quicker you sleep inside a building with no fire (as a percentage)
	int InsideFireAcceleratorPercent = 25; // How much quicker you sleep inside a building near a fire (as a percentage)
	// Night time config (eg. to make night-time from 10:30PM->5:45AM set it to 22, 30, 5, 45)
	int NightTimeStartHour = 0; // The starting hour of night time - overrides vanilla IsNight() - in 24 hour time (eg. 23 = 11pm)
	int NightTimeStartMin = 0; // The starting minute of night time - between 0-59
	int NightTimeEndHour = 0; // The ending hour of night time
	int NightTimeEndMin = 0; // The ending minute of night time
	// Energy drinks & rest object config
	ref array<ref EnergyDrink> EnergyDrinks = new array<ref EnergyDrink>; // List of energy drink objects (doesn't need to be drinks - can be any consumable)
	ref array<ref RestObject> RestObjects = new array<ref RestObject>; // List of rest objects that you can sleep near for a boost
	// String config (only applicable if TextNotificationOn = true)
	string Str_RestUpdate = "My rest level is"; // Rest level update
	string Str_RestUpdate1 = "and I'm cold...";
	string Str_RestUpdate2 = "and I'm getting cold...";
	string Str_RestUpdate3 = "and I'm warm.";
	string Str_RestUpdate4 = "and I'm comfortably warm.";
	string Str_RestUpdate5 = "and my clothes are wet and uncomfortable...";
	string Str_ITooTired1 = "I feel really tired... I should lie down soon and get some rest.";
	string Str_ITooTired2 = "I've been awake for a long time now... I should lie down and get some rest.";
	string Str_ITooTired3 = "I don't have much energy... I should lie down soon and get some rest.";
	string Str_CantSleep1 = "I don't think I can sleep any longer...";
	string Str_CantSleep2 = "I don't think I can sleep any longer, I'm too cold...";
	string Str_CantSleep3 = "I don't think I can sleep any longer, it's too bright and I'm cold...";
	string Str_CantSleep4 = "I don't think I can sleep any longer, it's too bright...";
	string Str_CantSleep5 = "I don't think I can sleep any longer, my clothes are wet and I'm cold...";
	// Debug config
	float TirednessHudX = 0.855; // X position for tiredness widget
	float TirednessHudY = 0.03; // Y position for tiredness widget
	string AdminSteam64ID = "steamID64(Dec): www.steamidfinder.com"; // Your steam 64 ID (decimal version) - optional, allows you to reload json config with a hotkey
	int DebugOn = 0; // Enable/disable debug mode (1 = full verbosity, 2 = low verbosity)
	int RestUpdateTick = 3; // How often to update player ticks (don't adjust this unless absolutely necessary (eg. if server performance is affected by lots of players), it will require a re-balance of nearly all the above values!)

	// Load config file or create default file if config doesn't exsit
	void Load() 
	{
		if (GetGame().IsServer()) 
		{
			if (FileExist(zenModFolder + zenConfigName))
			{ // If config exists, load file
				JsonFileLoader<ZenSleepConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			}
			else 
			{ // Config file does not exist, create default file
				// Save default settings for energy drinks
				EnergyDrinks.Insert(new EnergyDrink("SodaCan_Pipsi", 20));
				EnergyDrinks.Insert(new EnergyDrink("SodaCan_Cola", 25));
				EnergyDrinks.Insert(new EnergyDrink("SodaCan_Spite", 10));
				EnergyDrinks.Insert(new EnergyDrink("SodaCan_Kvass", 5));
				EnergyDrinks.Insert(new EnergyDrink("SodaCan_Fronta", 15));
				EnergyDrinks.Insert(new EnergyDrink("Epinephrine", -15));
				EnergyDrinks.Insert(new EnergyDrink("Morphine", -20));
				EnergyDrinks.Insert(new EnergyDrink("AntiChemInjector", -100));
				EnergyDrinks.Insert(new EnergyDrink("ZenSleep_Syringe", -100));
				EnergyDrinks.Insert(new EnergyDrink("ZenSleep_StimSyringe", 100));
				// Save default settings for rest objects
				RestObjects.Insert(new RestObject("MediumTent", 60, 100, 10, 0));
				// Save config
				Save();
			}
		}
	}

	// Save config
	void Save() 
	{
		if(!FileExist(zenModFolder))
		{ // If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenSleepConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Return an energy drink with the given type name
	EnergyDrink GetEnergyDrink(string type)
	{
		string itemType = type;
		itemType.ToLower();
		for (int i = 0; i < EnergyDrinks.Count(); i++)
		{
			string toFind = EnergyDrinks.Get(i).ItemType;
			toFind.ToLower();
			if (itemType.Contains(toFind)) // Use Contains() to find similar items with an item base (eg. Zagorky / ZagorkyChocolate)
			{
				return EnergyDrinks.Get(i);
			}
		}

		return new EnergyDrink();
	}

	// Return a rest object with the given type name
	RestObject GetRestObject(string type)
	{
		string objectType = type;
		objectType.ToLower();
		for (int i = 0; i < RestObjects.Count(); i++)
		{
			string toFind = RestObjects.Get(i).ObjectType;
			toFind.ToLower();
			if (objectType.Contains(toFind))
			{
				return RestObjects.Get(i);
			}
		}

		return new RestObject();
	}
};

// Defines energy drink config
class EnergyDrink
{
	string ItemType = ""; // Item type name
	int EnergyGained = 0; // How much energy this drink gives the player

	void EnergyDrink(string itemParam = "", int energyParam = 0)
	{
		ItemType = itemParam;
		EnergyGained = energyParam;
	}
};

// Defines rest objects config
class RestObject
{
	string ObjectType = ""; // Object type name
	int MaxRestDay = 50; // Max rest achievable when sleeping near this object during the day
	int MaxRestNight = 100; // Max rest achievable when sleeping near this object during the night
	int SleepAcceleratorPercent = 0; // Sleep accelerator time modifier for this object
	bool Influenza = true; // Can you catch influenza while sleeping in this rest object?

	// Define a new rest object config
	void RestObject(string objectParam = "", int dayParam = 0, int nightParam = 0, int sleepParam = 1, bool fluParam = true) 
	{
		ObjectType = objectParam;
		MaxRestDay = dayParam;
		MaxRestNight = nightParam;
		SleepAcceleratorPercent = sleepParam;
		Influenza = fluParam;
	}
};

// Save config data
ref ZenSleepConfig m_ZenSleepConfig;

// Helper function to return Config data storage object
static ZenSleepConfig GetZenSleepConfig()
{
	if(!m_ZenSleepConfig)
	{
		Print("[ZenSleepConfig] Init");
		m_ZenSleepConfig = new ZenSleepConfig;
		m_ZenSleepConfig.Load();
	}

	return m_ZenSleepConfig;
};