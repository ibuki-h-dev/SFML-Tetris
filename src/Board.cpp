#include "Board.hpp"

Board::Board():grid(HEIGHT, std::vector<int>(WIDTH, 0)){
    if(font.openFromFile(Config::font)){
        nextLabel = std::make_unique<sf::Text>(font, "NEXT", 30);
        holdLabel = std::make_unique<sf::Text>(font, "HOLD", 30);
    };
    bWidth = WIDTH*CELL_SIZE;
    bHeight = (HEIGHT-blank)*CELL_SIZE;
    plate = createRoundedRect(bWidth+2*Config::Board_X_Extra, bHeight+2*Config::Board_Y_Extra, 200.0f,Config::pointPerCorner, Config::plateColor);
    plate.setPosition({Config::Board_X-Config::Board_X_Extra, Config::Board_Y-Config::Board_Y_Extra});

    plateEdge = plate;
    plateEdge.setFillColor(sf::Color::Transparent);
    plateEdge.setOutlineThickness(2.0f);

    nextWindow = createRoundedRect(Config::nextWindowWidth, Config::nextiWindowHeight, 200.0f, Config::pointPerCorner_NW, Config::nextWindowColor);
    nextWindow.setOutlineThickness(1.0f);
    nextWindow.setOutlineColor(Config::outLineColor);
    nextWindow.setPosition({Config::nextWindow_X, Config::nextWindow_Y});

    holdWindow = createRoundedRect(Config::holdWindowWidth, Config::holdWindowHeight, 200.0f, Config::pointPerCorner_NW, Config::nextWindowColor);
    holdWindow.setOutlineThickness(1.0f);
    holdWindow.setOutlineColor(Config::outLineColor);
    holdWindow.setPosition({Config::holdWindow_X, Config::holdWindow_Y});

    frame = sf::RectangleShape({bWidth, bHeight});
    frame.setOutlineThickness(0.0f);
    frame.setOutlineColor(sf::Color::White);
    frame.setFillColor(sf::Color::Black);
    frame.setPosition({Config::Board_X,Config::Board_Y});
}

bool Board::isOccupied(int x, int y)const{
    if(x<0||x>=WIDTH||y<0||y>=HEIGHT)return true;
    return grid[y][x] != 0;
}

void Board::placeBlock(int x, int y, int colorID){
    grid[y][x] = colorID+1;
}

int Board::clearLines(){
    int clearcouter = 0;
    for(int y = HEIGHT - 1; y >= blank; --y){
        bool full = true;
        for(int x = 0; x < WIDTH;  ++x){
            if(grid[y][x] == 0) {
                full = false;
                break;
            }
        }
        if(full){
            float screenY = Config::Board_Y + (y - blank-clearcouter) * CELL_SIZE;
            activeEffects.push_back({screenY, 0.0f});
            grid.erase(grid.begin() + y);
            grid.insert(grid.begin(), std::vector<int>(WIDTH, 0));
            y++;
            clearcouter++;
        }
        
    }
    return clearcouter;
}

void Board::draw(sf::RenderWindow& window)const {
   
    window.draw(plate);
    window.draw(plateEdge);
    window.draw(frame);

    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 0.5f, CELL_SIZE - 0.5f));
    cell.setOutlineThickness(0.5f);
    cell.setOutlineColor(sf::Color(255, 255, 255, 100));

    for(int y=0; y < HEIGHT; ++y){
        for(int x=0; x < WIDTH; ++x){
            if(grid[y][x]==0 && y<blank) continue;
            switch(grid[y][x]) {
                case 0: cell.setFillColor(sf::Color::Transparent); break;
                case 1: cell.setFillColor(sf::Color::Cyan); break;
                case 2: cell.setFillColor(sf::Color::Yellow); break;
                case 3: cell.setFillColor(sf::Color::Green); break;
                case 4: cell.setFillColor(sf::Color::Red); break;
                case 5: cell.setFillColor(sf::Color::Blue); break;
                case 6: cell.setFillColor(sf::Color(255, 165, 0)); break;
                case 7: cell.setFillColor(sf::Color(128, 0, 128)); break;
                default: cell.setFillColor(sf::Color::White); break;
            }

            cell.setPosition({Config::Board_X+x* (float)CELL_SIZE, Config::Board_Y+(y-blank) * (float)CELL_SIZE});
            window.draw(cell);
        }
    }
    drawNextWindow(window);
    drawHoldWindow(window);

    for(const auto& effect : activeEffects){
        sf::RectangleShape flash({WIDTH*CELL_SIZE, CELL_SIZE});
        flash.setPosition({(float)Config::Board_X, effect.y_pos});

        sf::Color c = getRainbowColor(colorTimer);
        c.a = static_cast<std::uint8_t>((1.0f-effect.timer)*255);

        flash.setFillColor(c);
        flash.setOutlineThickness(1.0f);
        flash.setOutlineColor(sf::Color::White);
        flash.setOrigin({0, CELL_SIZE/2.0f});
        flash.setPosition({(float)Config::Board_X, effect.y_pos + CELL_SIZE/2.0f});
        flash.setScale({1.0f, 1.0f+effect.timer*2.0f});

        window.draw(flash);
    }
}

void Board::drawNextWindow(sf::RenderWindow& window)const{
    window.draw(nextWindow);
    if (nextLabel) {
        nextLabel->setPosition({Config::nextWindow_X+Config::nextWindowWidth/2-36, Config::nextWindow_Y - 40.f});
        nextLabel->setFillColor(Config::fontColor);
        window.draw(*nextLabel);
    } 
}
void Board::drawHoldWindow(sf::RenderWindow& window)const{
    window.draw(holdWindow);
    if(holdLabel){
        holdLabel->setPosition(sf::Vector2f{Config::holdWindow_X+24, Config::holdWindow_Y-36});
        holdLabel->setFillColor(Config::fontColor);
        window.draw(*holdLabel);
    }
}
void Board::update(float dt) {
    colorTimer += dt * 0.5f; 
    if (colorTimer > 1.0f) colorTimer -= 1.0f;

    sf::Color rainbow = getRainbowColor(colorTimer);
    plateEdge.setOutlineColor(rainbow);
    
    float thickness = 3.0f + std::sin(colorTimer * 10.0f) * 1.0f;
    plateEdge.setOutlineThickness(thickness);

    for(auto it = activeEffects.begin(); it != activeEffects.end();){
        it->timer += dt*2.0f;
        if(it->timer>1.0f){
            it = activeEffects.erase(it);
        }else{
            ++it;
        }
    }
}