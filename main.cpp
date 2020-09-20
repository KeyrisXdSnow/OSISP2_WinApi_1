#include <iostream>

#include <windows.h>
#include <tchar.h>
#include <vector>
#include "Sprite.h"
#include <gdiplus.h>

using namespace std;

const double SCALE = 0.15;
const double SPEED = 15;
const int coordinateStep = 1;

int incCoordinate (int axis) {
    return axis += coordinateStep * SPEED;
}
int decCoordinate (int axis) {
    return axis -= coordinateStep * SPEED;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam) { // Обработчик сообщений

    HDC hdc;
    PAINTSTRUCT ps;

    // для загрузки картинки
    static HDC memBit;
    static BITMAP bitmap;
    HANDLE hBitmap;

    static int x,y, width, height ;

    static bool isMouseActive = false ;
    static int oldMousePosX, oldMousePosY ;


    static Sprite *sprite = new Sprite (SCALE, SPEED) ;

    static UINT_PTR windowTimer ;

    switch (message) {

        case WM_CREATE : {

            windowTimer = SetTimer(hWnd,0,1,NULL) ;

            hBitmap = (HBITMAP) LoadImage(NULL, _T("C:\\Users\\Keyris\\CLionProjects\\test\\123.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

            if (hBitmap == NULL) {
                std::cout << "File not found" << std::endl;
                break;
            }
            GetObject(hBitmap, sizeof(bitmap), &bitmap);
            hdc = GetDC(hWnd);
            memBit = CreateCompatibleDC(hdc);
            SelectObject(memBit, hBitmap);
            ReleaseDC(hWnd,hdc);
            break;
        }

        case WM_SIZE : {
            width = LOWORD(lParam);
            height = HIWORD(lParam);

            sprite->recalculateCoordinates(width,height,width / 2,height / 2);

            break;
        }

        case WM_TIMER : {

            POINT pt;
            GetCursorPos( &pt );

            if( hWnd != WindowFromPoint( pt ) ) {
                int x = sprite->getRightTopX();
                int y = sprite->getRightTopY();

                if (windowTimer != 0 && isMouseActive) {

                    if (x <= width / 2) {
                        if (x < y) sprite->setRightTopX(0);
                        else sprite->setRightTopY(0);
                    } else {
                        if (x > y) sprite->setRightTopY(0);

                    }
                }

                isMouseActive = false;
                KillTimer(hWnd, 0);
                windowTimer = 0;
            }
               sprite->checkBorder(hWnd,width,height,&isMouseActive);
               sprite->checkTimers(hWnd,width,height);

            break;
        }
        case WM_KEYDOWN : {
            switch (wParam) {

                case VK_LEFT : {
                    sprite->setRightTopX(decCoordinate(sprite->getRightTopX()));
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    break;
                }

                case VK_RIGHT : {
                    sprite->setRightTopX( incCoordinate(sprite->getRightTopX()) );
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    break;
                }

                case VK_UP : {
                    sprite->setRightTopY( decCoordinate(sprite->getRightTopY()) );
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    break;
                }

                case VK_DOWN : {
                    sprite->setRightTopY( incCoordinate(sprite->getRightTopY()) );
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    break;
                }
            }
            InvalidateRect(hWnd, NULL, true);
            break;
        }

        case WM_MOUSEWHEEL : {

            if (isMouseActive) break;

            int direction = GET_WHEEL_DELTA_WPARAM(wParam);

            if (LOWORD(wParam) & MK_SHIFT) {
                if (direction > 0) {
                    sprite->setRightTopX(decCoordinate(sprite->getRightTopX()));
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    InvalidateRect(hWnd, NULL, true);
                }
                else if (direction < 0 ) {
                    sprite->setRightTopX(incCoordinate(sprite->getRightTopX()));
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    InvalidateRect(hWnd, NULL, true);
                }
            } else {
                if (direction > 0) {
                    sprite->setRightTopY(decCoordinate(sprite->getRightTopY()));
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    InvalidateRect(hWnd, NULL, true);
                }
                else if (direction < 0 ) {
                    sprite->setRightTopY(incCoordinate(sprite->getRightTopY()));
                    sprite->checkBorder(hWnd,width,height,&isMouseActive);
                    InvalidateRect(hWnd, NULL, true);
                }
            }
            break;
        }

        case WM_LBUTTONDOWN : {

            int x =LOWORD(lParam), y = HIWORD(lParam);

            if ( ( x >= sprite->getRightTopX() && x <= sprite->getLeftBottomX()) && ( y >= sprite->getRightTopY()
            && y <= sprite->getRightTopY() + height ) ) {

                if (isMouseActive) {
                    isMouseActive = false;
                    break;
                }

                hdc = GetDC(hWnd);

                oldMousePosX = x - sprite->getRightTopX();
                oldMousePosY = y;
                isMouseActive = true;

                ReleaseDC(hWnd, hdc);
            }
            break;
        }

        case WM_LBUTTONDBLCLK : {
            // смена спрайта
            break;
        }
        case WM_MOUSEMOVE : {

            if (windowTimer == 0 )
                windowTimer = SetTimer(hWnd,0,500,NULL) ;


            if (isMouseActive) {
                sprite->setRightTopX( LOWORD(lParam) - sprite->getWidth()/2);
                sprite->setRightTopY(HIWORD(lParam) -  sprite->getWidth()/2);
                sprite->checkBorder(hWnd,width,height,&isMouseActive);
                InvalidateRect(hWnd, NULL, true);
                UpdateWindow(hWnd);
            }
        }

        case WM_PAINT : {
            hdc = BeginPaint(hWnd,&ps);                  
            //BitBlt(hdc,  sprite->getRightTopX(),  sprite->getRightTopY(), bitmap.bmWidth, bitmap.bmHeight, memBit, 0, 0, SRCCOPY);
            StretchBlt(hdc, sprite->getRightTopX(),sprite->getRightTopY(),sprite->getWidth(),sprite->getHeight(),memBit,0,0,bitmap.bmWidth, bitmap.bmHeight,SRCCOPY);
            //Rectangle(hdc,sprite->getRightTopX(), sprite->getRightTopY(),sprite->getLeftBottomX(),sprite->getLeftBottomY());
            //Ellipse(hdc,sprite->getRightTopX(), sprite->getRightTopY(),sprite->getLeftBottomX(),sprite->getLeftBottomY());
            EndPaint (hWnd,&ps);
            break;
        }

        case WM_DESTROY :
            KillTimer(hWnd,0);
            sprite->killAllTimers(hWnd);
            delete sprite;
            DeleteDC(memBit);
            PostQuitMessage(0);
            break; // Завершение программы
            // Обработка сообщения по умолчанию

        default : {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This, // Дескриптор текущего приложения
                       HINSTANCE Prev, // В современных системах всегда 0. Нужен только для совместимости с ситеами старше Windows98 -_-
                       LPTSTR cmd, // Командная строка
                       int mode) // Режим отображения окна
{

    // 2. Создание класса

    HWND hWnd; // Дескриптор главного окна программы
    MSG msg; // Структура для хранения сообщения
    WNDCLASS wc; // Класс окна

    //wc — структура, содержащая информацию по настройке окна. Требуется заполнить следующие поля:

    wc.hInstance = This; //Дескриптор текущего приложения
    wc.lpszClassName = WinName; // Имя класса окна
    wc.lpfnWndProc = WndProc; // Имя функции для обработки сообщений.
    wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна
    //Такой стиль определяет автоматическую перерисовку окна при изменении
    //его ширины или высоты.
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Стандартная иконка
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
    wc.lpszMenuName = NULL; // Нет меню
    wc.cbClsExtra = 0; // Нет дополнительных данных класса
    wc.cbWndExtra = 0; // Нет дополнительных данных окна

    // Заполнение окна белым цветом
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 2);
    //Дескриптор кисти, которая используется для заполнения окна.
    //Стандартная конструкция, создает системную кисть белого цвета
    //WHITE_BRUSH. Требуется явное преобразование типа — HBRUSH.

    //2. Регистрация класса окна

    if (!RegisterClass(&wc))
        return 0;

    // Создание окна
    hWnd = CreateWindow(WinName, // Имя класса окна
                        _T("Каркас Windows-приложения"), // Заголовок окна
                        WS_OVERLAPPEDWINDOW, // Стиль окна
                        //WS_OVERLAPPEDWINDOW — макрос, определяющий стиль отображения стандартного окна, имеющего системное меню, заголовок, рамку для изменения размеров
                        //а также кнопки минимизации, развертывания и закрытия. Можно выбрать и другой стиль. WS_OVERLAPPED — стандартное окно с рамкой;
                        CW_USEDEFAULT, // x левого верхнего угла
                        CW_USEDEFAULT, // y девого верхнего угла
                        CW_USEDEFAULT, // width
                        CW_USEDEFAULT, // Height
                        // CW_USEDEFAULT означает, что система сама выберет
                        // для отображения окна наиболее (с ее точки зрения) удобное место и размер.
                        HWND_DESKTOP, // Дескриптор родительского окна
                        NULL, // Нет меню
                        This, // Дескриптор приложения
                        NULL); // Дополнительной информации нет

    ShowWindow(hWnd, mode); // Показать окно
    UpdateWindow(hWnd);

    // Цикл обработки сообщений

    // GetMessage
    //первы параметр &msg указатель на структуру MESSAGE, где и хранятся сообщения
    //второй параметр hwnd — определяет окно, для которого предназначено сообщение, если же необходимо перехватить сообщения всех окон данного приложения, он должен быть NULL;
    //остальные два параметра определяют [min, max] диапазон получаемых сообщений. Чаше всего необходимо обработать все сообщения, тогда эти параметры
    //должны быть равны 0
    //Сообщения определяются их номерами, символические имена для них определены и
    //файле включений winuser.h. Префикс всех системных сообщений WM_.
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // транслирует код нажатой клавиши в клавиатурные сообщения WM_CHAR.
        // При этом в переменную wParam структуры msg помещается код нажатой клавиши в Windows-кодировке CP-1251, в младшее слово lParam — количество
        //повторений этого сообщения в результате удержания клавиши в нажатом состоянии,
        //а в старшее слово — битовая карта со значениями,
        //Использование этой функции не обязательно и нужно только для обработки сообщений от клавиатуры.
        DispatchMessage(&msg); // обеспечивает возврат преобразованного сообщения обратно операционной системе и посылает сообщение функции обработки сообещний. У нас это WndProc()
    }
    return 0;
}


