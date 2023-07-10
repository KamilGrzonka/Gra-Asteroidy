#include "MainHeader.h"
namespace fs = std::filesystem;

Menu::Menu(Font& font) {
    

    titleText.setString("Gra Asteroidy");
    titleText.setFont(font);
    titleText.setCharacterSize(60);
    titleText.setFillColor(Color::White);

    playButton.setString("Rozpocznij gre");
    playButton.setFont(font);
    playButton.setCharacterSize(40);
    playButton.setFillColor(Color::White);

    instructionButton.setString("Instrukcja");
    instructionButton.setFont(font);
    instructionButton.setCharacterSize(40);
    instructionButton.setFillColor(Color::White);

    scoreboardButton.setString("Tablica Wynikow");
    scoreboardButton.setFont(font);
    scoreboardButton.setCharacterSize(40);
    scoreboardButton.setFillColor(Color::White);

    quitButton.setString("Wyjscie");
    quitButton.setFont(font);
    quitButton.setCharacterSize(40);
    quitButton.setFillColor(Color::White);

    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(600 - titleBounds.width / 2, 100);

    FloatRect playBounds = playButton.getLocalBounds();
    playButton.setPosition(600 - playBounds.width / 2, 250);

    FloatRect instructionsBounds = instructionButton.getLocalBounds();
    instructionButton.setPosition(600 - instructionsBounds.width / 2, 350);

    FloatRect scoreboardBounds = scoreboardButton.getLocalBounds();
    scoreboardButton.setPosition(600 - scoreboardBounds.width / 2, 450);

    FloatRect quitBounds = quitButton.getLocalBounds();
    quitButton.setPosition(600 - quitBounds.width / 2, 550);
}

void Menu::draw(RenderWindow& app) {
    Texture tm;
    tm.loadFromFile("images/Menu.png");
    Sprite sMenu(tm);
    app.draw(sMenu);
    app.draw(titleText);
    app.draw(playButton);
    app.draw(instructionButton);
    app.draw(scoreboardButton);
    app.draw(quitButton);
}

bool Menu::isPlayButtonPressed(const Vector2i& mousePosition) {
    return playButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition));
}

bool Menu::isInstructionButtonPressed(const Vector2i& mousePosition) {
    return instructionButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition));
}

bool Menu::isScoreBoardButtonPressed(const Vector2i& mousePosition) {
    return scoreboardButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition));
}

bool Menu::isQuitButtonPressed(const Vector2i& mousePosition) {
    return quitButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition));
}

Instrukcja::Instrukcja(const Font& font) : font(font), textureLoaded(false) {
    instructionText.setFont(font);
    instructionText.setCharacterSize(40);
    instructionText.setFillColor(Color::White);
    instructionText.setString("Instrukcja:\n\n"
        "Sterowanie:\n"
        "A - Obrot w lewo\n"
        "D - Obrot w prawo\n"
        "W - Naped\n"
        "S - Hamowanie\n"
        "Spacja - Strzal\n"
        "Esc - Wyjscie z gry\n\n"
        "Cel gry:\n"
        "Unikaj asteroid i zestrzel jak najwiecej z nich!");

    backText.setFont(font);
    backText.setCharacterSize(60);
    backText.setFillColor(Color::White);
    backText.setString("Powrot");

    FloatRect instructionBounds = instructionText.getLocalBounds();
    instructionText.setPosition(600 - instructionBounds.width / 2, 100);

    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(600 - backBounds.width / 2, 650);

    // Rozpoczêcie asynchronicznego ³adowania tekstury
    textureLoadingThread = std::thread(&Instrukcja::loadTextureAsync, this);
}
Instrukcja::~Instrukcja() {
    if (textureLoadingThread.joinable()) {
        textureLoadingThread.join();
    }
};
void Instrukcja::loadTextureAsync() {
    
    std::lock_guard<std::mutex> lock(textureMutex);
    // £adowanie tekstury
    if (instructionTexture.loadFromFile("images/Instrukcja.png")) {
        instructionSprite.setTexture(instructionTexture);
        textureLoaded = true;
    } else {
        std::cout << "B³¹d ³adowania tekstury!" << std::endl;
    }
}

void Instrukcja::draw(RenderWindow& app) {
    // Sprawdzenie, czy tekstura zosta³a za³adowana
    if (textureLoaded) {
        // Zabezpieczenie przed dostêpem do tekstury przez wiele w¹tków
        std::lock_guard<std::mutex> lock(textureMutex);
        app.draw(instructionSprite);
    }

    app.draw(instructionText);
    app.draw(backText);
}

bool Instrukcja::isBackClicked(const Vector2i& mousePosition) {
    return backText.getGlobalBounds().contains(Vector2f(mousePosition));
}

TablicaWynikow::TablicaWynikow(const Font& font) : font(font) {
    scoreboardText.setFont(font);
    scoreboardText.setCharacterSize(80);
    scoreboardText.setFillColor(Color::White);
    scoreboardText.setString("Tablica Wynikow");
    
    backText.setFont(font);
    backText.setCharacterSize(60);
    backText.setFillColor(Color::White);
    backText.setString("Powrot");

    FloatRect scoreboardBounds = scoreboardText.getLocalBounds();
    scoreboardText.setPosition(600 - scoreboardBounds.width / 2, 100);

    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(600 - backBounds.width / 2, 650);

    // Twórz nowe zadanie (packaged_task) do wczytania wyników
    std::packaged_task<void()> loadTask([this]() { WczytajWynik(); });
    std::future<void> future = loadTask.get_future();
    // Uruchamia zadanie asynchronicznie w oddzielnym w¹tku
    std::thread loadThread(std::move(loadTask));
    loadThread.detach();
}
void TablicaWynikow::WczytajWynik() {
    fs::path scoresFilePath("images/scores.txt");           //filesystem
    if (fs::exists(scoresFilePath) && fs::is_regular_file(scoresFilePath)) {
        std::packaged_task<std::string()> task([scoresFilePath]() {             //packaged_task
            std::ifstream scoresFile(scoresFilePath);
            std::string scoresTextString;

            if (scoresFile.is_open()) {
                std::string line;
                std::regex scoreRegex(R"((\w+):\s(\d+))");                      //regex

                while (std::getline(scoresFile, line)) {
                    std::smatch match;
                    if (std::regex_match(line, match, scoreRegex)) {
                        std::string name = match[1].str();
                        std::string score = match[2].str();
                        scoresTextString += name + ": " + score + "\n";
                    }
                }
                scoresFile.close();
            }

            return scoresTextString;
            });

        std::future<std::string> future = task.get_future();

        // Uruchom zadanie asynchronicznie w oddzielnym w¹tku
        std::thread taskThread(std::move(task));
        taskThread.detach();

        // Oczekuj na zakoñczenie zadania i zaktualizuj tekst
        std::string scoresTextString = future.get();

        scoresText.setFont(font);
        scoresText.setCharacterSize(40);
        scoresText.setFillColor(Color::White);
        scoresText.setString(scoresTextString);

        FloatRect scoresBounds = scoresText.getLocalBounds();
        scoresText.setPosition(600 - scoresBounds.width / 2, 250);
    }
}

void TablicaWynikow::draw(RenderWindow& app) {
    Texture tm;
    tm.loadFromFile("images/Instrukcja.png");
    Sprite sMenu(tm);
    app.draw(sMenu);
    app.draw(scoreboardText);
    app.draw(scoresText);
    app.draw(backText);
}

bool TablicaWynikow::isBackClicked(const Vector2i& mousePosition) {
    return backText.getGlobalBounds().contains(Vector2f(mousePosition));
}

