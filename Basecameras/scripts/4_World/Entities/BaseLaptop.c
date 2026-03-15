class BaseLaptop extends ItemBase
{
	private int netSynch_laptopId = -1;

	ref array<BaseCamera> arrayCamera = new array<BaseCamera>();

	// Component validation cache.  Marked dirty whenever an attachment changes
	// so HasRequiredComponentsForCCTV() only does inventory lookups once per change
	// instead of on every action-condition poll.
	private bool m_ComponentsDirty    = true;
	private bool m_HasRequiredComponents = false;

	void BaseLaptop()
	{
		RegisterNetSyncVariableInt("netSynch_laptopId");
	}

	// -------------------------------------------------------------------------
	// Camera management
	// -------------------------------------------------------------------------

	int GetLaptopId()
	{
		if (netSynch_laptopId == -1)
			GenerateLaptopId();

		return netSynch_laptopId;
	}

	array<BaseCamera> GetCameras()
	{
		GetBaseCameraLogger().LogDebug("Laptop:GetCameras");
		arrayCamera.Clear();

		array<BaseCamera> arrCamera = BaseCamera.GetCameras();
		foreach (BaseCamera baseCamera : arrCamera)
		{
			if (!baseCamera)
				continue;

			if (baseCamera.GetLaptopId() == netSynch_laptopId)
				arrayCamera.Insert(baseCamera);
		}

		GetBaseCameraLogger().LogDebug("Laptop:GetCameras result count: " + arrayCamera.Count());
		return arrayCamera;
	}

	// -------------------------------------------------------------------------
	// Component validation (cached)
	// -------------------------------------------------------------------------

	bool HasRequiredComponentsForCCTV()
	{
		if (m_ComponentsDirty)
		{
			m_HasRequiredComponents = ComputeRequiredComponents();
			m_ComponentsDirty = false;
		}
		return m_HasRequiredComponents;
	}

	private bool ComputeRequiredComponents()
	{
		return HasWifiRouter()
			&& HasRequiredKeyboard()
			&& HasRequiredMonitor()
			&& HasRequiredMouse()
			&& HasRequiredMother_Board()
			&& HasCooler()
			&& GetPSUCount()          >= 1
			&& GetRamCount()          >= 1
			&& GetKingstonCount()     >= 1
			&& GetGraphicCardCount()  >= 1
			&& GetFanCount()          >= 1;
	}

	// Invalidate the cache whenever an attachment is added or removed
	override void EEItemAttached(EntityAI item, string slotName)
	{
		super.EEItemAttached(item, slotName);
		m_ComponentsDirty = true;
	}

	override void EEItemDetached(EntityAI item, string slotName)
	{
		super.EEItemDetached(item, slotName);
		m_ComponentsDirty = true;
	}

	// -------------------------------------------------------------------------
	// Component presence checks
	// -------------------------------------------------------------------------

	bool HasRequiredKeyboard()    { return GetInventory().FindAttachmentByName("Keyboard")     != null; }
	bool HasRequiredMonitor()     { return GetInventory().FindAttachmentByName("Monitor")       != null; }
	bool HasRequiredMouse()       { return GetInventory().FindAttachmentByName("Mouse")         != null; }
	bool HasRequiredMother_Board(){ return GetInventory().FindAttachmentByName("Mother_Board")  != null; }
	bool HasWifiRouter()          { return GetInventory().FindAttachmentByName("WiFi_Router")   != null; }
	bool HasCooler()              { return GetInventory().FindAttachmentByName("Cooler")        != null; }

	int GetPSUCount()          { return CountSlots("PSU_",            2); }
	int GetRamCount()          { return CountSlots("Ram_",            4); }
	int GetKingstonCount()     { return CountSlots("Kingston_",       3); }
	int GetGraphicCardCount()  { return CountSlots("Graphics_Card_",  3); }
	int GetFanCount()          { return CountSlots("Fan_",            2); }

	// Counts numbered attachment slots (prefix + "1" … prefix + maxSlots)
	private int CountSlots(string prefix, int maxSlots)
	{
		int count = 0;
		for (int i = 1; i <= maxSlots; i++)
		{
			if (GetInventory().FindAttachmentByName(prefix + i.ToString()))
				count++;
		}
		return count;
	}

	// -------------------------------------------------------------------------
	// Identity / persistence
	// -------------------------------------------------------------------------

	override void EEInit()
	{
		super.EEInit();
	}

	void GenerateLaptopId()
	{
		GetBaseCameraLogger().LogDebug("Laptop:GenerateLaptopId");
		netSynch_laptopId = BaseCameraHelper.GetUniqueId(this);
		GetBaseCameraLogger().LogDebug("Laptop:netSynch_laptopId " + netSynch_laptopId.ToString());
		SetSynchDirty();
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		ctx.Write(netSynch_laptopId);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(netSynch_laptopId))
			return false;

		SetSynchDirty();
		return true;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionPlugIn);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
}
