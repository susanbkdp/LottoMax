// main.cpp
#include <iostream>
#include <algorithm>
#include <limits>
#include "LottoMax.h"

using namespace std;

int getValidatedInput(const string& prompt, int min, int max) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        cin >> input;

        // Check if input is numeric and not negative
        if (!input.empty() && input[0] == '-') {
            cout << "Invalid input. Please enter a positive integer between " << min << " and " << max << ".\n";
        }
        else if (all_of(input.begin(), input.end(), ::isdigit)) {
            value = stoi(input);
            // Check if value is within range
            if (value >= min && value <= max) {
                return value;
            }
        }

        // Error for invalid input
        cout << "Invalid input. Please enter an integer between " << min << " and " << max << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void playLottoMax() {
    LottoMax lotto;
    char playAgain;

    do {
        // Display rules
        lotto.displayRules();

        // Get number of lines to play (1-30)
        int numLines = getValidatedInput("How many lines would you like to play? (1-30): ", 1, 30);

        // Get Encore option (0 or 1)
        int playEncore = getValidatedInput("Would you like to play Encore? (0 = No, 1 = Yes): ", 0, 1);

        // Set up the LottoMax game using setters
        lotto.setNumLines(numLines);
        lotto.setNumEncores(playEncore);
        lotto.setTotalCost(numLines * 5 + playEncore);

        // Generate ticket lines, bonus lines, and Encore lines if applicable
        lotto.generateTicketLines();
        lotto.generateEncoreLines();
        lotto.generateWinningLine();

        // Display total cost
        cout << "Total cost: " << lotto.getTotalCost() << " CAD\n";

        // Save the ticket to a file
        string ticketFileName = lotto.generateUniqueFileName("ticket");
        lotto.saveTicketToFile(ticketFileName);

        // Save the winning results to a file
        string resultsFileName = lotto.generateUniqueFileName("results");
        lotto.saveResultsToFile(resultsFileName);

        // Display the winning numbers in the console
        cout << "\nWinning Numbers: ";
        for (int num : lotto.getWinningLine()) {
            cout << num << " ";
        }

        // Calculate and display winnings
        int winnings = lotto.calculateWinnings();
        if (winnings > 0) {
            cout << "Congratulations! You have won " << winnings << " CAD!\n";
        }
        else {
            cout << "Unfortunately, you did not win this time. Better luck next time!\n";
        }

        // Ask user if they want to play again
        cout << "\nWould you like to play again? (y/n): ";
        cin >> playAgain;
        while (playAgain != 'y' && playAgain != 'Y' && playAgain != 'n' && playAgain != 'N') {
            cout << "Invalid input. Please enter 'y' for yes or 'n' for no: ";
            cin >> playAgain;
        }
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\nThank you for playing Lotto Max! Good luck!\n";
}

int main() {
    cout << "Welcome to Lotto Max!\n";
    playLottoMax();
    return 0;
}