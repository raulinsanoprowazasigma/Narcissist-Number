#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cmath>


static const unsigned WIN_W = 960u; // size of window
static const unsigned WIN_H = 520u;


static const unsigned BIG = 90;  
static const unsigned EXP = 44;   // different sizes for symbols
static const unsigned MID = 70;   
static const unsigned RESULT = 110;  

static const float BASE_Y = 230.f;
static const float EXP_Y = 188.f; // coordinates of elements
static const float PLUS_Y = 238.f; 
static const float CENTER_Y = 268.f; 


static const float SPLIT_START = 0.6f; 
static const float SPLIT_END = 1.9f; // times of display
static const float HOLD_END = 2.7f; 
static const float MERGE_END = 3.7f;



struct Step {
    int value;                 
    std::vector<int> digits;   
    int next;                  // sum
};


static float lerp(float a, float b, float t) {    // fluent sliding
    return a + (b - a) * t; 
}




static int cubesum(int n) {
    int s = 0;
    while (n > 0) {
        int d = n % 10;
        s += d * d * d;
        n /= 10;
    }
    return s;
}



static std::vector<int> digitsof(int n) {       // save digits from sum
    std::vector<int> v;
    std::string s = std::to_string(n);
    for (char c : s) v.push_back(c - '0');
    return v;
}




static std::vector<Step> build_secuence(int start) {        // precalculate the sequence
    std::vector<Step> steps;
    int v = start;
    int top = 0;
    while (v != 153 && top < 2000) {
        int n = cubesum(v);
        steps.push_back(Step{ v, digitsof(v), n });
        v = n;
        top++;
    }
    return steps;
}






static sf::Text creatext(const sf::Font& f, const std::string& s, unsigned size, sf::Color color) {
    sf::Text t(f, s, size);
    t.setFillColor(color);
    return t;
}




static void center_origin(sf::Text& t) {
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
}


enum class State { INPUT, ANIM, DONE };

