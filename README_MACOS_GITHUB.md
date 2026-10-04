# SleepGuard — automatic Intel macOS DMG build

This workflow builds a native **Intel x86_64** macOS DMG on GitHub Actions.

## Runner

The workflow uses GitHub's native Intel runner:

```yaml
runs-on: macos-15-intel
```

## Build

1. Upload the whole project to GitHub, including `.github/workflows/build-macos-dmg.yml`.
2. Open **Actions**.
3. Select **Build SleepGuard macOS Intel DMG**.
4. Click **Run workflow**.
5. After a successful run, download the artifact:
   - `SleepGuard-macOS-Intel-x86_64`
6. Inside it is:
   - `SleepGuard-Intel-x86_64.dmg`

The workflow intentionally does **not** install the Homebrew `qt` meta-package and does **not** run `brew update`. It installs only `qtbase`, `qtconnectivity`, and `qttools`, then verifies that the resulting application is x86_64.
