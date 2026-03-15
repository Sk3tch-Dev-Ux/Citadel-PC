class BaseCamera extends ItemBase
{
    private static ref array<BaseCamera> arrCamera = new array<BaseCamera>();
    private int  netSynch_laptopId = 0;
    private bool netSynch_IsOn     = false;

    private ref BaseCameraSettings m_Settings;

    void BaseCamera()
    {
        RegisterNetSyncVariableBool("netSynch_IsOn");
        RegisterNetSyncVariableInt("netSynch_laptopId");
        arrCamera.Insert(this);
    }

    void ~BaseCamera()
    {
        int idx = arrCamera.Find(this);
        if (idx != -1)
            arrCamera.Remove(idx);
    }

    override void EEInit()
    {
        super.EEInit();

        // Load settings once per camera instance so GetCameraConsumption()
        // never touches the filesystem at runtime.
        if (GetGame().IsServer())
            m_Settings = BaseCameraSettings.Load();
    }

    int GetLaptopId()
    {
        return netSynch_laptopId;
    }

    static array<BaseCamera> GetCameras()
    {
        GetBaseCameraLogger().LogDebug("GetCameras: count" + arrCamera.Count());
        return arrCamera;
    }

    void SetLaptopId(int laptopId)
    {
        GetBaseCameraLogger().LogDebug("SetLaptopId:laptopId:" + laptopId);
        this.netSynch_laptopId = laptopId;
        SetSynchDirty();
    }

    float GetCameraConsumption()
    {
        if (!m_Settings)
            return 0.25; // fallback: 0.5% * 50 / 100

        return (m_Settings.cameraBatteryPerCentConsumptionPerSec * 50.0) / 100.0;
    }

    void SetCameraState(bool state)
    {
        GetBaseCameraLogger().LogDebug("SetCameraState:state:" + state);
        netSynch_IsOn = state;
        SetSynchDirty();
    }

    bool GetCameraState()
    {
        return netSynch_IsOn;
    }

    override void OnWorkStart()
	{
        super.OnWorkStart();
		if( GetGame().IsDedicatedServer())
		{
            SetCameraState(true);
            GetBaseCameraLogger().LogDebug("OnWorkStart");
            GetCompEM().SetEnergyUsage(GetCameraConsumption());
		}
	}

    override void OnWorkStop()
	{
        super.OnWorkStop();
		if( GetGame().IsDedicatedServer())
		{
            SetCameraState(false);
            GetBaseCameraLogger().LogDebug("OnWorkStop");
		}
	}

    int GetBatteryQuantity()
    {
        Battery9V battery = Battery9V.Cast( GetCompEM().GetEnergySource() );
        if( battery == null )
            return 0;
        
        return battery.GetQuantity();
    }

    override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		ctx.Write( netSynch_laptopId );
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if (!ctx.Read( netSynch_laptopId ))
			return false;
		
		SetSynchDirty();	

		return true;
	}

    override float GetDeployTime()
    {
        return 10.0;
    }

    override bool IsDeployable()
	{
		return true;
	}	

	override bool IsOneHandedBehaviour()
	{
		return true;
	}

    override void SetActions()
    {
		super.SetActions();
        AddAction( ActionTogglePlaceObject );
		AddAction(ActionPlaceObject);
    }
};