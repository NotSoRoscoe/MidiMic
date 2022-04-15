#include "NoteSmooth.hh"
#include <bits/stdc++.h>
#include "KickSort.h"

#define NUM_MEM_NOTES 5

// notes are in an integer midi value form
int memNotes[NUM_MEM_NOTES];
int memIndex = 0;
bool memEmpty = true;
bool available = false;

void addNote(int note) {
  // reset array on the first note that = -1
  if (note == -1 && !memEmpty) {
    clearNotes();
  } else if (note != -1) {
    
    // replace older values when array is filled
    if (memIndex > NUM_MEM_NOTES - 1){
      memIndex = 0;  
    }
    memNotes[memIndex] = note;
    memIndex++;
    memEmpty = false;    
  }
}

void clearNotes() {
  for (int i = 0; i < NUM_MEM_NOTES; i++) {
    memNotes[i] = -1;
  }
  memEmpty = true;
}

int smoothedNote(int lastNote){
    KickSort<int>::quickSort(memNotes, NUM_MEM_NOTES, KickSort_Dir::ASCENDING);
    int max = memNotes[NUM_MEM_NOTES - 1];
 
    int t = max + 1;
    int count[t];
    for (int i = 0; i < t; i++)
        count[i] = 0;
 

    for (int i = 0; i < NUM_MEM_NOTES; i++) {
      if (memNotes[i] == -1) {
        //ignore
      } else {
        count[memNotes[i]]++;
      }
    }
 
    // mode is the index with maximum count
    int mode = 0;
    int k = count[0];
    for (int i = 1; i < t; i++) {
        if (count[i] > k) {
            k = count[i];
            mode = i;
        }
    }
    
    if (mode == 0)
      mode = lastNote;
      
    return mode;
}
