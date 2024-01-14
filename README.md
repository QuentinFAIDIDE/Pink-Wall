# Pink Wall
## Description
A dumb brickwall sandwitched between a pink-to-white and a white-to-pink filter (approximately 3db/Octave slope). 

## Genesis and purpose for music
A tight brickwall can be used on the master bus to introduce artefacts
that indicates a cut is to be made somewhere in the mix. The issue is that very often the low frequencies will saturate first due to the fact that music generally
mimics a pink noise frequency response and not a flat white one.
This plugin simply wraps a brickwall between two filters that turn the pink noise
shaped signal into a white noise shaped one and back, and therefore allow for the
artefacts to be more likely to be introduced by mid and higher frequencies.
Needless to mention that the plugin's destructive properties can also be used for other creative purposes.

## Build and install
Documentation work in progress.

## References
- Filtering coefficients for pinking filter are from [here](https://ccrma.stanford.edu/%7Ejos/sasp/Example_Synthesis_1_F_Noise.html).
- The root `CMakelists.txt` was inspired by [this project's cmake]() by Anthony Alfimov. 
- The library used to make plugins is JUCE, and this software leverages the GPL compatibility of the license.