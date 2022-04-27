#include <SFML/Graphics.hpp>
#include <iostream>
#include "vrEmuLcd.h"

#define LCD_HEIGHT 2
#define LCD_WIDTH 16

constexpr int NUM_XPIXELS = 95;
constexpr int NUM_YPIXELS = 17;

using namespace sf;
int main()
{
    RenderWindow window(VideoMode(400.f, 88.f), "SFML Application" /*Style::Close*/);
    window.setFramerateLimit(30);
    RectangleShape lcdScreen;
    lcdScreen.setSize(Vector2f(380.f, 68.f));
    lcdScreen.setPosition(10.f, 10.f);
    lcdScreen.setFillColor(Color(31, 31, 255, 255));

    RectangleShape* pixels[NUM_XPIXELS][NUM_YPIXELS];

    for (int y = 0; y < NUM_YPIXELS; ++y) {
        for (int x = 0; x < NUM_XPIXELS; ++x) {
            pixels[x][y] = new RectangleShape();
            pixels[x][y]->setSize(Vector2f(3.f, 3.f));
            pixels[x][y]->setPosition(Vector2f(lcdScreen.getPosition().x + x * 4, lcdScreen.getPosition().y + y * 4));
            pixels[x][y]->setFillColor(Color::White);
            pixels[x][y]->setOutlineThickness(3);
            pixels[x][y]->setOutlineColor(Color::Transparent);
        }
    }

    VrEmuLcd      *lcd = vrEmuLcdNew(LCD_WIDTH, LCD_HEIGHT, EmuLcdRomA00);
    // send it commands:
    vrEmuLcdSendCommand(lcd, LCD_CMD_FUNCTION | LCD_CMD_FUNCTION_LCD_2LINE | 0x10);
    vrEmuLcdSendCommand(lcd, LCD_CMD_CLEAR);
    vrEmuLcdSendCommand(lcd, LCD_CMD_HOME);
    vrEmuLcdSendCommand(lcd, LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON);
    vrEmuLcdSendCommand(lcd, LCD_CMD_DISPLAY_CURSOR);
    vrEmuLcdWriteString(lcd, "Hello, World!");
    //vrEmuLcdWriteByte(lcd, 0x26);

    while (window.isOpen())
    {
        vrEmuLcdUpdatePixels(lcd);
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(lcdScreen);

        vrEmuLcdUpdatePixels(lcd);   // generates a snapshot of the pixels state
        for (int y = 0; y < NUM_YPIXELS; ++y) {
            for (int x = 0; x < NUM_XPIXELS; ++x) {
                char pixel{vrEmuLcdPixelState(lcd, x, y)};
                if(pixel == -1 || pixel == 0) {
                    pixels[x][y]->setFillColor(Color(0, 0, 224, 255));
                } else {
                    pixels[x][y]->setFillColor(Color(240, 240, 255, 255));
                }
                window.draw(*pixels[x][y]);
            }
        }
        window.display();
    }

    for (int y = 0; y < NUM_YPIXELS; ++y) {
        for (int x = 0; x < NUM_XPIXELS; ++x) {
            delete pixels[x][y];
        }
    }
    vrEmuLcdDestroy(lcd);

}