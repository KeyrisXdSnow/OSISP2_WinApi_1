
#include <iostream>
#include "Sprite.h"

Sprite::Sprite(double scale, int speed) {
    this->scale = scale;
    this->speed = speed;
}

void Sprite::recalculateCoordinates(int windowWidth, int windowHeight, int windowCenterX, int windowCenterY) {
    width = windowWidth * scale * 0.5;
    height = windowHeight * scale;

    rightTopX = windowCenterX - 0.5 * width;
    rightTopY = windowCenterY - 0.5 * height;

}

int Sprite::getRightTopX() const {
    return rightTopX;
}

void Sprite::setRightTopX(int rightTopX) {
    Sprite::rightTopX = rightTopX;
}

int Sprite::getRightTopY() const {
    return rightTopY;
}

void Sprite::setRightTopY(int rightTopY) {
    Sprite::rightTopY = rightTopY;
}

int Sprite::getLeftBottomY() const {
    return rightTopY + height;
}

int Sprite::getLeftBottomX() const {
    return rightTopX + width;
}

int Sprite::getWidth() const {
    return width;
}

int Sprite::getHeight() const {
    return height;
}

void Sprite::checkBorder(HWND hWnd, int windowWidth, int windowHeight, bool *isMouseActive) {

    if (rightTopX <= 0) {
        rightTopX = 0;
        timers[0] = SetTimer(hWnd, 1, timerTime, NULL);
        if ( *isMouseActive ) *isMouseActive = false;
         return;
    }
    if (rightTopX + width >= windowWidth) {
        rightTopX = windowWidth - width;
        timers[2] = SetTimer(hWnd, 3, timerTime, NULL);
        if ( *isMouseActive ) *isMouseActive = false;
        return;
    }
    if ( rightTopY <= 0) {
        rightTopY = 0;
        timers[1] = SetTimer(hWnd, 2, timerTime, NULL);
        if ( *isMouseActive ) *isMouseActive = false;
        return;
    }
    if ( rightTopY + height >= windowHeight) {
        rightTopY = windowHeight - height;
        timers[3] = SetTimer(hWnd, 4, timerTime, NULL);
        if ( *isMouseActive ) *isMouseActive = false;
        return;
    }
}

void Sprite::checkTimers(HWND hWnd, int windowWidth, int windowHeight) {

    if (timers[0] != 0) {
        if (rightTopX + width >= windowWidth*0.1){
            KillTimer(hWnd, 1);
            timers[0] = 0;
        } else {
            if (timers[2] != 0) {
                KillTimer(hWnd, 3);
                timers[2] =0;
            }
            rightTopX += 1;
            InvalidateRect(hWnd, NULL, true);
            return;
        }
    }

    if (timers[2] != 0) {
        if (rightTopX  <= windowWidth*0.9) {
            KillTimer(hWnd, 3);
            timers[2] = 0;
            InvalidateRect(hWnd, NULL, true);
            return;
        } else {
            if (timers[0] != 0) {
                KillTimer(hWnd, 1);
                timers[0] =0;
            }
            rightTopX -= 1;
            InvalidateRect(hWnd, NULL, true);
            return;
        }

    }

    if (timers[1] != 0) {
        if (rightTopY + height >= windowHeight*0.2) {
            KillTimer(hWnd, 2);
            timers[1] = 0;
        } else {
            if (timers[3] != 0) {
                KillTimer(hWnd, 4);
                timers[3] = 0;
            }
            rightTopY += 1;
            InvalidateRect(hWnd, NULL, true);
            return;
        }
    }

    if (timers[3] != 0) {
        if (rightTopY <= windowHeight*0.8) {
            KillTimer(hWnd, 4);
            timers[3] = 0;
        } else {
            if (timers[1] != 0) {
                KillTimer(hWnd, 2);
                timers[1] = 0;
            }
            rightTopY -= 1;
            InvalidateRect(hWnd, NULL, true);
            return;
        }
    }

}

