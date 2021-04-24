/**
 * Implementation of CVGate interface for mono polyphonic mode.
 * 
 * Last note priority logic
 * ------------------------
 * 
 * If all voices are used (at least 4 notes are played) then with each 
 * new note, the least recently used has to be replaced. 
 * 
 * Requires two buffers:
 * LRU - least recently used voices. Contains the voice/output index (0-3)
 *       in the order of lowest index being the least recently used and
 *       highest index being the most recently used. E.g. [3,0,2,1]
 * AN  - active notes. MIDI note number of incoming notes.
 * 
 * When the LRU is not full then add the new voice to the next empty slot.
 * If the LRU is full then find the new voice in the LRU and (1) shift all
 * voices that came in later than the new voice and put the new voice to 
 * the end of the LRU.
 * 
 * Example:
 * 1. Init state, the LRU and AN are empty. 
 *      LRU = [-1,-1,-1,-1]
 *      AN = [0,0,0,0]
 * 
 * 2. Let's assume a couple of notes come in and the LRU/AN start to get 
 *    filled. 
 *          LRU = [0,1,2,-1]
 *          AN = [60,62,64,-1]
 * 
 * 3. At this point, a fourth note comes in and the LRU gets full
 *          LRU = [0,1,2,3]
 *          AN = [60,62,64,66]
 * 
 * 4. When a next note comes in (remember no noteOffs up until now), the logic
 *    in the noteOn function gets the least recently used voice from the LRU.
 *    That will be the voice of the 5th (new) note. In this case the LRU
 *    needs to be updated so that the updated voice gets to the end of the
 *    LRU and all the others get left shifted by 1. Also the note for the 
 *    old voice has to be replaced with the new note.
 * 
 *    Before:
 *          LRU = [0,1,2,3]
 *          AN = [60,62,64,66]
 *    
 *    • Least recently used voice LRU[0] = 0
 *    • Note to be replaced = AN[LRU[0]] = 60
 *    • New least recently used voice / note = 1 / 62
 * 
 *    After:
 *          LRU = [1,2,3,0]
 *          AN = [70,62,64,66]
 * 
 * 5. This way if a next (6th) note comes in it's going to take the first 
 *    voice from the LRU and do the same as in step 4.

 *    Before:
 *          LRU = [1,2,3,0]
 *          AN = [70,62,64,66]
 * 
 *    After:
 *          LRU = [2,3,0,1]
 *          AN = [70,72,64,66]
 * 
 * 6. Let's assume that a noteOff message is received. The logic in the noteOff
 *    function finds if this note is actually played and if so which voice
 *    is playing it. Let's say it's the note 64 playing through voice #2. In this 
 *    case the note has to be removed from the active notes array and the associated
 *    voice has to be removed from the LRU and all the voices with higher LRU
 *    index has to be left shifted by 1.
 *  
 *    Before:
 *          LRU = [2,3,0,1]
 *          AN = [70,72,64,66]
 *
 *    After:
 *          LRU = [3,0,1,-1]
 *          AN = [70,72,0,66]
*/

#ifndef POLY_H
#define POLY_H

#include <inttypes.h>
#include "cvgate.h"
#include "settings.h"

enum poly_mode {
    POLYMODE_DEFAULT,
    POLYMODE_BCH, // Bass + chords
    POLYMODE_CHL // Chords + lead
};

class Poly: public CVGate {
    public:
        Poly();

        void setMode(poly_mode polyMode);
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void getCVGate(uint16_t *cv, int *gate);
        void debug();

    private:
        poly_mode _polyMode;
        uint8_t _notes[VOICES];
        uint16_t _cvs[VOICES];
        int _lru[VOICES];

        int _findVoice(uint8_t note);
        int _findVoiceLRUIndex(int voice);
        int _findInactiveVoice(void);
        void _addVoiceToLRU(int voice);
        void _removeVoiceFromLRU(int voice);
        void _leftShiftLRU(int fromIndex);
};

#endif