import winsound
import time
from midiutil.MidiFile import MIDIFile

noteLength = 250

def frequency(halfNotesFromA4):
    value = 440 * ((2)**(1.0/12.0))**halfNotesFromA4
    return (int)(value)

def processNote(note):
    if(note == 'C'):return frequency(3)
    if(note == 'D'):return frequency(5)
    if(note == 'E'):return frequency(7)
    if(note == 'F'):return frequency(8)
    if(note == 'G'):return frequency(10)
    if(note == 'A'):return frequency(12)
    if(note == 'B'):return frequency(14)

def beepSong(song):
        noteList = list(song)
        for note in noteList:
            print("Doing", note)
            winsound.Beep(processNote(note), noteLength)

def renderSong(song):
    MyMIDI = MIDIFile(1)
    track = 0
    time = 0
    MyMIDI.addTrackName(track, time, "Sample Track")
    MyMIDI.addTempo(track, time, 120)
    channel = 0
    volume = 100
    noteList = list(song)
    for note in noteList:
        pitch = 60
        #pitch = processNote(note)
        #duration = 1*(noteLength/1000)
        duration = 1
        MyMIDI.addNote(track, channel, pitch, time, duration, volume)
    binfile = open(song + ".mid", 'wb')
    MyMIDI.writeFile(binfile)
    binfile.close()

song = "EDCDEEE"

renderSong(song)
print("Should be done")
