#include "Game.hpp"

Game::Game():
    window(sf::VideoMode({Config::ScreenWidth, Config::ScreenHeight}), "C++Tetris"),
    bg(30),
    board(),
    piece(0),
    state(State::Playing),
    timer(0.0f),
    lineClearTimer(0.4f),
    lockDelay_changable(Config::lockDelay),
    isTouchingGround(false),
    canHold(true)
{
    window.setVerticalSyncEnabled(true);
    refillBag();
    spawnPiece();

    if(font.openFromFile(Config::font)){

    

    gameOverText = std::make_unique<sf::Text>(font, "Game Over\n Tap to Reset", 40);
    gameOverText->setPosition({Config::ScreenWidth/2.0f-100.0f, Config::ScreenHeight/2.0f});
    gameOverText->setFillColor(sf::Color::White);
    }
};
void Game::refillBag() {
    std::vector<int> bag = {0, 1, 2, 3, 4, 5, 6};
    
    // 乱数生成器（staticにして保持）
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    
    std::shuffle(bag.begin(), bag.end(), rng);
    
    for (int id : bag) {
        next.push_back(id);
    }
}
void Game::spawnPiece(){
    currentID = next.front();
    next.erase(next.begin());
    piece = Piece(currentID);
    if(!piece.isPossible(board, piece.getX(0), piece.getY(0), piece.body)){
        state = State::GameOver;
    }
    if(next.size()<7){
        refillBag();
    }
}
void Game::run(){
    while(window.isOpen()){
        float dt = clock.restart().asSeconds();
        processEvent(dt);
        update(dt);
        render();
    }
}
void Game::processEvent(float dt){
    while(const std::optional event = window.pollEvent()){
        
        if(event->is<sf::Event::Closed>()){
            window.close();
        }

        if(auto* touchBegan = event->getIf<sf::Event::TouchBegan>()){
            touchStartPos = touchBegan->position;
            lastMovePos = touchBegan->position;
            isSwiping = false;
        }

        if(auto* touchMoved = event->getIf<sf::Event::TouchMoved>()){
            sf::Vector2i currentPos = touchMoved->position;
            float dx = static_cast<float>(currentPos.x-lastMovePos.x);

            if(std::abs(dx) > swipeDist){
                int dir = (dx>0) ? 1 : -1;
                if(piece.move(board, dir, 0)){
                    lastMovePos = currentPos;
                    isSwiping = true;
                }
            }
        }

        if(auto* touchEnded = event->getIf<sf::Event::TouchEnded>()){
            sf::Vector2i endPos = touchEnded->position;
            float totalDist = std::sqrt(std::pow(endPos.x - touchStartPos.x, 2)+std::pow(endPos.y-touchStartPos.y, 2));

            if(!isSwiping&&totalDist< 20.0f){
                piece.rotate(board, true);
            }
        }
    }
}
void Game::update(float dt){
    if(state == State::GameOver){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){
            reset();
        }
    }
    bg.update();
    board.update(dt);
    if(state==State::ClearLine){
        lineClearTimer -= dt;
        if(lineClearTimer<0){
            lineClearTimer = 0.4f;
            state = State::Playing;
            spawnPiece();
        }
        return;
    }

    handleInput(dt);
    timer += dt;
     if(timer > Config::defaultDelay){
            if(!piece.move(board, 0, 1)){
                isTouchingGround = true;
            } else{
                isTouchingGround = false;
                lockClock.reset();
            }
            timer=0.0f;
        }
        if(isTouchingGround){
            if(lockClock.getElapsedTime().asSeconds() > lockDelay_changable){
                for(int i=0; i<4; i++){
                    board.placeBlock(piece.getX(i), piece.getY(i), piece.getID());
                }
                if(board.clearLines()>0){
                    state = State::ClearLine;
                }else{
                    spawnPiece();
                }
                isTouchingGround = false;
                lockClock.restart();
                canHold = true;
                lockDelay_changable = 0.5f;
            }
        }else{
            lockClock.restart();
        }
}

void Game::render(){
    window.clear();
    bg.draw(window);
    board.draw(window);
    if(state==State::Playing||state==State::GameOver){
        Piece shadow = piece;
        while(shadow.move(board, 0, 1)){}
        shadow.draw(window, true);
        piece.draw(window, false);
    }
    for (int i = 0; i < 5; i++) { // 先頭3つを表示
        Piece nextPiece(next[i]);
        nextPiece.drawAt(window, Config::nextWindow_X + Config::nextWindowWidth/2.5,Config::nextWindow_Y+ (i) * 80+40, true);
    }
    if (hold.has_value()) {
        Piece holdPiece(hold.value());
        holdPiece.drawAt(window, Config::holdWindow_X+Config::holdWindowWidth/2.5, Config::holdWindow_Y+Config::holdWindowHeight/2.5, true);
    }
    
    if(state == State::GameOver){
        window.draw(*gameOverText);
    }
    window.display();
}

void Game::reset(){
    for(auto& line : board.grid){
        std::fill(line.begin(), line.end(), 0);
    }
    next.clear();
    refillBag();
    state = State::Playing;
    spawnPiece(); // 最初の一個を生成
    canHold = true;
}

void Game::handleInput(float dt){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        if(!heldKeys[0]){
            piece.move(board, -1, 0);
            dasClock.restart();
            heldKeys[0] = true;
        }else if(dasClock.getElapsedTime().asSeconds()>Config::dasDelay){
            if(arrClock.getElapsedTime().asSeconds() > Config::arrDelay){
                piece.move(board, -1, 0);
                arrClock.restart();
            }
        }
    }else{
        heldKeys[0] =false;
    }
        
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        if(!heldKeys[1]){
            piece.move(board, +1, 0);
            dasClock.restart();
            heldKeys[1] = true;
        }else if(dasClock.getElapsedTime().asSeconds()>Config::dasDelay){
            if(arrClock.getElapsedTime().asSeconds() > Config::arrDelay){
                piece.move(board, +1, 0);
                arrClock.restart();
            }
        }
    }else{
        heldKeys[1] = false;
    }
        
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        if(!heldKeys[5]){
            while(piece.move(board , 0, 1)){}
            isTouchingGround = true;
            lockDelay_changable = 0.2f;
            heldKeys[5] = true;
        }
    }else{
        heldKeys[5] = false;
    }
        
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        if(!heldKeys[2]){
            piece.move(board, 0, 1);
            dasClock.restart();
            heldKeys[2] = true;
        }else if(dasClock.getElapsedTime().asSeconds()> Config::dasDelay){
            if(arrClock.getElapsedTime().asSeconds() > Config::arrDelay){
                piece.move(board, 0, 1);
                arrClock.restart();
            }
        }else{
            heldKeys[2] = false;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Slash)){
        if(!heldKeys[3]){
            piece.rotate(board, true);
            heldKeys[3] = true;
        }
    }else{
        heldKeys[3] = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)){
        if(!heldKeys[4]){
            piece.rotate(board, false);
            heldKeys[4] = true;
        }
    }else{
        heldKeys[4] = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
        if(canHold){
            if(!hold.has_value()){
                hold = currentID;
                currentID = next.front();
                piece = Piece(currentID);
            }else{
                int tmp = currentID;
                currentID = hold.value();
                hold = tmp;                    piece = Piece(currentID);
            }
            canHold = false; 
        }
    }
}
