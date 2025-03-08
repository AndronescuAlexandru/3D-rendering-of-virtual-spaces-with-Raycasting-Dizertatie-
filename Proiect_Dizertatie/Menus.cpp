#pragma once
#ifndef MENUS_H
#include "Menus.h"

int menuChoice = 0;
int button_width = 200;
int button_height = 30;

sf::Text startButtonText;
sf::Text loadGameButtonText;
sf::Text optionsButtonText;
sf::Text exitButtonText;
sf::Text versionID;

sf::RectangleShape startGameButton;
sf::RectangleShape loadGameButton;
sf::RectangleShape optionsButton;
sf::RectangleShape exitGameButton;

sf::Texture mainMenuBackgroundTexture;
sf::Sprite mainMenuBackgroundSprite;

void ResizeMainMenuBackground(sf::View view)
{
    unsigned int width = view.getSize().x;
    unsigned int height = view.getSize().y;
    float scaleX = static_cast<float>(width) / mainMenuBackgroundTexture.getSize().x;
    float scaleY = static_cast<float>(height) / mainMenuBackgroundTexture.getSize().y;

    // Maintain aspect ratio (optional)
    float scale = std::max(scaleX, scaleY);
    mainMenuBackgroundSprite.setScale(scale, scale);

    // Center the sprite
    float newWidth = mainMenuBackgroundTexture.getSize().x * scale;
    float newHeight = mainMenuBackgroundTexture.getSize().y * scale;
    mainMenuBackgroundSprite.setPosition((width - newWidth) / 2.f, (height - newHeight) / 2.f);

}

void UIConstruct(sf::Font font)
{
    startGameButton.setSize(sf::Vector2f(button_width, button_height));
    loadGameButton.setSize(sf::Vector2f(button_width, button_height));
    optionsButton.setSize(sf::Vector2f(button_width, button_height));
    exitGameButton.setSize(sf::Vector2f(button_width, button_height));

    startGameButton.setFillColor(sf::Color::Black);
    loadGameButton.setFillColor(sf::Color::Black);
    optionsButton.setFillColor(sf::Color::Black);
    exitGameButton.setFillColor(sf::Color::Black);
    versionID.setFillColor(sf::Color::White);

    startButtonText.setFont(font);
    loadGameButtonText.setFont(font);
    optionsButtonText.setFont(font);
    exitButtonText.setFont(font);
    versionID.setFont(font);

    startButtonText.setString("Start demo level");
    loadGameButtonText.setString("Load level from file");
    optionsButtonText.setString("Options");
    exitButtonText.setString("Exit game");
    versionID.setString(VERSION_ID);

    startButtonText.setCharacterSize(20);
    loadGameButtonText.setCharacterSize(20);
    optionsButtonText.setCharacterSize(20);
    exitButtonText.setCharacterSize(20);
    versionID.setCharacterSize(20);

    startButtonText.setPosition(screenWidth / 2 - button_width / 3, screenHeight / 2);
    loadGameButtonText.setPosition(screenWidth / 2 - button_width / 3, screenHeight / 2 + 35);
    optionsButtonText.setPosition(screenWidth / 2 - button_width / 3, screenHeight / 2 + 70);
    exitButtonText.setPosition(screenWidth / 2 - button_width / 3, screenHeight / 2 + 105);
    versionID.setPosition(5, screenHeight - 40);

    startGameButton.setPosition(screenWidth / 2 - button_width/2, screenHeight / 2);
    loadGameButton.setPosition(screenWidth / 2 - button_width / 2, screenHeight / 2 + 35);
    optionsButton.setPosition(screenWidth / 2 - button_width / 2, screenHeight / 2 + 70);
    exitGameButton.setPosition(screenWidth / 2 - button_width / 2, screenHeight / 2 + 105);

    if (!mainMenuBackgroundTexture.loadFromFile("Data/Textures/menu_background.jpg"))
    {
        printf("Cannot open image file menu_background.jpg");
    }

    mainMenuBackgroundSprite.setTexture(mainMenuBackgroundTexture);
    mainMenuBackgroundSprite.setScale(sf::Vector2f(menuSpriteScaleX, menuSpriteScaleY));
}

