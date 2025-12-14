# Migration from AppVeyor to GitHub Actions

This document describes the migration from AppVeyor CI to GitHub Actions.

## Overview

The project has migrated from AppVeyor to GitHub Actions for continuous integration. The new CI system provides:

- **Multi-platform support**: Windows, Linux (Ubuntu), and macOS
- **Multiple compilers**: MSVC, GCC, and Clang
- **Multiple configurations**: Release and Debug builds
- **Shared and static library builds**
- **Automatic testing** on all platforms
- **Build artifacts** for releases

## New Workflows

### 1. Windows CI (`.github/workflows/windows.yml`)
- **Runner**: Windows Server 2019
- **Compiler**: Visual Studio 2019 (MSVC)
- **Matrix builds**:
  - Release / Shared libraries
  - Debug / Static libraries
- **Dependencies**: Xerces-C installed via vcpkg
- **Artifacts**: Windows release builds packaged as zip files

### 2. Linux CI (`.github/workflows/linux.yml`)
- **Runner**: Ubuntu Latest
- **Compilers**: GCC and Clang
- **Matrix builds**:
  - GCC Release / Shared
  - GCC Debug / Static
  - Clang Release / Shared
- **Dependencies**: Xerces-C installed via apt
- **Artifacts**: Linux release builds packaged as tar.gz

### 3. macOS CI (`.github/workflows/macos.yml`)
- **Runner**: macOS Latest
- **Compiler**: Apple Clang
- **Matrix builds**:
  - Release / Shared libraries
  - Debug / Static libraries
- **Dependencies**: Xerces-C installed via Homebrew
- **Artifacts**: macOS release builds packaged as tar.gz

### 4. Combined CI Status (`.github/workflows/ci.yml`)
- Provides a single status check for branch protection rules

## Key Differences from AppVeyor

| Feature | AppVeyor | GitHub Actions |
|---------|----------|----------------|
| Platforms | Windows only | Windows, Linux, macOS |
| Compilers | MSVC, MinGW, Cygwin | MSVC, GCC, Clang |
| Dependency Management | vcpkg, manual builds | vcpkg (Windows), apt (Linux), Homebrew (macOS) |
| Caching | Manual cache configuration | Automatic with actions |
| Artifacts | Manual zip creation | Built-in artifact upload |
| Cost | Free for open source | Free for public repos |

## What Was Removed

The following AppVeyor-specific files are no longer needed:
- `.appveyor.yml` - AppVeyor configuration
- `scripts/ci-appveyor-setup` - AppVeyor setup script

These can be safely removed from the repository.

## Status Badges

The README.md now includes GitHub Actions status badges:

```markdown
[![Windows CI](https://github.com/djberg96/xalan-c/actions/workflows/windows.yml/badge.svg)](https://github.com/djberg96/xalan-c/actions/workflows/windows.yml)
[![Linux CI](https://github.com/djberg96/xalan-c/actions/workflows/linux.yml/badge.svg)](https://github.com/djberg96/xalan-c/actions/workflows/linux.yml)
[![macOS CI](https://github.com/djberg96/xalan-c/actions/workflows/macos.yml/badge.svg)](https://github.com/djberg96/xalan-c/actions/workflows/macos.yml)
```

## Triggering Workflows

Workflows are triggered by:
- Push to `master` or `main` branch
- Pull requests to `master` or `main` branch
- Manual trigger via GitHub Actions UI (`workflow_dispatch`)

## Viewing Results

1. Go to the repository on GitHub
2. Click the "Actions" tab
3. Select a workflow to see its runs
4. Click on a specific run to see detailed logs

## Customization

To customize the workflows:

1. Edit the `.github/workflows/*.yml` files
2. Common customizations:
   - Add/remove compiler versions in the matrix
   - Modify CMake configuration options
   - Add additional test commands
   - Change artifact packaging

## Testing Locally

To test changes before pushing:

```bash
# Install act (GitHub Actions local runner)
brew install act  # macOS
# or
sudo apt install act  # Linux

# Run a workflow locally
act -W .github/workflows/linux.yml
```

## Migration Checklist

- [x] Create Windows CI workflow
- [x] Create Linux CI workflow
- [x] Create macOS CI workflow
- [x] Add status badges to README
- [ ] Test workflows on GitHub
- [ ] Remove `.appveyor.yml`
- [ ] Remove `scripts/ci-appveyor-setup`
- [ ] Update documentation references to CI
