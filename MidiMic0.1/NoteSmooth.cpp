#include "NoteSmooth.hh"

#define NUM_MEM_NOTES 10

// notes are in an integer midi value form
int memNotes[NUM_MEM_NOTES];
int index = 0;
bool memEmpty = true;
bool available = false;

void addNote(int note) {
  // reset array on the first note that = -1
  if (note == -1 && !memEmpty) {
    clearNotes();
  } else if (note != -1) {
    
    // replace older values when array is filled
    if (index > NUM_MEM_NOTES - 1){
      index = 0;  
    }
    memNotes[index] = note;
    index++;
    memEmpty = false;    
  }
}

void clearNotes() {
  for (int i = 0; i < NUM_MEM_NOTES; i++) {
    memNotes[i] = -1;
  }
  memEmpty = true;
}

int smoothedNote(){
  int sum = 0;
  for (int i = 0; i < NUM_MEM_NOTES; i++) {
    if (memNotes[i] != -1) {
      sum += memNotes[i];
    }
  }
  return sum/NUM_MEM_NOTES;
}