void MainMenu(sf::RenderWindow &window , sf::RenderStates &state, sf::Font font)
{
    window.clear();

    currentLevel.AmbientSFX.stop();

    window.draw(mainMenuBackgroundSprite);

    window.draw(startGameButton);
    window.draw(loadGameButton);
    window.draw(optionsButton);
    window.draw(exitGameButton);

    window.draw(startButtonText);
    window.draw(loadGameButtonText);
    window.draw(optionsButtonText);
    window.draw(exitButtonText);
    window.draw(versionID);

    startGameButton.setFillColor(sf::Color::Black);
    optionsButton.setFillColor(sf::Color::Black);
    exitGameButton.setFillColor(sf::Color::Black);

    startButtonText.setFillColor(sf::Color::White);
    loadGameButtonText.setFillColor(sf::Color::White);
    optionsButtonText.setFillColor(sf::Color::White);
    exitButtonText.setFillColor(sf::Color::White);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && menuChoice < 3)
    {
        menuChoice++;
        sf::sleep(sf::milliseconds(100));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && menuChoice > 0)
    {
        menuChoice--;
        sf::sleep(sf::milliseconds(100));
    }

    if (noSaveFile)
        loadGameButton.setFillColor(sf::Color(74, 74, 74));
    else
        loadGameButton.setFillColor(sf::Color::Black);

    switch (menuChoice)
    {
    case 0:
        startGameButton.setFillColor(sf::Color::White);
        startButtonText.setFillColor(sf::Color::Black);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            currentLevel.ID = 0;
            currentLevel.ID_NextLevel = 0;

            if (debugMode == true)
            {
                printf("Enter level choice\n");
                scanf_s("%d", &currentLevel.ID);
            }

            gameState = IN_GAME;
            window.setMouseCursorVisible(false);
            currentLevel.loadLevel(window, state, true);
        }

        break;
    case 1:
        if (noSaveFile)
        {
            loadGameButton.setFillColor(sf::Color(135, 135, 135));
            loadGameButtonText.setFillColor(sf::Color::White);
        }
        else
        {
            loadGameButton.setFillColor(sf::Color::White);
            loadGameButtonText.setFillColor(sf::Color::Black);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && noSaveFile == false)
        {
            LoadSaveFile();
            gameState = IN_GAME;
            window.setMouseCursorVisible(false);
            currentLevel.loadLevel(window, state, false);
        }

        break;
    case 2:
        optionsButton.setFillColor(sf::Color::White);
        optionsButtonText.setFillColor(sf::Color::Black);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            OptionsMenu(window, font, mainMenuBackgroundSprite);

            startButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2);
            loadGameButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 35);
            optionsButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 70);
            exitButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 105);
            versionID.setPosition(5, screenHeight - 40);

            startGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2);
            loadGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 35);
            optionsButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 70);
            exitGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 105);
        }

        break;
    case 3:
        exitGameButton.setFillColor(sf::Color::White);
        exitButtonText.setFillColor(sf::Color::Black);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            exit(1);

        break;
    default:
        break;
    }
    sf::sleep(sf::milliseconds(50));
}

