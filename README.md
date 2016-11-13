# Acknowledgements
* Inspiration by http://chorder.cs.vassar.edu/spiffchorder%3Aforside.
  * Which is also the source of the nasa.h keyboard layout.
  * First enamoured with the keyboard by watching this interview https://www.youtube.com/watch?feature=player_detailpage&v=7kATKI34jvs#t=458
* This version is adapted from https://github.com/buzztiaan/Teenspiffchord

# Features
* Lazy keypress.
  * Chords are triggered only on keypress and then only when the numbers of keys goes up.
  * eg, pressing the pinky key and ring finger key together and then releasing the ring finger key and pressing it again will send 2 "u" keysyms.

# Hardware
* This uses the internal pullups for the buttons, and a the Bounce (v1) library to detect keypresses.
* The buttons are wired directly to the teensy.
* Hardware designs from buzztiaan have been left in place but I have not tested or used them.
