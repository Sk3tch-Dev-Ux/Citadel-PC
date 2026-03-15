class CfgPatches
{
	class Citadel_PC_Core_Script
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={};
	};
};
class CfgMods
{
	class Citadel_PC_Core
	{
		dir="Citadel_PC_Core";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Citadel_PC_Core";
		credits="TheDmitri";
		author="TheDmitri";
		authorID="0";
		version="1.1";
		extra=0;
		type="mod";
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
					"Citadel_PC_Core/scripts/Common",
					"Citadel_PC_Core/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"Citadel_PC/Scripts/4_World",
					"Citadel_PC_Core/scripts/Common",
					"Citadel_PC_Core/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"Citadel_PC_Core/scripts/Common",
					"Citadel_PC_Core/scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{

};
