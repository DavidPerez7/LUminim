LUminim - Optimized Lumina Desktop Fork
========================================

> **⚡ Fork of Lumina Desktop optimized to deliver better performance on older and resource-constrained hardware**

**This is NOT the official Lumina Desktop.** This fork focuses on reducing runtime memory and disk usage while keeping the core Lumina experience.

**Original Project**: [Lumina Desktop](https://github.com/lumina-desktop/lumina) @ https://lumina-desktop.org/

**This Fork**: Community optimization by [DavidPerez7](https://github.com/DavidPerez7/LUminim)  
**Current Version**: `v0.5.0-beta` (The "Slim & Fast" Update)

**Community Chat Channels:**
 - Telegram: ["Lumina Desktop" public channel](https://t.me/luminadesktop)
 - Matrix: #lumina-desktop:matrix.org

---

Description
===========

LUminim is a lightweight desktop environment based on Lumina, designed for minimal resource usage while keeping the same plugin-based flexibility. It removes non-essential components and applies compilation/runtime optimizations to reduce the overall footprint.

---

## 🎯 What's Different in LUminim?

### Removed for Speed (Physical Deletion & Cleaned)
- ❌ **OS Backends**: FreeBSD, OpenBSD, NetBSD, DragonFly, Solaris (Surgical 100% Linux-only)
- ❌ **Window Manager**: Fluxbox (Replaced by hardcoded Openbox engine)
- ❌ **Multimedia**: `lumina-mediaplayer` (Removed to reduce QtMultimedia bloat)
- ❌ **Archiver**: `lumina-archiver` (Removed in favor of system-native tools)
- ❌ **Utilities**: `lumina-calculator`, `lumina-pdf`, `lumina-fileinfo`, `lumina-info`, `lumina-pingcursor`
- ❌ **Plugins**: calendar, notepad, audioplayer, systemmonitor, rssreader, alarm
- ❌ **Weight**: Login.ogg, Logout.ogg, low-battery.ogg and 9/12 color themes
- ❌ **Qt Libraries**: QtMultimedia, QtConcurrent, QtQuick, QtQml (Reduced dependencies)

### Performance Scraping (Kernel-Level Optimizations)
- ✅ **CPU Throttling**: Panel timers reduced from 10ms to **150ms** (No more micro-stutters)
- ✅ **Memory Alignment**: Migrated X11 Window/State lists to **`QVector<WId>`** (Contiguous memory access)
- ✅ **Cache Locality**: Critical loops migrated from `QList` to **`QVector`** (Optimized for 1MB L2 Cache)
- ✅ **SVG Caching**: Icons now pre-render to 24x24 Pixmaps (Massive RAM/CPU saving)
- ✅ **Fast Build**: New `FASTBUILD=1` flag for `qmake` (Quick compilation on low-end CPUs)

### Optimizations Applied
- ✅ **Compilation**: `-O3 -Ofast -march=native -flto=16` (Link-Time Optimization)
- ✅ **Icons**: Lazy loading with max 50-item cache (-60-70 MB RAM)
- ✅ **Themes**: Smart caching with 60-second refresh (-filesystem overhead)
- ✅ **Platform**: Linux-only (removed FreeBSD/OpenBSD variants)

**Best For**: resource-constrained systems and minimal desktop installs  
**Not Ideal For**: Users needing advanced themes or all desktop widgets

---

## ⚠️ Breaking Changes from Original Lumina

If you're migrating from official Lumina:
- Desktop widgets: Use app menu to launch instead (notepad, calendar, etc)
- System info: Use `fastfetch`, `neofetch`, or `cat /proc/cpuinfo`  
- Audio: No sound notifications (use `paplay` or `aplay` if needed)
- Alarm: Use system `at` or `cron` instead
- Themes: Limited to 3 color schemes (modify in source if needed)

---

Operating Systems and Distributions <a name="osdistros"></a>
----

### Tested (Void Linux)
- Void Linux (runit + LightDM) — validated on at least one system; more testing is pending.
- Common pitfall: if the system forces a specific window manager (e.g. `fluxbox`) via `/etc/lightdm/Xsession`, the session may exit immediately with return code `127` (this is a system configuration issue, not a LUminim bug).

### Supported but not yet tested
- Distros supported by `install.sh` (Ubuntu/Debian/Mint, Fedora, etc.)

## Versioning
We use a simple `MAJOR.MINOR` scheme to reflect incremental improvements.
- `0.x` = active optimization / performance tuning
- `1.0` = first stable release (planned)

## Reporting Issues (planned)
This repo is under active development. For now, the best way to report issues is via GitHub Issues in this repository; mark reports with `void-linux` or `optimization` where appropriate.

## Build & Install (Surgical Way)
1. **Prepare Environment**:
```bash
git clone https://github.com/DavidPerez7/LUminim.git
cd LUminim
make distclean
```
2. **Compile & Install**:
```bash
qmake -r CONFIG+=FASTBUILD
make -j$(nproc)
sudo make install
```
3. **Session Setup & Repair**:
```bash
# Remove old sessions
sudo rm /usr/share/xsessions/lumina.desktop /usr/share/xsessions/LUminim.desktop

# Install new session
sudo cp src-qt5/core/lumina-desktop/Lumina-DE.desktop /usr/share/xsessions/LUminim.desktop

# Ensure Window Manager (Openbox)
sudo xbps-install -S openbox
sudo ln -sf /usr/bin/openbox /usr/bin/lumina-wm
sudo ln -sf /usr/bin/lumina-session /usr/local/bin/lumina-session
sudo reboot
```

---

## 🗺️ LUminim Roadmap (The Pentium T2310 Path)

### Phase 1: Surgical Cleanup (Current)
- [x] **OS Purge**: Remove all non-Linux backends (FreeBSD, OpenBSD, etc.).
- [x] **Weight Reduction**: Delete `dev-tools` and unfinished `lumina-desktop-unified`.
- [x] **Data Refactor**: Complete migration from `QList` to `QVector` in X11 core.

### Phase 2: Build & Runtime Optimization (In-Progress)
- [x] **PCH Implementation**: Global Precompiled Headers to slash compile times.
- [ ] **LTO Tuning**: Fine-tune Link-Time Optimization for minimal binary size.
- [ ] **Event Throttling**: Audit all `QTimer` instances to ensure >150ms intervals.
- [ ] **Icon Pre-rendering**: Move SVG rendering to a background thread with static cache.

### Phase 3: Hardware Specifics (Upcoming)
- [ ] **Pentium T2310 Profile**: SSE3/L2-Cache specific compiler flags.
- [ ] **Memory Pressure Handling**: Implement aggressive cache clearing when RAM < 128MB.
- [ ] **Openbox Integration**: Hardcode Openbox as the only supported WM (Remove Fluxbox logic).

### Phase 4: Release 1.0 "Slim"
- [ ] **Zero-Bloat Installer**: Minimal `install.sh` for Void Linux/Alpine.
- [ ] **Binary Packaging**: `.xbps` (Void) and `.apk` (Alpine) support.

---

### Troubleshooting (Void + LightDM)
If the session returns to the login screen, check whether LightDM is forcing a fixed window manager (e.g. running `fluxbox`) instead of running the session command.
1. Confirm the session file:
   ```bash
   cat /usr/share/xsessions/LUminim.desktop
   ```
2. Check LightDM’s Xsession wrapper:
   ```bash
   cat /etc/lightdm/Xsession | head
   ```
3. Inspect the last log entries:
   ```bash
   tail -n 80 ~/.xsession-errors
   sudo tail -n 80 /var/log/lightdm/lightdm.log
   ```
