#ifndef State_h
#define State_h

#include <Arduino.h>


#include "Constants.h"

class State
{
    public:
        // true if has note
        bool hasNote(){
            return note;
        } 
        // false - red 
        // true - blue
        bool getAlliance(){
            return blue;
        }

        bool isEnabled(){
            return enabled;
        }

        // true if has note
        void setNote(bool hasNote){
            note = hasNote;
        }
        // false - red 
        // true - blue
        void setAlliance(bool isBlue){
            blue = isBlue;
        }

        void setEnable(bool Enabled){
            enabled = Enabled;
        }

    private:


        bool enabled;
        bool note;
        bool blue;

        uint8_t nextShot;


};

#endif