void OptionsMenu(sf::RenderWindow& window, sf::Font font, sf::Sprite& menuBackgroundSprite)
{
    std::ofstream userSettingsData;
    userSettingsData.open("Data/UserData/UserSettings.dat", std::ofstream::trunc);

    int choice = 0;
    int current_tab = 0;

    sf::RectangleShape setRes_640_480_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_800_600_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1024_600_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1280_720_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1600_900_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1920_1080_Button(sf::Vector2f(120, 30));
    sf::RectangleShape decreaseDD(sf::Vector2f(30, 30));
    sf::RectangleShape increaseDD(sf::Vector2f(30, 30));
    sf::RectangleShape decrease_audio_volume(sf::Vector2f(30, 30));
    sf::RectangleShape increase_audio_volume(sf::Vector2f(30, 30));
    sf::RectangleShape audio_tab(sf::Vector2f(60, 30));
    sf::RectangleShape video_tab(sf::Vector2f(60, 30));

    sf::Text gameResSectionText;
    sf::Text ResText_640_480;
    sf::Text ResText_800_600;
    sf::Text ResText_1024_600;
    sf::Text ResText_1280_720;
    sf::Text ResText_1600_900;
    sf::Text ResText_1920_1080;
    sf::Text drawDistanceValue;
    sf::Text audio_volume_value;
    sf::Text video_tab_text;
    sf::Text audio_tab_text;
    sf::Text plus_text;
    sf::Text minus_text;

    // text properties

    // setting text font
    ResText_640_480.setFont(font);
    ResText_800_600.setFont(font);
    ResText_1024_600.setFont(font);
    ResText_1280_720.setFont(font);
    ResText_1600_900.setFont(font);
    ResText_1920_1080.setFont(font);
    gameResSectionText.setFont(font);
    drawDistanceValue.setFont(font);
    video_tab_text.setFont(font);
    audio_tab_text.setFont(font);
    audio_volume_value.setFont(font);

    //setting text character size
    ResText_640_480.setCharacterSize(20);
    ResText_800_600.setCharacterSize(20);
    ResText_1024_600.setCharacterSize(20);
    ResText_1280_720.setCharacterSize(20);
    ResText_1600_900.setCharacterSize(20);
    ResText_1920_1080.setCharacterSize(20);
    gameResSectionText.setCharacterSize(20);
    drawDistanceValue.setCharacterSize(20);
    video_tab_text.setCharacterSize(20);
    audio_tab_text.setCharacterSize(20);
    audio_volume_value.setCharacterSize(20);

    //setting text string
    ResText_640_480.setString("640x480");
    ResText_800_600.setString("800x600");
    ResText_1024_600.setString("1024x600");
    ResText_1280_720.setString("1280x720");
    ResText_1600_900.setString("1600x900");
    ResText_1920_1080.setString("1920x1080");
    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
    drawDistanceValue.setString("Draw distance " + std::to_string(RENDER_DISTANCE));
    audio_volume_value.setString("Audio volume level " + std::to_string(master_sound_volume));
    video_tab_text.setString("Video");
    audio_tab_text.setString("Audio");

    //setting text color
    ResText_640_480.setFillColor(sf::Color::White);
    ResText_800_600.setFillColor(sf::Color::White);
    ResText_1024_600.setFillColor(sf::Color::White);
    ResText_1280_720.setFillColor(sf::Color::White);
    ResText_1600_900.setFillColor(sf::Color::White);
    ResText_1920_1080.setFillColor(sf::Color::White);
    gameResSectionText.setFillColor(sf::Color::White);
    drawDistanceValue.setFillColor(sf::Color::White);
    audio_volume_value.setFillColor(sf::Color::White);

    // setting text position
    gameResSectionText.setPosition(5, 35);
    ResText_640_480.setPosition(5, 60);
    ResText_800_600.setPosition(5, 95);
    ResText_1024_600.setPosition(5, 130);
    ResText_1280_720.setPosition(5, 165);
    ResText_1600_900.setPosition(5, 200);
    ResText_1920_1080.setPosition(5, 235);
    drawDistanceValue.setPosition(5, 270);
    video_tab_text.setPosition(70, 5);
    audio_tab_text.setPosition(5, 5);
    audio_volume_value.setPosition(5, 40);

    // button properties

    // setting button color
    setRes_640_480_Button.setFillColor(sf::Color::Black);
    setRes_800_600_Button.setFillColor(sf::Color::Black);
    setRes_1024_600_Button.setFillColor(sf::Color::Black);
    setRes_1280_720_Button.setFillColor(sf::Color::Black);
    setRes_1600_900_Button.setFillColor(sf::Color::Black);
    setRes_1920_1080_Button.setFillColor(sf::Color::Black);
    decreaseDD.setFillColor(sf::Color::Black);
    increaseDD.setFillColor(sf::Color::Black);
    decrease_audio_volume.setFillColor(sf::Color::Black);
    increase_audio_volume.setFillColor(sf::Color::Black);

    // setting button position
    setRes_640_480_Button.setPosition(sf::Vector2f(5, 60));
    setRes_800_600_Button.setPosition(sf::Vector2f(5, 95));
    setRes_1024_600_Button.setPosition(sf::Vector2f(5, 130));
    setRes_1280_720_Button.setPosition(sf::Vector2f(5, 165));
    setRes_1600_900_Button.setPosition(sf::Vector2f(5, 200));
    setRes_1920_1080_Button.setPosition(sf::Vector2f(5, 235));
    decreaseDD.setPosition(sf::Vector2f(5, 300));
    increaseDD.setPosition(sf::Vector2f(50, 300));
    video_tab.setPosition(sf::Vector2f(70,5));
    audio_tab.setPosition(sf::Vector2f(5, 5));
    decrease_audio_volume.setPosition(sf::Vector2f(5, 70));
    increase_audio_volume.setPosition(sf::Vector2f(35, 70));

    while (true)
    {
        window.clear();

        window.draw(menuBackgroundSprite);

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && choice > 0)
            choice--;

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && choice < 6)
            choice++;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && current_tab > 0)
            current_tab--;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && current_tab < 1)
            current_tab++;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            WriteToUserSettings(userSettingsData);
            break;
        }

        sf::sleep(sf::milliseconds(100));

        switch (current_tab)
        {
        case 0:
        {
            window.draw(audio_tab_text);
            window.draw(video_tab_text);
            window.draw(audio_tab);
            window.draw(video_tab);
            window.draw(audio_volume_value);
            window.draw(decrease_audio_volume);
            window.draw(increase_audio_volume);

            audio_tab.setFillColor(sf::Color::White);
            video_tab.setFillColor(sf::Color::Black);

            audio_tab_text.setFillColor(sf::Color::Black);
            video_tab_text.setFillColor(sf::Color::White);
            decrease_audio_volume.setFillColor(sf::Color::White);
            increase_audio_volume.setFillColor(sf::Color::White);

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && master_sound_volume > 0)
            {
                decrease_audio_volume.setFillColor(sf::Color::Black);
                increase_audio_volume.setFillColor(sf::Color::White);
                master_sound_volume--;

                audio_volume_value.setString("Audio volume level " + std::to_string(master_sound_volume));
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && master_sound_volume < 100)
            {
                decrease_audio_volume.setFillColor(sf::Color::White);
                increase_audio_volume.setFillColor(sf::Color::Black);
                master_sound_volume++;

                audio_volume_value.setString("Audio volume level " + std::to_string(master_sound_volume));
            }

            break;
        }

        case 1:
        {
            window.draw(gameResSectionText);

            window.draw(setRes_640_480_Button);
            window.draw(setRes_800_600_Button);
            window.draw(setRes_1024_600_Button);
            window.draw(setRes_1280_720_Button);
            window.draw(setRes_1600_900_Button);
            window.draw(setRes_1920_1080_Button);
            window.draw(audio_tab);
            window.draw(video_tab);
            window.draw(audio_tab_text);
            window.draw(video_tab_text);

            window.draw(ResText_640_480);
            window.draw(ResText_800_600);
            window.draw(ResText_1024_600);
            window.draw(ResText_1280_720);
            window.draw(ResText_1600_900);
            window.draw(ResText_1920_1080);

            window.draw(drawDistanceValue);
            window.draw(decreaseDD);
            window.draw(increaseDD);

            setRes_640_480_Button.setFillColor(sf::Color::Black);
            setRes_800_600_Button.setFillColor(sf::Color::Black);
            setRes_1024_600_Button.setFillColor(sf::Color::Black);
            setRes_1280_720_Button.setFillColor(sf::Color::Black);
            setRes_1600_900_Button.setFillColor(sf::Color::Black);
            setRes_1920_1080_Button.setFillColor(sf::Color::Black);
            audio_tab.setFillColor(sf::Color::Black);
            video_tab.setFillColor(sf::Color::White);

            audio_tab_text.setFillColor(sf::Color::White);
            video_tab_text.setFillColor(sf::Color::Black);

            ResText_640_480.setFillColor(sf::Color::White);
            ResText_800_600.setFillColor(sf::Color::White);
            ResText_1024_600.setFillColor(sf::Color::White);
            ResText_1280_720.setFillColor(sf::Color::White);
            ResText_1600_900.setFillColor(sf::Color::White);
            ResText_1920_1080.setFillColor(sf::Color::White);

            decreaseDD.setFillColor(sf::Color::Black);
            increaseDD.setFillColor(sf::Color::Black);

            switch (choice)
            {
            case 0:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 640 && screenHeight != 480)
                {
                    screenWidth = 640;
                    screenHeight = 480;

                    menuSpriteScaleX = 1.0;
                    menuSpriteScaleY = 0.75;

                    window.close();
                    window.create(sf::VideoMode(640, 480), "Backrooms");

                    menuBackgroundSprite.setScale(sf::Vector2f(1.0, 0.75));
                    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
                }

                setRes_640_480_Button.setFillColor(sf::Color::White);
                ResText_640_480.setFillColor(sf::Color::Black);

                break;
            case 1:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 800 && screenHeight != 600)
                {
                    screenWidth = 800;
                    screenHeight = 600;

                    menuSpriteScaleX = 1.25;
                    menuSpriteScaleY = 1;

                    window.close();
                    window.create(sf::VideoMode(800, 600), "Backrooms");

                    menuBackgroundSprite.setScale(sf::Vector2f(1.25, 1));
                    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
                }

                setRes_800_600_Button.setFillColor(sf::Color::White);
                ResText_800_600.setFillColor(sf::Color::Black);

                break;
            case 2:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1024 && screenHeight != 600)
                {
                    screenWidth = 1024;
                    screenHeight = 600;

                    menuSpriteScaleX = 1.75;
                    menuSpriteScaleY = 1;

                    window.close();
                    window.create(sf::VideoMode(1024, 600), "Backrooms");

                    menuBackgroundSprite.setScale(sf::Vector2f(1.75, 1));
                    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
                }

                setRes_1024_600_Button.setFillColor(sf::Color::White);
                ResText_1024_600.setFillColor(sf::Color::Black);

                break;
            case 3:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1280 && screenHeight != 720)
                {
                    screenWidth = 1280;
                    screenHeight = 720;

                    menuSpriteScaleX = 1.95;
                    menuSpriteScaleY = 1.15;

                    window.close();
                    window.create(sf::VideoMode(1280, 720), "Backrooms");

                    menuBackgroundSprite.setScale(sf::Vector2f(1.95, 1.15));
                    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
                }

                setRes_1280_720_Button.setFillColor(sf::Color::White);
                ResText_1280_720.setFillColor(sf::Color::Black);

                break;
            case 4:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1600 && screenHeight != 900)
                {
                    screenWidth = 1600;
                    screenHeight = 900;

                    menuSpriteScaleX = 2.45;
                    menuSpriteScaleY = 1.5;

                    window.close();
                    window.create(sf::VideoMode(1600, 900), "Backrooms");

                    menuBackgroundSprite.setScale(sf::Vector2f(2.45, 1.50));
                    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
                }

                setRes_1600_900_Button.setFillColor(sf::Color::White);
                ResText_1600_900.setFillColor(sf::Color::Black);

                break;
            case 5:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1920 && screenHeight != 1080)
                {
                    screenWidth = 1920;
                    screenHeight = 1080;

                    menuSpriteScaleX = 2.85;
                    menuSpriteScaleY = 1.65;

                    window.close();
                    window.create(sf::VideoMode(1920, 1080), "Backrooms");

                    menuBackgroundSprite.setScale(sf::Vector2f(2.85, 1.65));
                    gameResSectionText.setString("Screen resolution (Current screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
                }

                setRes_1920_1080_Button.setFillColor(sf::Color::White);
                ResText_1920_1080.setFillColor(sf::Color::Black);

                break;
            case 6:
                decreaseDD.setFillColor(sf::Color::White);
                increaseDD.setFillColor(sf::Color::White);
                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && RENDER_DISTANCE > 8)
                {
                    decreaseDD.setFillColor(sf::Color::Black);
                    increaseDD.setFillColor(sf::Color::White);
                    RENDER_DISTANCE--;

                    drawDistanceValue.setString("Draw distance " + std::to_string(RENDER_DISTANCE));
                }

                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && RENDER_DISTANCE < 128)
                {
                    decreaseDD.setFillColor(sf::Color::White);
                    increaseDD.setFillColor(sf::Color::Black);
                    RENDER_DISTANCE++;

                    drawDistanceValue.setString("Draw distance " + std::to_string(RENDER_DISTANCE));
                }
                break;
            case 7:

                break;
            default:
                break;
            }

            break;
        }
        default:
            break;
        }

        window.display();
    }
}


