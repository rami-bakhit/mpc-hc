# Contributing to MPC-Kelpie

MPC-Kelpie is a fork of [MPC-HC](https://github.com/clsid2/mpc-hc). Fixes that
are not specific to MPC-Kelpie are best submitted to MPC-HC; they reach this
repository with the next upstream merge.

## Pull Requests

1. Make sure you have a [GitHub account](https://github.com/signup/free).
2. [Fork](https://github.com/rami-bakhit/mpc-hc/fork) this repository.
3. Create a new topic branch (based on the `main` branch) to contain your feature, change, or fix.
4. **Set `core.autocrlf` to true: `git config core.autocrlf true`.**
5. [Open a Pull Request](https://github.com/rami-bakhit/mpc-hc/pulls) against `main` with a clear title and description.

### General development guidelines

1. Keep your patches clean, without unrelated changes. One feature or fix per branch and pull request.
2. Follow the code style of the surrounding MPC-HC code, so that a change can also be offered upstream.
3. New options are disabled by default.
4. When the resource files change, run [sync.bat](../src/mpc-hc/mpcresources/sync.bat)
   (requires Python, see Part F of [Compilation.md](../docs/Compilation.md)) and commit the updated
   translation files.
5. Don't change third-party code or submodules.
6. Add an entry to [CHANGELOG.md](../CHANGELOG.md) for each change in behavior.

## Reporting issues

See [Reporting issues](README.md#reporting-issues) in the README.
