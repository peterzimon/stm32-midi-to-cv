#include <stdio.h>
#include "poly.h"

Poly::Poly(void) {
    _polyMode = POLYMODE_DEFAULT;
    _polyFirstVoice = 0;
    _polyLastVoice = VOICES;
    reset();
}

void Poly::reset(void) {
    for (int i = 0; i < VOICES; i++) {
        _notes[i] = _latchNotes[i] = _cvs[i] = 0;
        _lru[i] = -1;
    }
}

void Poly::setMode(poly_mode polyMode) {
    _polyMode = polyMode;
    switch (_polyMode)
    {
    case POLYMODE_DEFAULT:
        _polyFirstVoice = 0;
        _polyLastVoice = VOICES;
        break;
    case POLYMODE_BCH:
        _polyFirstVoice = 1;
        _polyLastVoice = VOICES;
        break;
    case POLYMODE_CHL:
        _polyFirstVoice = 0;
        _polyLastVoice = VOICES - 1;
        break;
    }
}

void Poly::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    switch (_polyMode)
    {
    case POLYMODE_DEFAULT:
        break;
    case POLYMODE_BCH:
        if (note == _leadNote) {
            _leadNote = 0;
            _notes[0] = 0;
            return;
        }
        break;
    
    case POLYMODE_CHL:
        if (note == _leadNote) {
            _leadNote = 0;
            _notes[VOICES - 1] = 0;
            return;
        }
        break;
    }
    
    int voice = _findVoice(note);
    if (voice == -1) {
        return;
    }
    _notes[voice] = 0;
    int voiceIndex = _findVoiceLRUIndex(voice);
    _leftShiftLRU(voiceIndex + 1);
    _lru[VOICES - 1] = -1;
}

void Poly::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    switch (_polyMode)
    {
    case POLYMODE_DEFAULT:
        break;
    case POLYMODE_BCH:
        if (note < CHORD_LEAD_BORDER_NOTE) {
            _leadNote = note;
            _notes[0] = _latchNotes[0] = note;
            return;
        }
        break;
    
    case POLYMODE_CHL:
        if (note >= CHORD_LEAD_BORDER_NOTE) {
            _leadNote = note;
            _notes[VOICES - 1] = _latchNotes[VOICES - 1] = note;
            return;
        }
        break;
    }

    if (_findVoice(note) != -1) {
        return;
    }
    int voice = _findInactiveVoice();

    if (voice != -1) {
        _addVoiceToLRU(voice);
    } else {
        voice = _lru[0];
        _leftShiftLRU(1);
        _lru[_polyLastVoice - 1] = voice;
    }

    _notes[voice] = _latchNotes[voice] = note;
}

void Poly::getCVGate(uint16_t *cv, int *gate) {
    for (int voice = 0; voice < VOICES; voice++) {
        cv[voice] = cvForNote(_latchNotes[voice], voice);
        gate[voice] = _notes[voice] ? 1 : 0;
    }
}

int Poly::_findVoice(uint8_t note) {
    for (int i = _polyFirstVoice; i < _polyLastVoice; i++) {
        if (_notes[i] == note) return i;
    }
    return -1;
}

int Poly::_findVoiceLRUIndex(int voice) {
    if (voice < 0) return 0;
    for (int i = _polyFirstVoice; i < _polyLastVoice; i++) {
        if (_lru[i] == voice) return i;
    }
    return -1;
}

int Poly::_findInactiveVoice(void) {
    for (int i = _polyFirstVoice; i < _polyLastVoice; i++) {
        if (!_notes[i]) {
            return i;
        }
    }
    return -1;
}

void Poly::_addVoiceToLRU(int voice) {
    for (int i = _polyFirstVoice; i < _polyLastVoice; i++) {
        if (_lru[i] == -1) {
            _lru[i] = voice;
            return;
        }
    }
}

void Poly::_removeVoiceFromLRU(int voice) {
    for (int i = _polyFirstVoice; i < _polyLastVoice; i++) {
        if (_lru[i] == voice) {
            _lru[i] = -1;
            return;
        }
    }
}

void Poly::_leftShiftLRU(int fromIndex) {
    if (fromIndex == 0) return;
    for (int i = _polyFirstVoice; i < _polyLastVoice; i++) {
        if (i >= fromIndex) {
            _lru[i - 1] = _lru[i];
        }
    }
}

void Poly::debug() {
    return;

    printf("LRU\r\n");
    for (int i = 0; i < VOICES; i++) {
        printf("%d:", _lru[i]);
    }
    printf("\r\n");
}