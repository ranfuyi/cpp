//
// Created by Chlzhong on 5/24/2024.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const sf::Color PURPLE(128, 0, 128);

class SortVisualizer {
private:
    std::vector<int> arr;
    sf::RenderWindow window;
    sf::Font font;
    std::string currentAlgorithm;
    int comparisons;
    int swaps;
    std::chrono::steady_clock::time_point startTime;
    bool sorting;
    bool graphicalMode;
    std::vector<int> initialArray;

    void drawBars(int highlight1 = -1, int highlight2 = -1) {
        window.clear(sf::Color::Black);

        float barWidth = static_cast<float>(WINDOW_WIDTH) / arr.size();
        float scaleFactor = static_cast<float>(WINDOW_HEIGHT - 100) / *std::max_element(arr.begin(), arr.end());

        for (size_t i = 0; i < arr.size(); ++i) {
            sf::RectangleShape bar(sf::Vector2f(barWidth - 1, arr[i] * scaleFactor));
            bar.setPosition(i * barWidth, WINDOW_HEIGHT - arr[i] * scaleFactor - 50);

            if (i == highlight1 || i == highlight2) {
                bar.setFillColor(sf::Color::Red);
            } else {
                bar.setFillColor(PURPLE);
            }

            window.draw(bar);

            sf::Text valueText;
            valueText.setFont(font);
            valueText.setString(std::to_string(arr[i]));
            valueText.setCharacterSize(12);
            valueText.setFillColor(sf::Color::White);
            valueText.setPosition(i * barWidth, WINDOW_HEIGHT - arr[i] * scaleFactor - 70);
            window.draw(valueText);
        }

        drawStats();
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Slow down the animation
    }

    void drawStats() {
        std::ostringstream stats;
        stats << "Algorithm: " << currentAlgorithm << "\n"
              << "Comparisons: " << comparisons << "\n"
              << "Swaps: " << swaps << "\n"
              << "Time: " << std::fixed << std::setprecision(2)
              << std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() << "s";

        sf::Text statsText;
        statsText.setFont(font);
        statsText.setString(stats.str());
        statsText.setCharacterSize(20);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(10, 10);
        window.draw(statsText);
    }

    void swap(int &a, int &b) {
        int temp = a;
        a = b;
        b = temp;
        swaps++;
    }

    void resetStats() {
        comparisons = 0;
        swaps = 0;
        startTime = std::chrono::steady_clock::now();
    }

    void displayTextArray(const std::vector<int> &array, const std::string &label) {
        std::cout << label << ": ";
        for (size_t i = 0; i < array.size(); ++i) {
            std::cout << std::setw(4) << array[i];
        }
        std::cout << std::endl;
    }

