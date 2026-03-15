modded class PluginRecipesManagerBase
{
    override void RegisterRecipies()
    {
        super.RegisterRecipies();
        
        // Registering custom GPU repair recipes
        RegisterRecipe(new CraftFullCitadelGraphicsCard1080TI);
        RegisterRecipe(new CraftFullCitadelRTX2080TI);
        RegisterRecipe(new CraftFullCitadelGraphicsCard4090TI);
        RegisterRecipe(new CraftFullCitadelGraphicsCard660);
    }
}
