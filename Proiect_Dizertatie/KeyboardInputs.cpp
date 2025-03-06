#include "KeyboardInputs.h"

/*void KeyboardInput(bool hasFocus, Player& player, sf::Vector2f size, float dt) // handles keyboard input
{
    bool coordinatesRecentlyChanged = false;
    if (hasFocus)
    {
        using kb = sf::Keyboard;

        // moving forward or backwards (1.0 or -1.0)
        float moveForward = 0.0;
        bool elevatorCalled = false;

        if (gameState == IN_GAME)
        {
            // get input
            if (kb::isKeyPressed(kb::Up))
            {
                moveForward = 1.0;
            }
            else
                if (kb::isKeyPressed(kb::Down))
                {
                    moveForward = -1.0;
                }
                else
                {
                    footstepsPlaying = false;
                    currentLevel.footsteps.stop();
                }

            // debug controls
            if (debugMode == true)
            {
                if (kb::isKeyPressed(kb::A)) // shortens the render distance and darkens the wall shading
                {
                    wallShading = 4;
                    ceilingShading = 4;
                    floorShading = 4;
                    RENDER_DISTANCE = 4;
                }

                if (kb::isKeyPressed(kb::D))
                {
                    wallShading = 1.5;
                    RENDER_DISTANCE = 16;
                }

                if (kb::isKeyPressed(kb::Q))
                {
                    std::cout << "Enter player's new X position\n";
                    std::cin >> player.position.x;
                    std::cout << "Enter player's new Y position\n";
                    std::cin >> player.position.y;
                }

                if (kb::isKeyPressed(kb::W) && cameraHeight < 0.95)
                {
                    cameraHeight += 0.1 * dt;
                    keyboardPressed = true;
                }
                else
                {
                    keyboardPressed = false;
                }

                if (kb::isKeyPressed(kb::S) && cameraHeight > 0.1)
                    cameraHeight -= 0.1 * dt;

                if (kb::isKeyPressed(kb::L))
                {
                    std::cout << "Enter level ID to load \n";
                    std::cin >> currentLevel.ID_NextLevel;

                    levelChanged = true;
                }
            }

            if (kb::isKeyPressed(kb::LShift))
            {
                player.playerMoveSpeed = 3.5;
                currentLevel.footsteps.setPitch(1.4);
            }
            else if (player.playerMoveSpeed = 4.0)
            {
                player.playerMoveSpeed = 2;
                currentLevel.footsteps.setPitch(0.9);
            }

            if (kb::isKeyPressed(kb::Space))
            {
                cameraHeight = 0.8;
                //keyboardPressed = true;
            }

            if (kb::isKeyPressed(kb::E))
            {
                if (currentLevel.ID == 0 && (int)player.position.x == 2 && (int)player.position.y == 1)
                {
                    currentLevel.ID_NextLevel = 6;
                    PlaySFX(0, soundEffect);

                    levelChanged = true;
                }

                if (currentLevel.ID == 0 && (((int)player.position.x == 1 && (int)player.position.y == 99)
                    || ((int)player.position.x == 126 && (int)player.position.y == 17)))
                {
                    currentLevel.ID_NextLevel = 1;
                    //playSFX(1, soundEffect);
                    levelChanged = true;
                }

                if (currentLevel.ID == 1 && (int)player.position.x == 124 && (int)player.position.y == 123 && coordinatesRecentlyChanged == false)
                {
                    PlaySFX(0, soundEffect);
                    player.position.y = 126.5;
                    coordinatesRecentlyChanged = true;
                    sf::sleep(sf::milliseconds(500));
                }
                if (currentLevel.ID == 1 && (int)player.position.x == 124 && (int)player.position.y == 126 && coordinatesRecentlyChanged == false)
                {
                    PlaySFX(0, soundEffect);
                    player.position.y = 123;
                    coordinatesRecentlyChanged = true;
                    sf::sleep(sf::milliseconds(500));
                }

                if (currentLevel.ID == 1 && (int)player.position.x == 1 && (int)player.position.y == 126)
                {
                    currentLevel.ID_NextLevel = 2;
                    PlaySFX(0, soundEffect);

                    levelChanged = true;
                }

                if (currentLevel.ID == 2 && (int)player.position.x == 33 && (int)player.position.y == 247 && elevatorCalled == false)
                {
                    PlaySFX(2, soundEffect);
                    globalClock.restart();
                    elevatorCalled = true;
                    currentLevel.ID_NextLevel = 4;

                    levelChanged = true;
                }

                if (currentLevel.ID == 2 && (int)player.position.x == 199 && (int)player.position.y == 99 && elevatorCalled == false)
                {
                    PlaySFX(0, soundEffect);
                    player.setPlayerNewPos(201, 99);
                    coordinatesRecentlyChanged = true;
                }

                if (currentLevel.ID == 2 && (int)player.position.x == 124 && (int)player.position.y == 177)
                {
                    PlaySFX(0, soundEffect);
                    currentLevel.ID_NextLevel = 3;

                    levelChanged = true;
                }

                if (currentLevel.ID == 5 && (int)player.position.x == 1 && (int)player.position.y == 2)
                {
                    PlaySFX(0, soundEffect);
                    player.setPlayerNewPos(5, 2.5);
                    coordinatesRecentlyChanged = true;
                }

                if (currentLevel.ID == 5 && (int)player.position.x == 4 && (int)player.position.y == 2)
                {
                    PlaySFX(0, soundEffect);
                    player.setPlayerNewPos(1.5, 3);
                    coordinatesRecentlyChanged = true;
                }
            }

            if (moveForward != 0.0f)
            {
                if (footstepsPlaying == false)
                {
                    currentLevel.footsteps.play();
                    currentLevel.footsteps.setLoop(true);
                    footstepsPlaying = true;
                }


                sf::Vector2f moveVec = player.direction * player.playerMoveSpeed * moveForward * dt;

                if (player.CanMove(sf::Vector2f(player.position.x + moveVec.x, player.position.y), size)) {
                    player.position.x += moveVec.x;
                }
                if (player.CanMove(sf::Vector2f(player.position.x, player.position.y + moveVec.y), size)) {
                    player.position.y += moveVec.y;
                }
            }

            // rotating rightwards or leftwards(1.0 or -1.0)
            float rotateDirection = 0.0;

            // get input
            if (kb::isKeyPressed(kb::Left))
            {
                rotateDirection = -1.0;
            }
            else if (kb::isKeyPressed(kb::Right))
            {
                rotateDirection = 1.0;
            }

            if (kb::isKeyPressed(kb::Escape))
            {
                gameState = PAUSE;

            }

            // handle rotation
            if (rotateDirection != 0.0)
            {
                float rotation = player.playerRotateSpeed * rotateDirection * dt;
                player.direction = player.Rotate(player.direction, rotation);
                player.plane = player.Rotate(player.plane, rotation);
            }

            coordinatesRecentlyChanged = false;
        }

    }
}*/