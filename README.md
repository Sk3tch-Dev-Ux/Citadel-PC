# Citadel PC

A DayZ mod that adds a fully functional PC desk system with cryptocurrency mining, CCTV surveillance, 3D printing, and crafting mechanics.

## Features

- **PC Desk System** -- Deployable desk with monitor, keyboard, mouse, WiFi router, and full PC internals (motherboard, GPUs, RAM, PSU, cooler, fans, SSDs, hard drives)
- **Cryptocurrency Mining** -- Mine Bitcoin (and extensible to Ethereum, XRP, Solana) using installed GPUs. Higher-tier GPUs yield more coins per tick but wear out over time
- **GPU Tiers** -- Four graphics cards with increasing performance and longevity:
  - GTX 660 (2 MH/s, 2h lifetime)
  - GTX 1080 Ti (10 MH/s, 4h lifetime)
  - RTX 2080 Ti (18 MH/s, 6h lifetime)
  - RTX 4090 Ti (30 MH/s, 8h lifetime)
- **Coin Withdrawal** -- Withdraw mined coins as physical in-game items to your inventory
- **CCTV Surveillance** -- Deploy cameras and view feeds from your PC desk's monitor interface
- **3D Printer** -- Place and use a 3D printer with filament holder and color filaments
- **GPU Repair** -- Craft repair kits to restore worn-out graphics cards
- **Desk Construction** -- Unpack desk boxes, deploy holograms, and assemble/dismantle desks with a screwdriver
- **Mining Chassis** -- Standalone chassis supporting up to 6 GPUs for dedicated mining rigs
- **Power Management** -- Desks require power (plugged in via energy manager) to boot and operate

## Installation

1. Download or clone this repository
2. Copy the following folders into your DayZ server's mod directory:
   - `Citadel_PC/`
   - `Citadel_PC_Core/`
   - `Citadel_PC_Crafting/`
   - `Basecameras/`
3. Add all four mods to your server's `-mod=` launch parameter:
   ```
   -mod=@Citadel_PC;@Citadel_PC_Core;@Citadel_PC_Crafting;@Basecameras
   ```
4. Add the item classnames to your server's `types.xml` to enable spawning (see Item Classes below)
5. Restart your server

### Dependencies

- [Community Framework (CF)](https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036) -- Required for module registration, RPC, and logging

## Mod Structure

```
Citadel-PC/
├── Basecameras/           # CCTV camera system and laptop menu UI
├── Citadel_PC/            # Items, models, textures, entity scripts, recipes
├── Citadel_PC_Core/       # Core logic: mining, actions, settings, logging
└── Citadel_PC_Crafting/   # GPU repair recipes and repair kit item
```

## Configuration

On first server start, a settings file is auto-generated at:

```
<DayZ Server Profile>/Citadel_PC/Config/CitadelPcSettings.json
```

### Mining Settings

| Setting | Default | Description |
|---------|---------|-------------|
| `MINING_TICK_INTERVAL` | 60s | How often mining payouts and GPU wear are calculated |
| GPU lifetimes | 2h--8h | Seconds of continuous mining before a GPU is destroyed |
| Coin yields | 0.05--0.25 | Coins earned per GPU per tick |
| Hash rates | 2--30 MH/s | Informational rate displayed in the UI |

### Logging Settings

Generated at `<Profile>/Citadel_PC/Logging/Config/LoggingSettings.json`:

| Setting | Default | Description |
|---------|---------|-------------|
| `logLevel` | 1 (Info) | 0 = Debug, 1 = Info, 2 = Warn, 3 = Error |
| `refreshRateInSeconds` | 60 | How often the server re-reads logging config |

Logs are written to `<Profile>/Citadel_PC/Logging/Logs/`.

## Item Classes

### Core Desk & Peripherals
- `Citadel_Desk` -- Assembled PC desk
- `Citadel_Desk_Box` -- Boxed desk kit (unpack to get parts)
- `Citadel_Desk_Manual` -- Desk deployment manual
- `Citadel_Monitor`, `Citadel_Keyboard`, `Citadel_Mouse`
- `Citadel_MousePad`, `Citadel_MousePad_Rolled`, `Citadel_MousePad_Case`
- `Citadel_WiFi_Router`
- `Citadel_Powerstrip` -- Power cable reel

### PC Internals
- `Citadel_Mother_Board`
- `Citadel_Graphics_Card_660`, `Citadel_Graphics_Card_1080TI`, `Citadel_RTX_2080TI`, `Citadel_Graphics_Card_4090TI`
- `Citadel_Kingston` -- SSD storage
- `Citadel_PSU` -- Power supply
- `Citadel_Hard_Drive`, `Citadel_SSD`
- `Citadel_Cooler`, `Citadel_Fan`
- `Citadel_GSKILL_TridentZ`, `Citadel_Corsair_Vengeance`, `Citadel_Corsair_Platinum` -- RAM sticks
- `Citadel_Back_Cover`

### Cryptocurrency
- `Citadel_Bitcoin`, `Citadel_Ethereum`, `Citadel_XRP`, `Citadel_Solana`

### Mining
- `Citadel_Mining_Chassis` -- Standalone mining rig chassis

### CCTV
- `Citadel_CCTV` -- Surveillance camera
- `Citadel_CCTV_Kit` -- Deployable camera kit

### 3D Printing
- `Citadel_3D_Printer`
- `Citadel_Filament_Holder`, `Citadel_Filament_Holder_Kit`
- `Citadel_Filament_Red`, `Citadel_Filament_Green`, `Citadel_Filament_Yellow`, `Citadel_Filament_Blue`

### USB Drives
- `Citadel_Tier1_USB` through `Citadel_Tier5_USB`

### Crafting
- `Citadel_GpuRepairKit` -- Used with damaged GPUs to craft a fully repaired card

## Credits

- Original mod by **TheDmitri** and **Genesis**
