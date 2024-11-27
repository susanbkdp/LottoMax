#include "LottoMax.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor
LottoMax::LottoMax() {
    // Seed for random number generation
    srand(static_cast<unsigned int>(time(0)));
    numLines = 0;
    numEncores = 0;
    totalCost = 0;
    prizeAmount = 0;
}

// Displays the rules of Lotto Max
void LottoMax::displayRules() const {
    cout << "\nLotto Max Rules:" << endl;
    cout << " - Each ticket line costs 5 CAD and consists of 7 unique numbers between 1 and 50." << endl;
    cout << " - Optional Encore costs an additional 1 CAD per Encore." << endl;
    cout << " - Every ticket includes at least one Encore line automatically." << endl;
    cout << " - If you play Encore, you get two Encore lines total." << endl;
    cout << " - Check the official OLG website for winning numbers and jackpot amounts.\n" << endl;
}

// Generates a random number between min and max
int LottoMax::generateRandomNumber(int min, int max) const {
    return rand() % (max - min + 1) + min;
}

// Generates ticket lines with 7 unique random numbers each
void LottoMax::generateLines(int numLines, vector<vector<int>>& lines, const string& lineType) {
    for (int i = 0; i < numLines; ++i) {
        vector<int> line;
        while (line.size() < 7) {
            int num = generateRandomNumber(1, 50);
            if (find(line.begin(), line.end(), num) == line.end()) {
                line.push_back(num);
            }
        }
        // Store the generated line
        lines.push_back(line);
        cout << lineType << " line generated: ";
        for (int num : line) {
            cout << num << " ";
        }
        cout << endl;
    }
}

// Clears ticket lines and generates new ones
void LottoMax::generateTicketLines() {
    // Clear previous ticket lines
    ticketLines.clear();
    // Generate new ticket lines
    generateLines(numLines, ticketLines, "Ticket");
}

// Generates the Encore lines
void LottoMax::generateEncoreLines() {
    encoreLines.clear();

    // Automatic Encore line (always included)
    cout << "Generating automatic Encore line...\n";
    generateLines(1, encoreLines, "Automatic Encore");

    // Optional additional Encore line 
    if (numEncores > 0) {
        cout << "Generating additional Encore line...\n";
        generateLines(1, encoreLines, "Additional Encore");
    }

    // Display all Encore lines in the console
    cout << "Encore Lines:\n";
    for (size_t i = 0; i < encoreLines.size(); ++i) {
        cout << "Encore Line " << (i + 1) << ": ";
        for (int num : encoreLines[i]) {
            cout << num << " ";
        }
        cout << endl;
    }
}

// Loads the winning line from a file
void LottoMax::generateWinningLine() {
    string fileName = "winning_numbers.txt";
    loadWinningLineFromFile(fileName);
    if (winningLine.empty()) {
        cout << "Invalid or missing winning numbers file. Generating random winning numbers." << endl;
        while (winningLine.size() < 7) {
            int num = generateRandomNumber(1, 50);
            if (find(winningLine.begin(), winningLine.end(), num) == winningLine.end()) {
                winningLine.push_back(num);
            }
        }
    }
    else {
        cout << "Winning numbers loaded successfully from file: " << fileName << "\n";
    }
}

// Loads the winning line
void LottoMax::loadWinningLineFromFile(const string& fileName) {
    ifstream inFile(fileName);
    if (inFile) {
        winningLine.clear();
        int num;
        for (int i = 0; i < 7 && inFile >> num; ++i) {
            winningLine.push_back(num);
        }
    }
    else {
        cout << "Error: Winning numbers file not found. Generating random winning numbers." << endl;
    }
}

// Calculates total winnings
int LottoMax::calculateWinnings() const {
    int totalWinnings = 0;
    for (const auto& line : ticketLines) {
        int lineWinnings = calculatePrize(line);
        totalWinnings += lineWinnings;
        if (lineWinnings > 0) {
            int matches = count_if(line.begin(), line.end(), [this](int num) {
                return find(winningLine.begin(), winningLine.end(), num) != winningLine.end();
                });
            cout << "Line: ";
            for (int num : line) {
                cout << num << " ";
            }
            cout << " - Matches: " << matches << " - Winnings: " << lineWinnings << " CAD\n";
        }
    }
    return totalWinnings;
}

