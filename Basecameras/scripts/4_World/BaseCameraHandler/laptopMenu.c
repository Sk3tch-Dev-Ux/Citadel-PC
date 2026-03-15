class ILaptopMenu : UIScriptedMenu
{
	// --- Core references ---
	BaseLaptop laptop;
	Utopia_Desk desk;
	BaseCamera  currentCamera;

	ref array<BaseCamera> arrCamera;

	protected Camera cameraScreen;

	// --- Widget references ---
	private Widget       wBackground;
	private Widget       wNoBatteryPanel;
	private Widget       wCameraStatusPanel;
	private XComboBoxWidget wComboSwitchCamera;
	// menuBar (bottom)
	private TextWidget   wTextMiningStatus;
	private TextWidget   wTextBatteryPerCent;
	// camera status overlay
	private TextWidget   wTextCameraLabel;
	private TextWidget   wTextCameraStatus;
	private TextWidget   wTextSignalStrength;
	private TextWidget   wTextCameraBattery;
	// no-signal screen
	private TextWidget   wTextNoSignalReason;

	// --- Constants ---
	private static int HOME_SCREEN    = 0;
	private static bool CAMERA_ON     = true;
	private static bool CAMERA_OFF    = false;

	void ILaptopMenu()
	{
		arrCamera = new array<BaseCamera>();
	}

	// -------------------------------------------------------------------------
	// Initialisation
	// -------------------------------------------------------------------------

	override Widget Init()
	{
		layoutRoot          = GetGame().GetWorkspace().CreateWidgets("Basecameras/datasets/layouts/laptopMenu.layout");
		wBackground         = Widget.Cast(layoutRoot.FindAnyWidget("background"));
		wNoBatteryPanel     = Widget.Cast(layoutRoot.FindAnyWidget("noBatteryPanel"));
		wCameraStatusPanel  = Widget.Cast(layoutRoot.FindAnyWidget("cameraStatusPanel"));
		wComboSwitchCamera  = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("comboSwitchCamera"));
		wTextMiningStatus   = TextWidget.Cast(layoutRoot.FindAnyWidget("textMiningStatus"));
		wTextBatteryPerCent = TextWidget.Cast(layoutRoot.FindAnyWidget("textBatteryPerCent"));
		wTextCameraLabel    = TextWidget.Cast(layoutRoot.FindAnyWidget("textCameraLabel"));
		wTextCameraStatus   = TextWidget.Cast(layoutRoot.FindAnyWidget("textCameraStatus"));
		wTextSignalStrength = TextWidget.Cast(layoutRoot.FindAnyWidget("textSignalStrength"));
		wTextCameraBattery  = TextWidget.Cast(layoutRoot.FindAnyWidget("textCameraBattery"));
		wTextNoSignalReason = TextWidget.Cast(layoutRoot.FindAnyWidget("textNoSignalReason"));

		layoutRoot.Show(false);
		return layoutRoot;
	}

	void SetLaptop(BaseLaptop _laptop)
	{
		laptop = _laptop;
		desk   = Utopia_Desk.Cast(_laptop);
	}

	void InitLoading()
	{
		GetBaseCameraLogger().LogDebug("InitLoading");
		arrCamera = laptop.GetCameras();
		InitLoadingCameras();
		CreateCameraScreen();
		UpdateMiningStatus();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateScreen, 1000, true);
	}

	void InitLoadingCameras()
	{
		wComboSwitchCamera.ClearAll();
		wComboSwitchCamera.AddItem("HOME");
		for (int i = 0; i < arrCamera.Count(); i++)
			wComboSwitchCamera.AddItem(string.Format("CAM %1", i + 1));
		wComboSwitchCamera.SetCurrentItem(HOME_SCREEN);
	}

	// -------------------------------------------------------------------------
	// Per-second update
	// -------------------------------------------------------------------------

	void UpdateScreen()
	{
		UpdateMiningStatus();

		if (!currentCamera)
		{
			wTextBatteryPerCent.SetText("---");
			return;
		}

		UpdateBattery();
		UpdateCameraStatusPanel();

		if (IsCameraTurnedOff())
		{
			SwitchOffPreviousCamera();
			ShowNoSignalScreen("OFFLINE");
			return;
		}

		if (IsBatteryOut())
		{
			SwitchOffPreviousCamera();
			ShowNoSignalScreen("NO POWER");
			return;
		}

		HideNoSignalScreen();
		UpdateCameraScreen();
	}

	// -------------------------------------------------------------------------
	// Mining status (bottom-left of toolbar)
	// -------------------------------------------------------------------------

	void UpdateMiningStatus()
	{
		if (!desk)
		{
			wTextMiningStatus.SetText("");
			return;
		}

		if (!desk.m_IsPCRunning)
		{
			wTextMiningStatus.SetText("OFFLINE");
			wTextMiningStatus.SetColor(ARGB(255, 80, 80, 80));
			return;
		}

		if (desk.m_IsMiningActive)
		{
			wTextMiningStatus.SetText("MINING");
			wTextMiningStatus.SetColor(ARGB(255, 51, 224, 51));
		}
		else
		{
			wTextMiningStatus.SetText("IDLE");
			wTextMiningStatus.SetColor(ARGB(255, 128, 128, 128));
		}
	}

	// -------------------------------------------------------------------------
	// Camera status overlay
	// -------------------------------------------------------------------------

	void UpdateCameraStatusPanel()
	{
		if (!currentCamera)
		{
			wCameraStatusPanel.Show(false);
			return;
		}

		wCameraStatusPanel.Show(true);

		// Camera label
		int camIdx = wComboSwitchCamera.GetCurrentItem();
		wTextCameraLabel.SetText(string.Format("CAM %1", camIdx));

		// Online / offline
		if (!currentCamera.GetCameraState() || IsBatteryOut())
		{
			wTextCameraStatus.SetText("OFFLINE");
			wTextCameraStatus.SetColor(ARGB(255, 224, 51, 51));
		}
		else
		{
			wTextCameraStatus.SetText("ONLINE");
			wTextCameraStatus.SetColor(ARGB(255, 51, 224, 51));
		}

		// Signal strength label and color
		float noise    = GetNoiseIntensity();
		string sigText = GetSignalLabel(noise);
		int    sigR, sigG, sigB;
		GetSignalColor(noise, sigR, sigG, sigB);
		wTextSignalStrength.SetText(sigText);
		wTextSignalStrength.SetColor(ARGB(255, sigR, sigG, sigB));

		// Battery
		int battery = GetCameraBatteryPercent();
		wTextCameraBattery.SetText(string.Format("BATTERY: %1%%", battery));
		if (battery > 50)
			wTextCameraBattery.SetColor(ARGB(255, 140, 217, 140));
		else if (battery > 20)
			wTextCameraBattery.SetColor(ARGB(255, 217, 200, 51));
		else
			wTextCameraBattery.SetColor(ARGB(255, 217, 80, 80));
	}

	// -------------------------------------------------------------------------
	// Battery
	// -------------------------------------------------------------------------

	int GetCameraBatteryPercent()
	{
		if (!currentCamera)
			return 0;
		return currentCamera.GetBatteryQuantity() * 2;
	}

	void UpdateBattery()
	{
		int pct = GetCameraBatteryPercent();
		wTextBatteryPerCent.SetText(pct.ToString() + "%");
	}

	bool IsBatteryOut()
	{
		return GetCameraBatteryPercent() <= 0;
	}

	bool IsCameraTurnedOff()
	{
		return !currentCamera.GetCameraState();
	}

	// -------------------------------------------------------------------------
	// Signal / noise
	// -------------------------------------------------------------------------

	// Returns a smooth 0-100 noise value that scales linearly with distance.
	float GetNoiseIntensity()
	{
		if (!currentCamera)
			return 0;

		float dist = vector.Distance(currentCamera.GetPosition(), laptop.GetPosition());
		return Math.Clamp((dist / 1000.0) * 100.0, 0.0, 100.0);
	}

	string GetSignalLabel(float noise)
	{
		if (noise < 20) return "EXCELLENT";
		if (noise < 45) return "GOOD";
		if (noise < 65) return "WEAK";
		if (noise < 85) return "POOR";
		return "NO SIGNAL";
	}

	void GetSignalColor(float noise, out int r, out int g, out int b)
	{
		if (noise < 20)      { r = 51;  g = 224; b = 51;  return; } // green
		if (noise < 45)      { r = 140; g = 217; b = 51;  return; } // yellow-green
		if (noise < 65)      { r = 217; g = 200; b = 51;  return; } // yellow
		if (noise < 85)      { r = 217; g = 120; b = 51;  return; } // orange
		                       r = 217; g = 51;  b = 51;            // red
	}

	// -------------------------------------------------------------------------
	// Camera feed post-processing
	// -------------------------------------------------------------------------

	void ResetEffects()
	{
		PPEffects.ResetDOFOverride();
		PPEffects.ResetAll();
		PPEffects.SetNVParams(1.0, 0.0, 2.35, 2.75);
	}

	void GiveEffects()
	{
		float noise = GetNoiseIntensity();
		PPEffects.SetColorizationNV(0.75, 0.75, 0.75);
		PPEffects.SetNVParams(3.0, noise, 9.0, 1.0);
	}

	// -------------------------------------------------------------------------
	// Camera screen (staticcamera object in world)
	// -------------------------------------------------------------------------

	void CreateCameraScreen()
	{
		vector pos = GetGame().GetPlayer().GetPosition();
		pos[1] = pos[1] + 2;

		cameraScreen = Camera.Cast(g_Game.CreateObject("staticcamera", pos, true));
		cameraScreen.SetActive(true);
		ShowHomeScreen();
	}

	void UpdateCameraScreen()
	{
		if (!currentCamera || !cameraScreen)
			return;

		cameraScreen.SetPosition(currentCamera.ModelToWorld(currentCamera.GetMemoryPointPos("camera_start")));
		cameraScreen.LookAt(currentCamera.ModelToWorld(currentCamera.GetMemoryPointPos("camera_end")));
	}

	void ShowHomeScreen()
	{
		if (!desk || !cameraScreen)
			return;

		cameraScreen.SetPosition(desk.ModelToWorld(desk.GetMemoryPointPos("pos_start")));
		cameraScreen.LookAt(desk.ModelToWorld(desk.GetMemoryPointPos("pos_end")));
	}

	void DeleteCameraScreen()
	{
		if (!cameraScreen)
			return;

		cameraScreen.SetActive(false);
		GetGame().ObjectDelete(cameraScreen);
		cameraScreen = null;
	}

	// -------------------------------------------------------------------------
	// No-signal screen
	// -------------------------------------------------------------------------

	void ShowNoSignalScreen(string reason)
	{
		wBackground.SetColor(ARGB(255, 0, 0, 0));
		wNoBatteryPanel.Show(true);
		wTextNoSignalReason.SetText(reason);
		wCameraStatusPanel.Show(false);
	}

	void HideNoSignalScreen()
	{
		wBackground.SetColor(ARGB(0, 0, 0, 0));
		wNoBatteryPanel.Show(false);
	}

	// -------------------------------------------------------------------------
	// Camera switching
	// -------------------------------------------------------------------------

	void OnSwitchCamera()
	{
		SwitchOffPreviousCamera();

		currentCamera = GetCurrentCamera();

		if (!currentCamera)
		{
			HideNoSignalScreen();
			wCameraStatusPanel.Show(false);
			wTextBatteryPerCent.SetText("---");
			ShowHomeScreen();
			ResetEffects();
			return;
		}

		GiveEffects();
		SwitchOnCamera();
		UpdateCameraStatusPanel();
	}

	BaseCamera GetCurrentCamera()
	{
		int idx = wComboSwitchCamera.GetCurrentItem();
		if (idx == HOME_SCREEN || idx - 1 >= arrCamera.Count())
			return null;

		return arrCamera[idx - 1];
	}

	void SwitchOnCamera()
	{
		if (!currentCamera)
			return;

		SwitchCameraState(currentCamera, CAMERA_ON);
	}

	void SwitchOffPreviousCamera()
	{
		if (!currentCamera)
			return;

		SwitchCameraState(currentCamera, CAMERA_OFF);
	}

	void SwitchCameraState(BaseCamera camera, bool state)
	{
		GetRPCManager().SendRPC("BaseCamera", "SwitchBaseCameraRequest", new Param2<BaseCamera, bool>(camera, state), true, null);
	}

	// -------------------------------------------------------------------------
	// Menu lifecycle
	// -------------------------------------------------------------------------

	override void OnShow()
	{
		super.OnShow();
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		layoutRoot.Show(true);
		GetGame().GetUIManager().ShowUICursor(true);
		GetGame().GetMission().GetHud().Show(false);
		SetFocus(layoutRoot);
		GiveEffects();
	}

	void ResetPlayerCamera()
	{
		PlayerBase player = GetGame().GetPlayer();
		GetGame().SelectPlayer(null, player);
	}

	override void OnHide()
	{
		super.OnHide();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(UpdateScreen);
		GetGame().GetInput().ResetGameFocus();
		GetGame().GetMission().PlayerControlEnable(true);
		GetGame().GetUIManager().ShowUICursor(false);
		GetGame().GetMission().GetHud().Show(true);
		SwitchOffPreviousCamera();
		DeleteCameraScreen();
		arrCamera.Clear();
		currentCamera = null;
		ResetPlayerCamera();
		ResetEffects();
	}

	// -------------------------------------------------------------------------
	// Input
	// -------------------------------------------------------------------------

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == wComboSwitchCamera)
		{
			OnSwitchCamera();
			return true;
		}

		return true;
	}
}
