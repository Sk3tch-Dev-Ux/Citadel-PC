modded class PluginRecipesManager extends PluginBase
{
	override void RegisterRecipies()
    {
        super.RegisterRecipies();			
        RegisterRecipe(new Pack_Citadel_Desk);
        RegisterRecipe(new Pack_Citadel_Filament_Holder);
        RegisterRecipe(new Unpack_Citadel_Desk_Box);
    }
}


