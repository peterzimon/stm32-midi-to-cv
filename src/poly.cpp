#include <stdio.h>
#include "poly.h"

Poly::Poly(void) {
    for (int i = 0; i < VOICES; i++) {
        _notes[i] = 0;
        _cvs[i] = 0;
        _lru[i] = -1;
    }
}

void Poly::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
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
    if (_findVoice(note) != -1) {
        return;
    }
    int voice = _findInactiveVoice();

    if (voice != -1) {
        _addVoiceToLRU(voice);
    } else {
        voice = _lru[0];
        _leftShiftLRU(1);
        _lru[VOICES - 1] = voice;
    }

    _notes[voice] = note;
}

void Poly::getCVGate(uint16_t *cv, int *gate) {
    for (int i = 0; i < VOICES; i++) {
        cv[i] = cvForNote(_notes[i]);
        gate[i] = _notes[i] ? 1 : 0;
    }
}

int Poly::_findVoice(uint8_t note) {
    for (int i = 0; i < VOICES; i++) {
        if (_notes[i] == note) return i;
    }
    return -1;
}

int Poly::_findVoiceLRUIndex(int voice) {
    if (voice < 0) return 0;
    for (int i = 0; i < VOICES; i++) {
        if (_lru[i] == voice) return i;
    }
    return -1;
}

int Poly::_findInactiveVoice(void) {
    for (int i = 0; i < VOICES; i++) {
        if (!_notes[i]) {
            return i;
        }
    }
    return -1;
}

void Poly::_addVoiceToLRU(int voice) {
    for (int i = 0; i < VOICES; i++) {
        if (_lru[i] == -1) {
            _lru[i] = voice;
            return;
        }
    }
}

void Poly::_removeVoiceFromLRU(int voice) {
    for (int i = 0; i < VOICES; i++) {
        if (_lru[i] == voice) {
            _lru[i] = -1;
            return;
        }
    }
}

void Poly::_leftShiftLRU(int fromIndex) {
    if (fromIndex == 0) return;
    for (int i = 0; i < VOICES; i++) {
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