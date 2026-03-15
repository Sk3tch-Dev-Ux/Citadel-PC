[CF_RegisterModule(CitadelPcCoreModule)]
class CitadelPcCoreModule : CF_ModuleWorld
{
    ref CitadelPcSettings settings;

    override void OnInit()
    {
        super.OnInit();

        EnableInvokeConnect();
        EnableMissionStart();
    }

    CitadelPcSettings GetSettings()
    {
        return settings;
    }

    void InitializeModule()
    {
        GetCitadelPcLogger().LogInfo("Citadel_PC_Core has started");

        if(GetGame().IsServer())
        {
            settings = CitadelPcSettings.Load();
        }
        else
        {
            settings = new CitadelPcSettings();
        }

        InitRPCs();
    }

    void InitRPCs()
    {
        //client
        if(GetGame().IsClient())
        {
            GetRPCManager().AddRPC("CitadelPcSystem", "GetConfigResponse", this, SingeplayerExecutionType.Client);
        }
        else //server
        {
            /*example : Register RPC on the server side
            GetRPCManager().AddRPC("CitadelPcSystem", "FunctionName", this, SingeplayerExecutionType.Server);
            */
        }
    }

    //RPC Method trigger when the method GetRPCmANA
    void GetConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type != CallType.Client)
            return;

        Param1<CitadelPcSettings> data;
        if(!ctx.Read(data)){
            return;
        }

        settings = data.param1;
    }

    void OnPlayerConnect(PlayerBase player, PlayerIdentity identity)
    {
        GetRPCManager().SendRPC("CitadelPcSystem", "GetConfigResponse", new Param1<CitadelPcSettings>(settings), true, identity);
    }

    override void OnInvokeConnect(Class sender, CF_EventArgs args)
    {
        super.OnInvokeConnect(sender, args);

        auto cArgs = CF_EventPlayerArgs.Cast(args);

        if(!IsMissionHost()){
            return;
        }

        if(!cArgs.Player || !cArgs.Identity)
            return;

        OnPlayerConnect(cArgs.Player, cArgs.Identity);
    }

    override void OnMissionStart(Class sender, CF_EventArgs args)
    {
        super.OnMissionStart(sender, args);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InitializeModule, 1000); 
    }

}

static ref CitadelPcCoreModule GetCitadelPcModule()
{
    return CitadelPcCoreModule.Cast(CF_ModuleCoreManager.Get(CitadelPcCoreModule));
}

