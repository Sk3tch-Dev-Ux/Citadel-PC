class Utopia_Desk: BaseLaptop
{
	// Net-synced state flags — readable on client for action conditions and UI
	bool m_IsPCRunning;
	bool m_IsShuttingDown;
	bool m_IsStarting;
	bool m_IsMiningActive;

	// Server-side mining state
	ref map<string, bool>  m_CoinMiningStatus;
	ref map<string, float> m_MinedCoins;

	// How often the mining tick fires (seconds). Lower = more frequent payouts, more CPU.
	private static float MINING_TICK_INTERVAL = 60.0;

	void Utopia_Desk()
	{
		RegisterNetSyncVariableBool("m_IsPCRunning");
		RegisterNetSyncVariableBool("m_IsShuttingDown");
		RegisterNetSyncVariableBool("m_IsStarting");
		RegisterNetSyncVariableBool("m_IsMiningActive");

		m_CoinMiningStatus = new map<string, bool>();
		m_MinedCoins       = new map<string, float>();
	}

	// -------------------------------------------------------------------------
	// Cargo / inventory overrides
	// -------------------------------------------------------------------------

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return IsOpen();
	}

	override bool CanReleaseCargo(EntityAI attachment)
	{
		return IsOpen();
	}

	override bool CanDisplayAttachmentSlot(string slot_name)
	{
		return true;
	}

	// -------------------------------------------------------------------------
	// Initialisation — clear transient states that must not survive a restart
	// -------------------------------------------------------------------------

	override void EEInit()
	{
		super.EEInit();

		if (GetGame().IsServer())
		{
			m_IsShuttingDown = false;
			m_IsStarting     = false;
			SetSynchDirty();
		}
	}

	// -------------------------------------------------------------------------
	// Power management
	// -------------------------------------------------------------------------

	override void OnWorkStart()
	{
		super.OnWorkStart();

		if (!GetGame().IsServer())
			return;

		m_IsStarting     = true;
		m_IsShuttingDown = false;
		SetSynchDirty();

		GetUtopiaPcLogger().LogInfo("Utopia_Desk: Starting up...");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FinishStartup, 2000);
	}

	void FinishStartup()
	{
		m_IsPCRunning = true;
		m_IsStarting  = false;
		SetSynchDirty();

		GetUtopiaPcLogger().LogInfo("Utopia_Desk: Ready");
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		if (!GetGame().IsServer())
			return;

		// Cancel a pending startup if power is cut mid-boot
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(FinishStartup);

		m_IsShuttingDown = true;
		m_IsStarting     = false;
		SetSynchDirty();

		// Collect active coins first to avoid mutating the map during foreach
		array<string> activeCoins = new array<string>();
		foreach (string coinName, bool active : m_CoinMiningStatus)
		{
			if (active)
				activeCoins.Insert(coinName);
		}
		foreach (string coinName : activeCoins)
			StopMining(coinName);

		GetUtopiaPcLogger().LogInfo("Utopia_Desk: Shutting down...");
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FinishShutdown, 2000);
	}

	void FinishShutdown()
	{
		m_IsPCRunning    = false;
		m_IsShuttingDown = false;
		SetSynchDirty();

		GetUtopiaPcLogger().LogInfo("Utopia_Desk: Powered off");
	}

	// -------------------------------------------------------------------------
	// Mining
	// -------------------------------------------------------------------------

	bool IsAnyMiningActive()
	{
		foreach (string coinName, bool active : m_CoinMiningStatus)
		{
			if (active)
				return true;
		}
		return false;
	}

	void StartMining()
	{
		if (!GetGame().IsServer())
			return;

		UtopiaPcSettings settings = GetUtopiaPcModule().GetSettings();
		if (!settings)
		{
			GetUtopiaPcLogger().LogError("Utopia_Desk: StartMining — settings unavailable");
			return;
		}

		bool anyStarted = false;

		foreach (string coinName, UtopiaCoinSettings coinCfg : settings.coinSettings)
		{
			foreach (string slotName : g_UtopiaPcGraphicCardSlots)
			{
				ItemBase gpu = ItemBase.Cast(GetInventory().FindAttachmentByName(slotName));
				if (!gpu || gpu.IsRuined())
					continue;

				if (coinCfg.CanCardMine(gpu.GetType()))
				{
					m_CoinMiningStatus.Set(coinName, true);
					anyStarted = true;
					GetUtopiaPcLogger().LogInfo("Utopia_Desk: Mining " + coinName + " with " + gpu.GetType() + " [" + slotName + "]");
					break;
				}
			}
		}

		if (anyStarted)
		{
			m_IsMiningActive = true;
			SetSynchDirty();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(MiningTick, (int)(MINING_TICK_INTERVAL * 1000), true);
			GetUtopiaPcLogger().LogInfo("Utopia_Desk: Mining started");
		}
		else
		{
			GetUtopiaPcLogger().LogWarning("Utopia_Desk: StartMining — no compatible GPUs found for any configured coin");
		}
	}

	void StopMining(string coinName)
	{
		if (!GetGame().IsServer())
			return;

		m_CoinMiningStatus.Set(coinName, false);
		GetUtopiaPcLogger().LogInfo("Utopia_Desk: Stopped mining " + coinName);

		if (!IsAnyMiningActive())
		{
			m_IsMiningActive = false;
			SetSynchDirty();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(MiningTick);
			GetUtopiaPcLogger().LogInfo("Utopia_Desk: All mining stopped");
		}
	}

	void MiningTick()
	{
		if (!GetGame().IsServer())
			return;

		UtopiaPcSettings settings = GetUtopiaPcModule().GetSettings();
		if (!settings)
			return;

		foreach (string coinName, bool active : m_CoinMiningStatus)
		{
			if (!active)
				continue;

			UtopiaCoinSettings coinCfg = settings.GetCoinSettings(coinName);
			if (!coinCfg)
				continue;

			foreach (string slotName : g_UtopiaPcGraphicCardSlots)
			{
				ItemBase gpu = ItemBase.Cast(GetInventory().FindAttachmentByName(slotName));
				if (!gpu)
					continue;

				string gpuType = gpu.GetType();
				if (!coinCfg.CanCardMine(gpuType))
					continue;

				// --- Accumulate coins ---
				float yield = coinCfg.GetYield(gpuType);
				float balance = 0;
				if (m_MinedCoins.Contains(coinName))
					balance = m_MinedCoins.Get(coinName);
				m_MinedCoins.Set(coinName, balance + yield);

				GetUtopiaPcLogger().LogDebug("Utopia_Desk: +" + yield + " " + coinName + " | balance: " + (balance + yield) + " | GPU: " + gpuType);

				// --- GPU wear based on its configured lifetime ---
				// Damage is spread evenly over (lifetime / tickInterval) ticks so the GPU
				// lasts exactly `lifetime` seconds of continuous mining.
				int lifetime = settings.FindLifetimeForClassName(gpuType);
				if (lifetime > 0)
				{
					float ticksToDestroy = Math.Max(1, lifetime / MINING_TICK_INTERVAL);
					float damagePerTick  = gpu.GetMaxHealth("", "Health") / ticksToDestroy;
					gpu.AddHealth("", "Health", -damagePerTick);
				}

				if (gpu.IsRuined())
				{
					GetUtopiaPcLogger().LogWarning("Utopia_Desk: GPU " + gpuType + " ruined — stopping " + coinName);
					StopMining(coinName);
					break;
				}
			}
		}
	}

	// -------------------------------------------------------------------------
	// Coin withdrawal
	// -------------------------------------------------------------------------

	void WithdrawCoins(PlayerBase player)
	{
		if (!GetGame().IsServer())
			return;

		UtopiaPcSettings settings = GetUtopiaPcModule().GetSettings();
		if (!settings)
			return;

		foreach (string coinName, float amount : m_MinedCoins)
		{
			if (amount <= 0)
				continue;

			UtopiaCoinSettings coinCfg = settings.GetCoinSettings(coinName);
			if (!coinCfg || coinCfg.itemClass == "")
				continue;

			// Try player inventory first; fall back to dropping at feet
			EntityAI item = player.GetInventory().CreateInInventory(coinCfg.itemClass);
			if (!item)
			{
				item = EntityAI.Cast(GetGame().CreateObject(coinCfg.itemClass, player.GetPosition(), false, true));
				GetUtopiaPcLogger().LogWarning("Utopia_Desk: No inventory space for " + coinName + " — dropped on ground");
			}

			if (item)
			{
				ItemBase coinItem = ItemBase.Cast(item);
				if (coinItem)
					coinItem.SetQuantity(amount);

				m_MinedCoins.Set(coinName, 0);
				GetUtopiaPcLogger().LogInfo("Utopia_Desk: Withdrew " + amount + " " + coinName);
			}
		}
	}

	// -------------------------------------------------------------------------
	// Persistence
	// -------------------------------------------------------------------------

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(m_IsPCRunning);

		// Only persist non-zero coin balances
		int count = 0;
		foreach (string cn, float amt : m_MinedCoins)
		{
			if (amt > 0)
				count++;
		}
		ctx.Write(count);
		foreach (string cn, float amt : m_MinedCoins)
		{
			if (amt > 0)
			{
				ctx.Write(cn);
				ctx.Write(amt);
			}
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_IsPCRunning))
			return false;

		int count;
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; i++)
		{
			string cn;
			float  amt;
			if (!ctx.Read(cn) || !ctx.Read(amt))
				return false;

			m_MinedCoins.Set(cn, amt);
		}

		SetSynchDirty();
		return true;
	}
}
