//
// Created by Keyris on 08.09.2020.
//
#include <vector>
#include <windows.h>

#ifndef TEST_SPRITE_H
#define TEST_SPRITE_H

using namespace std;

class Sprite {

private:

    int rightTopX ;
    int rightTopY ;
    int width, height ;

    double scale ;
    int speed ;

    UINT_PTR timers[4] = { 0, 0, 0, 0}  ;

    const int timerTime = 1;


public:
    Sprite( double  scale, int speed );

    void recalculateCoordinates(int windowWidth, int windowHeight, int windowCenterX, int windowCenterY);

    int getRightTopX() const;

    void setRightTopX(int rightTopX);

    int getRightTopY() const;

    void setRightTopY(int rightTopY);

    int getLeftBottomY() const;

    int getLeftBottomX() const;

    int getScale() const;

    int getWidth() const;

    int getHeight() const;

    void checkBorder(HWND hWnd, int windowWidth, int windowHeight, bool *isMouseActive);

    void checkTimers(HWND hWnd, int windowWidth, int windowHeight);

    void killAllTimers (HWND hWnd);

};


#endif //TEST_SPRITE_H
