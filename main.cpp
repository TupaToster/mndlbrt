#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <chrono>

const unsigned int ScrSize = 1000;
float XLIML = -2.0;
float XLIMH = 0.0;
float YLIML = -1.0;
float YLIMH = 1.0;
float delta = 0.1;
const float sigma = 0.8;

inline void calcPoint (unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a, float x, float y) {

    unsigned int i = 0;

    float x_0 = x;
    float y_0 = y;
    const int topCalcLimit = 100;
    const float maxDist = 100;
    const unsigned char r0 = 49;
    const unsigned char g0 = 56;
    const unsigned char b0 = 54;
    const unsigned char r1 = 255;
    const unsigned char g1 = 10;
    const unsigned char b1 = 10;

    for (i = 0; i < topCalcLimit; i++) {

        if (x * x + y * y >= maxDist) break;
        float xt = x;
        x = x * x - y * y + x_0;
        y = 2 * xt * y + y_0;

    }
    *r = r0 + (i * (r1 - r0) / topCalcLimit);
    *g = g0 + (i * (g1 - g0) / topCalcLimit);
    *b = b0 + (i * (b1 - b0) / topCalcLimit);

    *a = 255;
}


int main () {

    sf::RenderWindow window (sf::VideoMode (ScrSize, ScrSize), "mndlbrt");

    sf::Texture pixlArr;
    pixlArr.create (ScrSize, ScrSize);

    unsigned char rgbArray[ScrSize * ScrSize * 4] = {0};

    while (window.isOpen ()) {


        clock_t time = clock ();

        for (int i = 0; i < ScrSize * ScrSize * 4; i+=4) {

            calcPoint (rgbArray + i, rgbArray + i + 1, rgbArray + i + 2, rgbArray + i + 3,
                XLIML + float((i / 4) % ScrSize) / ScrSize * (XLIMH - XLIML),
                YLIMH - float(i / (ScrSize * 4)) / ScrSize * (YLIMH - YLIML));
        }

        time = (clock () - time) * 1000 / CLOCKS_PER_SEC;

        printf ("%ld\n", time);

        pixlArr.update (rgbArray);

        sf::Sprite sprite (pixlArr);

        window.draw (sprite);
        window.display ();

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
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal)) {

            float shift = (XLIMH - XLIML - sigma * (XLIMH - XLIML)) / 2;
            XLIML += shift;
            XLIMH -= shift;
            YLIMH -= shift;
            YLIML += shift;
            delta *= sigma;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)) {

            float shift = ((XLIMH - XLIML) / sigma - XLIMH + XLIML) / 2;
            XLIML -= shift;
            XLIMH += shift;
            YLIMH += shift;
            YLIML -= shift;
            delta /= sigma;
        }

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
