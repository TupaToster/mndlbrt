#include "protos.h"

// Draw related constants and variables
const unsigned int ScrSize = 1000;  // pixel size of screen (its square)
float XLIML = -2.0; // lower x starting lim
float XLIMH = 0.0; // upper x starting lim
float YLIML = -1.0; // lower y starting lim
float YLIMH = 1.0;  // upper y starting lim
float delta = 0.1; // how much to move per key press (scales when zooming)
const float sigma = 0.8; // coeffitient for zooming/dezooming

int main () {

    sf::RenderWindow window (sf::VideoMode (ScrSize, ScrSize), "mndlbrt");

    sf::Texture pixlArr;    // creates a texture
    pixlArr.create (ScrSize, ScrSize); // sets its dimensions to ScrSize * ScrSize

    unsigned char rgbaArray[ScrSize * ScrSize * 4] = {0};    // Array of rgb quads

    while (window.isOpen ()) {

        clock_t time = clock ();    // gets initial time

        for (int i = 0; i < ScrSize * ScrSize * 4; i+=4) {

            calcPoint (rgbaArray + i, rgbaArray + i + 1, rgbaArray + i + 2, rgbaArray + i + 3,
                XLIML + float((i / 4) % ScrSize) / ScrSize * (XLIMH - XLIML),       // calls calc function
                YLIMH - float(i / (ScrSize * 4)) / ScrSize * (YLIMH - YLIML));
        }

        time = (clock () - time) * 1000 / CLOCKS_PER_SEC; // measures work time

        printf ("%ld\n", time);

        pixlArr.update (rgbaArray);      // updates texture with rgbquad array

        sf::Sprite sprite (pixlArr);    // creates sprite (хз зачем но так в примере сфмла написано)

        window.draw (sprite);   // draws to window
        window.display ();  // displays it

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) {

            XLIMH -= delta;
            XLIML -= delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) {

            XLIML += delta;
            XLIMH += delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up)) {

            YLIML += delta;
            YLIMH += delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down)) {

            YLIML -= delta;
            YLIMH -= delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal)) {    // Zoom

            float shift = (XLIMH - XLIML - sigma * (XLIMH - XLIML)) / 2;
            XLIML += shift;
            XLIMH -= shift;
            YLIMH -= shift;
            YLIML += shift;
            delta *= sigma;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)) {     // UnZoom

            float shift = ((XLIMH - XLIML) / sigma - XLIMH + XLIML) / 2;
            XLIML -= shift;
            XLIMH += shift;
            YLIMH += shift;
            YLIML -= shift;
            delta /= sigma;
        }

        sf::Event event;        // Event var

        while (window.pollEvent(event)) {   // Check for being closed if there is an event (to close the window lol)

            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