// Determines prize
int LottoMax::calculatePrize(const vector<int>& line) const {
    static const vector<pair<int, int>> prizeTable = {
        {7, 75000000},
        {6, 4524},
        {5, 102},
        {4, 20},
        {3, 0}
    };

    int mainMatches = count_if(line.begin(), line.end(), [this](int num) {
        return find(winningLine.begin(), winningLine.end(), num) != winningLine.end();
        });

    bool encoreMatch = false;
    for (const auto& encoreLine : encoreLines) {
        if (any_of(line.begin(), line.end(), [&encoreLine](int num) {
            return find(encoreLine.begin(), encoreLine.end(), num) != encoreLine.end();
            })) {
            encoreMatch = true;
            break;
        }
    }

    // Check if the line matches prize criteria
    for (const auto& prizeEntry : prizeTable) {
        int matches = prizeEntry.first;
        int prize = prizeEntry.second;
        if (mainMatches == matches) {
            if (mainMatches == 6 && encoreMatch) return 321264;
            if (mainMatches == 5 && encoreMatch) return 1004;
            if (mainMatches == 4 && encoreMatch) return 51;
            if (mainMatches == 3 && encoreMatch) return 20;
            return prize;
        }
    }

    return 0;
}

// Generates a unique file name with the current timestamp
string LottoMax::generateUniqueFileName(const string& prefix) const {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    tm timeInfo;
    localtime_s(&timeInfo, &in_time_t);

    stringstream ss;
    ss << prefix << "_" << put_time(&timeInfo, "%Y%m%d_%H%M%S") << ".txt";
    return ss.str();
}

// Saves the ticket details
void LottoMax::saveTicketToFile(const string& fileName) const {
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Error creating file." << endl;
        return;
    }

    outFile << "Lotto Max Ticket\n";
    outFile << "-------------------\n";
    for (size_t i = 0; i < ticketLines.size(); ++i) {
        outFile << "Line " << (i + 1) << ": ";
        for (int num : ticketLines[i]) {
            outFile << num << " ";
        }
        outFile << "\n";
    }
    outFile << "Number of Encores: " << numEncores << "\n";
    outFile << "\nEncore Lines:\n";
    for (size_t i = 0; i < encoreLines.size(); ++i) {
        outFile << "Encore Line " << (i + 1) << ": ";
        for (int num : encoreLines[i]) {
            outFile << num << " ";
        }
        outFile << "\n";
    }
    outFile << "Total Cost: " << totalCost << " CAD\n";
    outFile << "Thank you for playing Lotto Max!\n";

    outFile.close();
    cout << "Ticket saved to " << fileName << endl;
}

// Saves the results to a file
void LottoMax::saveResultsToFile(const string& fileName) const {
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Error creating results file." << endl;
        return;
    }

    // Header and winning numbers
    outFile << "Lotto Max Results\n";
    outFile << "-------------------\n";
    outFile << "Winning Numbers: ";
    for (int num : winningLine) {
        outFile << num << " ";
    }
    outFile << "\n";
    outFile << "Jackpot: 75 Million CAD\n\n";

    // Results for each ticket line
    outFile << "Ticket Results:\n";
    for (size_t i = 0; i < ticketLines.size(); ++i) {
        const auto& line = ticketLines[i];
        int matches = count_if(line.begin(), line.end(), [this](int num) {
            return find(winningLine.begin(), winningLine.end(), num) != winningLine.end();
            });
        int lineWinnings = calculatePrize(line);

        outFile << "Line " << (i + 1) << ": ";
        for (int num : line) {
            outFile << num << " ";
        }
        outFile << "- Matches: " << matches << " - Winnings: " << lineWinnings << " CAD\n";
    }

    // Encore results
    outFile << "\nEncore Lines:\n";
    for (size_t i = 0; i < encoreLines.size(); ++i) {
        outFile << "Encore Line " << (i + 1) << ": ";
        for (int num : encoreLines[i]) {
            outFile << num << " ";
        }
        outFile << "\n";
    }

    outFile.close();
    cout << "Results saved to " << fileName << endl;
}