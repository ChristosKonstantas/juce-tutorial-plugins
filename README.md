## Instructions

1. Clone https://github.com/juce-framework/JUCE
   
   ```
   git clone https://github.com/juce-framework/JUCE.git
   ```

3. Execute Projucer.exe

    2.1. For Windows execute
   
   ```
   .\path\to\juce-framework\extras\Projucer\Builds\VisualStudio2022\x64/Debug\App\Projucer.exe
   ```
   
    2.2. For Linux execute
   
   ```
   cd /path/to/juce-framework/extras/Projucer/Builds/LinuxMakefile/
   make
   ./build/Projucer
   ```
   
5. Open with [Projucer](https://docs.juce.com/master/tutorial_new_projucer_project.html), project.jucer file. Save and open with Visual Studio Code version that you use.
    
## Projects

### HelloWorldWin
A simple hello world window using JUCE following [PFM Starting From Scratch](https://www.youtube.com/watch?v=JHTcLVOcnQ4&t=0s).

### SimpleEQ
A simple equalizer plugin (in progress).

### WavetableSynth
A simple wavetable synthesizer from [WolfSound](https://thewolfsound.com/sound-synthesis/wavetable-synth-plugin-in-juce/).

### XY_Pad
A simple XY Pad with a draggable thumb, a gain slider for volume control, and a panner slider for stereo balance.