int main() {
    sf::RenderWindow window(sf::VideoMode({ WIN_W, WIN_H }), "153 - Suma de cubos");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("arial.ttf") &&!font.openFromFile("C:/Windows/Fonts/arial.ttf")) return 1;

    const sf::Color COL_BCKGND(20, 20, 28);          // colours defined
    const sf::Color COL_DIGIT(245, 245, 250);
    const sf::Color COL_EXP(120, 200, 255);
    const sf::Color COL_PLUS(150, 150, 160);   
    const sf::Color COL_AMBAR(255, 200, 80);   
    const sf::Color COL_GREEN(120, 230, 140);  
    const sf::Color COL_TEXT(210, 210, 220);

    State state = State::INPUT;
    std::string input; 
    std::string warning;
    std::vector<Step> steps;
    std::size_t current = 0;
    int counter = 0;

    sf::Clock tempclock;            // current iteration clock
    sf::Clock globalclock;          // cursor blink



    auto begin = [&]() {
        warning.clear();
        if (input.empty()) { warning = "Escribe un numero primero."; return; }

        long long n = 0;
        try { n = std::stoll(input); }
        catch (...) { warning = "Numero no valido."; return; }

        if (n < 1) { warning = "Debe ser un entero positivo."; return; }
        if (n % 3) { warning = "La propiedad solo vale para multiplos de 3."; return; }

        steps = build_secuence(static_cast<int>(n));
        current = 0;
        counter = 0;
        tempclock.restart();
        state = (steps.empty()) ? State::DONE : State::ANIM;
        };

   


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* texto = event->getIf<sf::Event::TextEntered>()) {
                if (state == State::INPUT) {
                    char32_t u = texto->unicode;
                    if (u == 8) {                       // '\r'
                        if (!input.empty()) input.pop_back();
                    }
                    else if (u >= '0' && u <= '9') {
                        if (input.size() < 9) input.push_back(static_cast<char>(u));
                    }
                }
            }
            else if (const auto* tile = event->getIf<sf::Event::KeyPressed>()) {
                if (tile->code == sf::Keyboard::Key::Enter) {
                    if (state == State::INPUT) begin();
                    else { state = State::INPUT; input.clear(); warning.clear(); }
                }
                else if (tile->code == sf::Keyboard::Key::Escape) {
                    state = State::INPUT; input.clear(); warning.clear();
                }
            }
        }

        if (state == State::ANIM) {
            float t = tempclock.getElapsedTime().asSeconds(); // iteration time
            if (t >= MERGE_END) {
                counter++;
                current++;
                tempclock.restart();
                if (current >= steps.size()) state = State::DONE;
            }
        }


        window.clear(COL_BCKGND);


        if (state == State::INPUT) {
            sf::Text titulo = creatext(font, "PRIMER NUMERO NARCISISTA | SUMA DE CUBOS -> 153",34, COL_TEXT);
            center_origin(titulo);
            titulo.setPosition({ WIN_W / 2.f, 110.f });
            window.draw(titulo);

            sf::Text prompt = creatext(font, "Introduce un multiplo de 3 y pulsa Enter:",26, COL_TEXT);
            center_origin(prompt);
            prompt.setPosition({ WIN_W / 2.f, 180.f });
            window.draw(prompt);


            bool cursorOn = std::fmod(globalclock.getElapsedTime().asSeconds(), 1.0f) < 0.5f;
            std::string vis = input + (cursorOn ? "|" : "");
            sf::Text campo = creatext(font, vis.empty() ? "|" : vis, 80, COL_AMBAR);
            center_origin(campo);
            campo.setPosition({ WIN_W / 2.f, 270.f });
            window.draw(campo);

            if (!warning.empty()) {
                sf::Text msg = creatext(font, warning, 24, sf::Color(255, 120, 120));
                center_origin(msg);
                msg.setPosition({ WIN_W / 2.f, 360.f });
                window.draw(msg);
            }
        }
        else if (state == State::ANIM) {
            const Step& paso = steps[current];
            const int k = static_cast<int>(paso.digits.size());
            const float t = tempclock.getElapsedTime().asSeconds();



            float p = std::clamp((t - SPLIT_START) / (SPLIT_END - SPLIT_START), 0.f, 1.f);
            float q = std::clamp((t - HOLD_END) / (MERGE_END - HOLD_END), 0.f, 1.f);


            std::vector<sf::Text> digT;
            std::vector<float> mainW(k);
            for (int i = 0; i < k; ++i) {
                digT.push_back(creatext(font, std::to_string(paso.digits[i]), BIG, COL_DIGIT));
				sf::FloatRect b = digT[i].getLocalBounds();
                digT[i].setOrigin({ b.position.x, 0.f });
                mainW[i] = b.size.x;
            }
            sf::Text expmodel = creatext(font, "3", EXP, COL_EXP);
            float expW = expmodel.getLocalBounds().size.x;
            sf::Text plusmodel = creatext(font, "+", MID, COL_PLUS);
            float plusW = plusmodel.getLocalBounds().size.x;
            const float pad = 20.f;

            float gap = 4.f;
            float totalC = -gap;
            for (int i = 0; i < k; ++i) totalC += mainW[i] + gap;
            std::vector<float> clustX(k);
            float x = (WIN_W - totalC) / 2.f;
            for (int i = 0; i < k; ++i) { clustX[i] = x; x += mainW[i] + gap; }

            float totalS = 0.f;
            for (int i = 0; i < k; ++i) {
                totalS += mainW[i] + expW;
                if (i < k - 1) totalS += 2 * pad + plusW;
            }
            std::vector<float> spreadX(k), plusX(std::max(0, k - 1));
            x = (WIN_W - totalS) / 2.f;
            for (int i = 0; i < k; ++i) {
                spreadX[i] = x;
                x += mainW[i] + expW;
                if (i < k - 1) { plusX[i] = x + pad; x += 2 * pad + plusW; }
            }

            const float cx = WIN_W / 2.f;

            for (int i = 0; i < k; ++i) {
                float baseX = lerp(clustX[i], spreadX[i], p);
                float dx = (q > 0.f) ? lerp(spreadX[i], cx, q) : baseX;

                std::uint8_t aDig = static_cast<std::uint8_t>(255 * (1.f - q));
                digT[i].setFillColor(sf::Color(COL_DIGIT.r, COL_DIGIT.g, COL_DIGIT.b, aDig));
                digT[i].setPosition({ dx, BASE_Y });
                window.draw(digT[i]);

                std::uint8_t aExp = static_cast<std::uint8_t>(255 * p * (1.f - q));
                sf::Text e = creatext(font, "3", EXP, sf::Color(COL_EXP.r, COL_EXP.g, COL_EXP.b, aExp));
                e.setPosition({ dx + mainW[i], EXP_Y });
                window.draw(e);

                if (i < k - 1) {
                    float px = (q > 0.f) ? lerp(plusX[i], cx, q) : lerp(clustX[i], plusX[i], p);
                    std::uint8_t aPlus = static_cast<std::uint8_t>(255 * p * (1.f - q));
                    sf::Text pl = creatext(font, "+", MID, sf::Color(COL_PLUS.r, COL_PLUS.g, COL_PLUS.b, aPlus));
                    pl.setPosition({ px, PLUS_Y });
                    window.draw(pl);
                }
            }

            if (q > 0.f) {
                sf::Color base = (paso.next == 153) ? COL_GREEN : COL_AMBAR;
                std::uint8_t aRes = static_cast<std::uint8_t>(255 * q);
                sf::Text res = creatext(font, std::to_string(paso.next), RESULT,
                    sf::Color(base.r, base.g, base.b, aRes));
                center_origin(res);
                float s = lerp(0.6f, 1.0f, q);
                res.setScale({ s, s });
                sf::FloatRect db = digT[0].getLocalBounds();
                float digitCenterY = BASE_Y + db.position.y + db.size.y / 2.f;
                res.setPosition({ cx, digitCenterY });
                window.draw(res);
            }

            sf::Text pie = creatext(font, "n  ->  suma de los cubos de sus digitos",
                22, COL_TEXT);
            center_origin(pie);
            pie.setPosition({ WIN_W / 2.f, WIN_H - 40.f });
            window.draw(pie);
        }
        else {
            sf::Text n153 = creatext(font, "153", RESULT + 30, COL_GREEN);
            center_origin(n153);
            n153.setPosition({ WIN_W / 2.f, 230.f });
            window.draw(n153);

            std::string msg = "Alcanzado en " + std::to_string(counter) + " iteraciones";
            if (counter == 0) msg = "El numero ya era 153";
            sf::Text r = creatext(font, msg, 30, COL_TEXT);
            center_origin(r);
            r.setPosition({ WIN_W / 2.f, 330.f });
            window.draw(r);

            sf::Text rein = creatext(font, "Pulsa Enter para probar otro numero", 22, COL_TEXT);
            center_origin(rein);
            rein.setPosition({ WIN_W / 2.f, 390.f });
            window.draw(rein);
        }

        if (state != State::INPUT) {
            sf::Text cont = creatext(font, "Iteraciones: " + std::to_string(counter),30, COL_TEXT);
            cont.setPosition({ 24.f, 16.f });
            window.draw(cont);
        }

        window.display();
    }
    return 0;
}