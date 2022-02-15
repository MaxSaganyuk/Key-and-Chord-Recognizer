# Key-and-Chord-Recognizer
Musical utility for fast recognition of real time played chord name (and keys in which chord "fits") using MIDI-piano input and Qt for UI

## _How to use_

Connect your MIDI keyboard device to the PC via USB and start the program.

## _Program_

Detailed description can be viewed here (in Ukrainian language):
https://drive.google.com/file/d/1VfvGBDOpOXICgxRbyOOo0uHUoh8W_9RP/view?usp=sharing

Short version in English below:

This program can:
- Determine triad chords
- Determine inverted chords
- Determine extended (seventh, nineth, eleventh) chords
- Determine suspended and added tone chords
- Determine current all major and minor keys in which the chord fits

UI looks like this:

<img src="readMeImages\1.png"></img>

After F minor chord was played in the MIDI-keyboard - UI looks like this:

<img src="readMeImages\2.png"></img>

_Current chord_ section displays the name of the chord

<img src="readMeImages\3.png"></img>
<img src="readMeImages\4.png"></img>

The program can determine more complex chords like, for example, F minor seventh with sharped nineth

<img src="readMeImages\5.png"></img>

_Current key_ displays all major and minor keys in which current chord "fits" (for F minor)

<img src="readMeImages\6.png"></img>
<img src="readMeImages\7.png"></img>

You can play all notes in the key - the chord will not be detrmined by the key will

<img src="readMeImages\8.png"></img>

Chord names are usually analized through the context of the key, because the program attempts to help the user without knowing the exact key in which the chord is being played all "white piano keys" are assumed to be C D E F G A B and "black piano keys" are either sharps or flats (no situations where we assume the C key as B#, for example). 

If you are more aware about the key/tonality you can switch between sharps and flats.

<img src="readMeImages\9.png"></img>

<img src="readMeImages\10.png"></img>

<img src="readMeImages\11.png"></img>

Gif that demonstrates real time chord and key recognition

## _The downsides of the project_
- Determines one chord at a time
- Does not determine "wide" chords
- Ineffective assumptions of sharps and flats

