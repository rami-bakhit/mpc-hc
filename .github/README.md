# MPC-Kelpie

MPC-Kelpie is a fork of [MPC-HC](https://github.com/clsid2/mpc-hc) with a few
additional features. It follows the upstream `develop` branch and is updated
regularly.

This is not an official MPC-HC build. Please report problems with MPC-Kelpie
in this repository, not to the MPC-HC project.

## Features

All options added by MPC-Kelpie are disabled by default. With these options
disabled, the player behaves the same as the upstream version it is based on.

- Color Controls button for the toolbar (Options > Player > Toolbar Layout).
  It has its own icon and does not use one of the four custom button slots.
- "Load Subtitles..." and "Download Subtitles..." in the menu of the subtitles
  toolbar button. The button is also enabled when the open video has no
  subtitles.
- The Options dialog opens at the position where it was last closed.
- MPC-Kelpie program icon.
- MPC-Kelpie logo in the player window when no file is open
  (Options > Player > Logo).

The following options are available in Options > Advanced:

| Option | Description |
|---|---|
| `MouseDragPanVideo` | Pan the video vertically by dragging with the left mouse button (fullscreen only). |
| `MouseDragSubtitles` | Move the subtitles vertically by dragging them with the left mouse button (fullscreen only). |
| `RememberFileViewSettings` | Remember the vertical image position, subtitle position and subtitle size for each file. |
| `RememberFileColorSettings` | Remember brightness, contrast, hue and saturation for each file. |
| `InheritSettingsFromFolder` | A file without saved settings starts with the settings of the most recently played file from the same folder. Requires `RememberFileViewSettings` or `RememberFileColorSettings`. |
| `KeepSubtitlePosAndSize` | Keep the current subtitle position and size when other subtitles are selected, loaded or downloaded for the same file. |

Per-file settings are stored as part of the recent files history.

See [CHANGELOG.md](../CHANGELOG.md) for a dated list of changes.

## Relationship to MPC-HC

MPC-Kelpie is based on the `develop` branch of
[clsid2/mpc-hc](https://github.com/clsid2/mpc-hc). Upstream changes are merged
regularly, and always after an upstream stable release. Each release states
the upstream commit it is based on.

Each feature is kept in its own commit, so it can be submitted upstream if the
MPC-HC maintainers are interested.

MPC-Kelpie is not affiliated with or endorsed by the MPC-HC project.

## Download

No binary releases have been published yet. Releases will be available on the
[Releases](https://github.com/rami-bakhit/mpc-hc/releases) page, together with
SHA-256 checksums and a complete source archive.

Builds are 64-bit (x64) only.

## Running alongside MPC-HC

MPC-HC stores its settings in the Windows registry under the same key for
every build, so MPC-Kelpie and an installed MPC-HC would overwrite each
other's settings. To keep them separate, run MPC-Kelpie in portable mode by
placing an `.ini` file with the same name as the executable in the same
folder, for example `mpc-hc64.ini` next to `mpc-hc64.exe`.

## Building

MPC-Kelpie is built the same way as MPC-HC. See
[docs/Compilation.md](../docs/Compilation.md) for instructions. Clone the
repository with submodules:

```
git clone --recursive https://github.com/rami-bakhit/mpc-hc.git
```

The default branch is `main`.

## Branches

| Branch | Purpose |
|---|---|
| `main` | MPC-Kelpie: upstream plus all fork features. Releases are tagged here. History is never rewritten. |
| `develop` | Unmodified mirror of upstream `develop`. |
| `feature/*` | One branch per feature. These branches may be rebased. |

Release tags are named `kelpie-<upstream version>-<n>`, for example
`kelpie-2.8.2-1`.

## Reporting issues

Open an issue in this repository and include:

- the MPC-Kelpie version (Help > About shows the version, commit and branch)
- your Windows version, GPU and video renderer
- steps to reproduce the problem

If the problem also occurs with the official MPC-HC build, it should be
reported to the MPC-HC project. A link to the upstream issue is welcome here
as well.

## License

MPC-Kelpie is a modified version of MPC-HC and is distributed under the same
license, the GNU General Public License version 3 (see
[COPYING.txt](../COPYING.txt)). Changes made in this fork are listed with
dates in [CHANGELOG.md](../CHANGELOG.md) and in the commit history.

Third-party components are subject to their own licenses, as in MPC-HC.

## Credits

MPC-HC is the work of its original authors, clsid2 and the contributors listed
in [docs/Authors.txt](../docs/Authors.txt). MPC-Kelpie is maintained by
[rami-bakhit](https://github.com/rami-bakhit).