    void displayTextStats() {
        std::cout << "Algorithm: " << currentAlgorithm << " | "
                  << "Comparisons: " << comparisons << " | "
                  << "Swaps: " << swaps << " | "
                  << "Time: " << std::fixed << std::setprecision(2)
                  << std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() << "s\n";
    }

public:
    SortVisualizer() : sorting(false), graphicalMode(false) {
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading font" << std::endl;
        }
    }

    void setGraphicalMode(bool mode) {
        graphicalMode = mode;
        if (graphicalMode) {
            window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fixed Sorting Visualizer");
            window.setFramerateLimit(60);
        }
    }

    void generateRandomArray(int size) {
        arr.clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        for (int i = 0; i < size; ++i) {
            arr.push_back(dis(gen));
        }
        initialArray = arr;
    }

    void getUserInput(int size) {
        arr.clear();
        std::cout << "Enter " << size << " integers:\n";
        for (int i = 0; i < size; ++i) {
            int num;
            std::cin >> num;
            arr.push_back(num);
        }
        initialArray = arr;
    }

    void bubbleSort() {
        currentAlgorithm = "Bubble Sort";
        resetStats();
        sorting = true;

        if (!graphicalMode) {
            displayTextArray(initialArray, "Initial array");
        }

        for (size_t i = 0; i < arr.size() && sorting; ++i) {
            bool swapped = false;
            for (size_t j = 0; j < arr.size() - i - 1 && sorting; ++j) {
                comparisons++;
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
                if (graphicalMode) {
                    drawBars(j, j + 1);
                }
            }
            if (!graphicalMode) {
                displayTextArray(arr, "Current array");
                displayTextStats();
            }
            if (!swapped) break;
        }
        if (graphicalMode) {
            drawBars();
        } else {
            displayTextArray(arr, "Sorted array ");
            displayTextStats();
        }
        sorting = false;
    }

    void quickSort(int low, int high) {
        if (low < high && sorting) {
            int pi = partition(low, high);
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
    }

    int partition(int low, int high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1 && sorting; ++j) {
            comparisons++;
            if (arr[j] < pivot) {
                ++i;
                swap(arr[i], arr[j]);
            }
            if (graphicalMode) {
                drawBars(j, high);
            }
        }
        swap(arr[i + 1], arr[high]);
        if (!graphicalMode) {
            displayTextArray(arr, "Current array");
            displayTextStats();
        }
        return i + 1;
    }

    void runQuickSort() {
        currentAlgorithm = "Quick Sort";
        resetStats();
        sorting = true;

        if (!graphicalMode) {
            displayTextArray(initialArray, "Initial array");
        }

        quickSort(0, arr.size() - 1);

        if (graphicalMode) {
            drawBars();
        } else {
            displayTextArray(arr, "Sorted array ");
            displayTextStats();
        }
        sorting = false;
    }

    void selectionSort() {
        currentAlgorithm = "Selection Sort";
        resetStats();
        sorting = true;

        if (!graphicalMode) {
            displayTextArray(initialArray, "Initial array");
        }

        for (size_t i = 0; i < arr.size() - 1 && sorting; ++i) {
            size_t minIdx = i;
            for (size_t j = i + 1; j < arr.size() && sorting; ++j) {
                comparisons++;
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
                if (graphicalMode) {
                    drawBars(i, j);
                }
            }
            if (minIdx != i) {
                swap(arr[i], arr[minIdx]);
            }
            if (!graphicalMode) {
                displayTextArray(arr, "Current array");
                displayTextStats();
            }
        }
        if (graphicalMode) {
            drawBars();
        } else {
            displayTextArray(arr, "Sorted array ");
            displayTextStats();
        }
        sorting = false;
    }

    void shellSort() {
        currentAlgorithm = "Shell Sort";
        resetStats();
        sorting = true;

        if (!graphicalMode) {
            displayTextArray(initialArray, "Initial array");
        }

        for (int gap = arr.size() / 2; gap > 0 && sorting; gap /= 2) {
            for (int i = gap; i < arr.size() && sorting; i++) {
                int temp = arr[i];
                int j;
                for (j = i; j >= gap && arr[j - gap] > temp && sorting; j -= gap) {
                    comparisons++;
                    arr[j] = arr[j - gap];
                    swaps++;
                    if (graphicalMode) {
                        drawBars(j, j - gap);
                    }
                }
                arr[j] = temp;
            }
            if (!graphicalMode) {
                displayTextArray(arr, "Current array");
                displayTextStats();
            }
        }
        if (graphicalMode) {
            drawBars();
        } else {
            displayTextArray(arr, "Sorted array ");
            displayTextStats();
        }
        sorting = false;
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    sorting = false;
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
    }

    void drawMenu() {
        sf::Text menuText;
        menuText.setFont(font);
        menuText.setCharacterSize(20);
        menuText.setFillColor(sf::Color::White);
        menuText.setPosition(10, WINDOW_HEIGHT - 40);
        menuText.setString("1: Bubble | 2: Quick | 3: Selection | 4: Shell | 5: Reset | ESC: Exit");
        window.draw(menuText);
    }

    void run() {
        if (!graphicalMode) return;

        while (window.isOpen()) {
            handleEvents();

            if (!sorting) {
                window.clear(sf::Color::Black);
                drawBars();
                drawMenu();
                window.display();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) bubbleSort();
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) runQuickSort();
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) selectionSort();
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) shellSort();
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
                    arr = initialArray;
                    drawBars();
                }
            }
        }
    }

    void reset() {
        arr = initialArray;
    }
};

int main() {
    SortVisualizer visualizer;

    std::cout << "Select display mode:" << std::endl;
    std::cout << "1 - Graphical mode" << std::endl;
    std::cout << "2 - Text mode" << std::endl;
    int displayMode;
    std::cin >> displayMode;

    visualizer.setGraphicalMode(displayMode == 1);

    std::cout << "Select data input method:" << std::endl;
    std::cout << "1 - Random 20 numbers" << std::endl;
    std::cout << "2 - User-defined 10 numbers" << std::endl;
    int inputMethod;
    std::cin >> inputMethod;

    if (inputMethod == 1) {
        visualizer.generateRandomArray(20);
    } else {
        visualizer.getUserInput(10);
    }

    if (displayMode == 1) {
        visualizer.run();
    } else {
        int choice;
        do {
            std::cout << "\nSelect sorting algorithm:" << std::endl;
            std::cout << "1 - Bubble Sort" << std::endl;
            std::cout << "2 - Quick Sort" << std::endl;
            std::cout << "3 - Selection Sort" << std::endl;
            std::cout << "4 - Shell Sort" << std::endl;
            std::cout << "5 - Reset array" << std::endl;
            std::cout << "0 - Exit program" << std::endl;
            std::cin >> choice;

            switch (choice) {
                case 1: visualizer.bubbleSort(); break;
                case 2: visualizer.runQuickSort(); break;
                case 3: visualizer.selectionSort(); break;
                case 4: visualizer.shellSort(); break;
                case 5: visualizer.reset(); std::cout << "Array reset to initial state." << std::endl; break;
                case 0: std::cout << "Exiting program." << std::endl; break;
                default: std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } while (choice != 0);
    }

    return 0;
}