void PauseMenu(sf::RenderWindow& window, sf::Font font, sf::Event event, sf::View view, sf::Vector2i& prevMousePosition, sf::Vector2i& windowCenter)
{
    using kb = sf::Keyboard;
    int choice = 0;

    sf::Text saveGameText;
    sf::Text returnToMenuText;
    sf::Text returnToGameText;

    sf::RectangleShape pauseMenuBackground(sf::Vector2f(screenWidth, screenHeight/2));
    sf::RectangleShape saveGameButton(sf::Vector2f(200, 40));
    sf::RectangleShape returnToMenuButton(sf::Vector2f(200, 40));
    sf::RectangleShape returnToGameButton(sf::Vector2f(200, 40));

    pauseMenuBackground.setPosition(0, screenHeight/3);
    pauseMenuBackground.setFillColor(sf::Color(150,150,150,100));

    saveGameButton.setPosition(screenWidth / 2.4, screenHeight / 2);
    returnToMenuButton.setPosition(screenWidth / 2.4, screenHeight / 2 + 45);
    returnToGameButton.setPosition(screenWidth / 2.4, screenHeight / 2 + 90);

    saveGameText.setFont(font);
    returnToMenuText.setFont(font);
    returnToGameText.setFont(font);

    saveGameText.setCharacterSize(20);
    returnToMenuText.setCharacterSize(20);
    returnToGameText.setCharacterSize(20);

    saveGameText.setString("Save game");
    returnToMenuText.setString("Return to main menu");
    returnToGameText.setString("Return to game");

    saveGameText.setPosition(screenWidth / 2.4, screenHeight / 2);
    returnToMenuText.setPosition(screenWidth / 2.4, screenHeight / 2 + 45);
    returnToGameText.setPosition(screenWidth / 2.4, screenHeight / 2 + 90);

    while (gameState == PAUSE)
    {
        if (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                hasFocus = false;
                break;
            case sf::Event::GainedFocus:
                hasFocus = true;
                break;
            case sf::Event::Resized:
                // update the view to the new size of the window
                view.setSize({
                                        static_cast<float>(event.size.width),
                                        static_cast<float>(event.size.height)
                    });
                window.setView(view);
                break;
            default:
                break;
            }
        }

        sf::sleep(sf::milliseconds(50));

        window.draw(saveGameButton);
        window.draw(returnToMenuButton);
        window.draw(returnToGameButton);

        window.draw(saveGameText);
        window.draw(returnToMenuText);
        window.draw(returnToGameText);    

        window.display();

        if (kb::isKeyPressed(kb::Up) && choice > 0)
            choice--;
        if (kb::isKeyPressed(kb::Down) && choice < 2)
            choice++;

        switch (choice)
        {
        case 0:
        {
            saveGameButton.setFillColor(sf::Color::White);
            returnToMenuButton.setFillColor(sf::Color::Black);
            returnToGameButton.setFillColor(sf::Color::Black);

            saveGameText.setFillColor(sf::Color::Black);
            returnToMenuText.setFillColor(sf::Color::White);
            returnToGameText.setFillColor(sf::Color::White);

            if (kb::isKeyPressed(kb::Enter))
                WriteSaveFile();

            break;
        }
        case 1:
        {
            saveGameButton.setFillColor(sf::Color::Black);
            returnToMenuButton.setFillColor(sf::Color::White);
            returnToGameButton.setFillColor(sf::Color::Black);

            saveGameText.setFillColor(sf::Color::White);
            returnToMenuText.setFillColor(sf::Color::Black);
            returnToGameText.setFillColor(sf::Color::White);

            if (kb::isKeyPressed(kb::Enter))
            {
                sf::sleep(sf::milliseconds(100));
                currentLevel.AmbientSFX.stop();
                currentLevel.AmbientSFX2.stop();
                currentLevel.machineSFX.stop();
                gameState = MAIN_MENU;
                window.setMouseCursorVisible(false);
            }

            break;
        }
        case 2:
        {
            saveGameButton.setFillColor(sf::Color::Black);
            returnToMenuButton.setFillColor(sf::Color::Black);
            returnToGameButton.setFillColor(sf::Color::White);

            saveGameText.setFillColor(sf::Color::White);
            returnToMenuText.setFillColor(sf::Color::White);
            returnToGameText.setFillColor(sf::Color::Black);

            if (kb::isKeyPressed(kb::Enter))
            {
                gameState = IN_GAME;
                window.setMouseCursorVisible(false); 
                sf::Mouse::setPosition(windowCenter, window); 
                prevMousePosition = windowCenter;

            }

            break;
        }
        default:
            break;
        }
    }
    
}

#endif