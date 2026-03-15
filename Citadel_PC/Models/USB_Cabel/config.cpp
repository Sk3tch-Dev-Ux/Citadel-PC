class CfgPatches
{
	class Citadel_PC
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Citadel_USB_Cabel: Inventory_Base
	{
		scope = 2;
		model = "Citadel_PC\Models\USB_Cabel\USB_Cabel.p3d";
		displayName = "USB Cabel";
		descriptionShort = "";
		weight = 500;
		itemSize[] = {2,2};
		rotationFlags = 17;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[]= {{1,{}},{0.69999999,{}},{0.5,{}},{0.30000001,{}},{0,{}}};
				};
			};
		};
	};
};
