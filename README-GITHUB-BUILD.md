# Riow Visualizer — build in the cloud

This project includes the C++ JUCE source and a GitHub Actions workflow intended to build a Windows x64 VST3 without installing Visual Studio on your PC.

## Steps
1. Create a new GitHub repository named `RiowVisualizer`.
2. Upload all files and folders from this project to the repository root, including the hidden `.github/workflows/build.yml` file.
3. Open the repository's **Actions** tab and select **Build Riow Visualizer VST3**.
4. Click **Run workflow** if it has not started automatically.
5. If the run succeeds, open it and download the artifact named `RiowVisualizer-VST3-Windows`.
6. Extract the ZIP and copy the `RiowVisualizer.vst3` bundle into `C:\Program Files\Common Files\VST3\`. Restart the DAW and rescan plugins.

## Important
The first build may fail if the source needs code adjustments for the selected JUCE/compiler version. If it fails, share the error log. Do not assume a working plugin was generated until the workflow succeeds.

This is a prototype. It does not export MP4 video yet, and automatic BPM/key detection is not implemented.
