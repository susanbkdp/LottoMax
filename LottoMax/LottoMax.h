#pragma once
#ifndef LOTTOMAX_H
#define LOTTOMAX_H

#include <vector>
#include <string>

class LottoMax {
public:
    LottoMax();

    void playGame();
    void displayRules() const;
    void saveTicketToFile(const std::string& fileName) const;
    void saveResultsToFile(const std::string& fileName) const;
    int calculateWinnings() const;

    // Public setters and getters
    void setNumLines(int numLines) { this->numLines = numLines; }
    void setNumEncores(int numEncores) { this->numEncores = numEncores; }
    void setTotalCost(int totalCost) { this->totalCost = totalCost; }
    int getTotalCost() const { return totalCost; }
    std::vector<int> getWinningLine() const { return winningLine; }

    // Public functions for generating lines
    void generateTicketLines();
    void generateEncoreLines();
    void generateWinningLine();
    std::string generateUniqueFileName(const std::string& prefix) const;
    void loadWinningLineFromFile(const std::string& fileName);
    void generateLines(int numLines, std::vector<std::vector<int>>& lines, const std::string& lineType); // Add generateLines

private:
    std::vector<std::vector<int>> ticketLines;
    std::vector<std::vector<int>> encoreLines; // Declare encoreLines here
    std::vector<int> winningLine;
    int numLines;
    int numEncores;
    int totalCost;
    int prizeAmount;

    int generateRandomNumber(int min, int max) const;
    int calculatePrize(const std::vector<int>& line) const;
};

#endif