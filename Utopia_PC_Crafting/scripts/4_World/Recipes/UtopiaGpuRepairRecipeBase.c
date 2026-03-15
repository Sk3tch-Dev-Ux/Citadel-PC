// Shared base for all GPU repair recipes.
// Subclasses only need to override Init() and call SetupGpuRecipe() with their class name.
class UtopiaGpuRepairRecipeBase : RecipeBase
{
	protected void SetupGpuRecipe(string gpuClassName)
	{
		m_Name           = "Repair " + gpuClassName;
		m_IsInstaRecipe  = false;
		m_AnimationLength = 10;
		m_Specialty      = 0.01;

		// Ingredient 0: the GPU being repaired (any damage state, consumed)
		m_MinDamageIngredient[0] = -1;
		m_MaxDamageIngredient[0] = -1;
		m_MinQuantityIngredient[0] = -1;
		m_MaxQuantityIngredient[0] = -1;

		// Ingredient 1: repair kit (max damage 3, min qty 1, consumed)
		m_MinDamageIngredient[1] = -1;
		m_MaxDamageIngredient[1] = 3;
		m_MinQuantityIngredient[1] = 1;
		m_MaxQuantityIngredient[1] = -1;

		InsertIngredient(0, gpuClassName);
		m_IngredientAddHealth[0]   = 0;
		m_IngredientSetHealth[0]   = -1;
		m_IngredientAddQuantity[0] = 0;
		m_IngredientDestroy[0]     = true;

		InsertIngredient(1, "Utopia_GpuRepairKit");
		m_IngredientAddHealth[1]   = 0;
		m_IngredientSetHealth[1]   = -1;
		m_IngredientAddQuantity[1] = -1;
		m_IngredientDestroy[1]     = true;

		AddResult(gpuClassName);
		m_ResultSetFullQuantity[0]      = true;
		m_ResultSetQuantity[0]          = 1;
		m_ResultSetHealth[0]            = 100;
		m_ResultInheritsHealth[0]       = -1;
		m_ResultInheritsColor[0]        = -1;
		m_ResultToInventory[0]          = 1;
		m_ResultUseSoftSkills[0]        = false;
		m_ResultReplacesIngredient[0]   = -1;
	}

	override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		return true;
	}

	override void Do(ItemBase ingredients[], PlayerBase player, array<ItemBase> results, float specialty_weight)
	{
		super.Do(ingredients, player, results, specialty_weight);
	}
}
