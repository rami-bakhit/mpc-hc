# Changelog

All notable changes to MPC-Kelpie are documented in this file. Changes made
in upstream MPC-HC are not listed here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).
Release tags are named `kelpie-<upstream version>-<n>`. Each entry includes
the date the change was first made.

## [Unreleased]

Based on [clsid2/mpc-hc](https://github.com/clsid2/mpc-hc) `develop` at
`d3c043624` (2026-10-01).

### Added

- Color Controls toolbar button with its own icon (2026-08-16).
- `MouseDragPanVideo` option to pan the video vertically with the left mouse
  button in fullscreen (2026-08-24).
- `MouseDragSubtitles` option to move the subtitles vertically with the left
  mouse button in fullscreen (2026-08-25).
- `RememberFileViewSettings` option to remember the vertical image position,
  subtitle position and subtitle size for each file (2026-08-25).
- `RememberFileColorSettings` option to remember brightness, contrast, hue and
  saturation for each file. The Color Controls dialog is updated when another
  file is opened (2026-08-28).
- `InheritSettingsFromFolder` option: a file without saved settings starts
  with the settings of the most recently played file from the same folder
  (2026-08-28).
- "Load Subtitles..." and "Download Subtitles..." in the menu of the subtitles
  toolbar button (2026-09-24).
- `KeepSubtitlePosAndSize` option to keep the subtitle position and size when
  other subtitles are selected, loaded or downloaded for the same file
  (2026-09-24).
- The Options dialog opens at the position where it was last closed
  (2026-09-24).

New options are located in Options > Advanced and are disabled by default.

### Changed

- The program icon is replaced with the MPC-Kelpie icon (2026-10-02).
- The MPC-Kelpie logo is the default logo shown when no file is open. The
  upstream logos remain available in Options > Player > Logo. Without a
  remembered window size, the player starts with a 960x540 video area, the
  size of the logo (2026-10-02).
- The window title shows "MPC-Kelpie" instead of "Media Player Classic Home
  Cinema" when no file is open (2026-10-02).
- The executable is renamed to `mpc-kelpie64.exe`, or `mpc-kelpie.exe` for
  32-bit builds (2026-10-02).
- MPC-Kelpie uses its own settings registry key, AppData folder, window class
  and file association names, so it no longer shares them with MPC-HC.
  Settings are not imported from MPC-HC (2026-10-03).
