#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cctype>  // для isspace
#include <numeric>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define WHITE "\033[37m"


using namespace std;

enum class Suit {
    Spades,   // ♠
    Hearts,   // ♥
    Diamonds, // ♦
    Clubs     // ♣
};

string suitToString(Suit suit) {
    switch (suit) {
    case Suit::Spades:   return string(WHITE) + "♠" + RESET;
    case Suit::Hearts:   return string(RED) + "♥" + RESET;
    case Suit::Diamonds: return string(RED) + "♦" + RESET;
    case Suit::Clubs:    return string(WHITE) + "♣" + RESET;
    default:             return "?";
    }
}




/* ------------------------------------------------------------------------------------ */
// CARD
/* ------------------------------------------------------------------------------------ */
struct Card {
    Suit suit;
    string value;

    string toString() const {
        return value + suitToString(suit);
    }

    bool operator==(const Card& other) const {
        return suit == other.suit && value == other.value;
    }
};

Card toCard(string& stringCard)
{
    Suit suit;
    string value;
    if (stringCard.find("♠") != string::npos) {
        suit = Suit::Spades;
    }
    else if (stringCard.find("♥") != string::npos) {
        suit = Suit::Hearts;
    }
    else if (stringCard.find("♦") != string::npos) {
        suit = Suit::Diamonds;
    }
    else if (stringCard.find("♣") != string::npos) {
        suit = Suit::Clubs;
    }
    else {
        throw invalid_argument("Invalid card suit.");
    }
    value = stringCard.substr(0, stringCard.find_first_of("♠♥♦♣"));
    return { suit, value };
}

int cardValueRank(const string& value) {
    if (value == "2") return 2;
    if (value == "3") return 3;
    if (value == "4") return 4;
    if (value == "5") return 5;
    if (value == "6") return 6;
    if (value == "7") return 7;
    if (value == "8") return 8;
    if (value == "9") return 9;
    if (value == "10") return 10;
    if (value == "J") return 11;
    if (value == "Q") return 12;
    if (value == "K") return 13;
    if (value == "A") return 14;
    return 0; // for invalid or unknown values
}

string rankToCardValue(int rank) {
    switch (rank) {
    case 2: return "2";
    case 3: return "3";
    case 4: return "4";
    case 5: return "5";
    case 6: return "6";
    case 7: return "7";
    case 8: return "8";
    case 9: return "9";
    case 10: return "10";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    case 14: return "A";
    default: return "?"; // for invalid or unknown ranks
    }
}
/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */


string strip(string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    if (start == s.size()) {
        return ""; 
    }
    size_t end = s.size() - 1;
    while (end > start && isspace(static_cast<unsigned char>(s[end]))) {
        --end;
    }
    return s.substr(start, end - start + 1);
}

bool isValidIndicesLine(string& line) {
    for (char ch : line) {
        if (!isdigit(static_cast<unsigned char>(ch)) && !isspace(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

class PokerGame {
private:
    vector<string> values = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    vector<Suit> suits = { Suit::Hearts, Suit::Diamonds, Suit::Clubs, Suit::Spades };
    vector<Card> deck;
    vector<Card> playerHand;
    vector<Card> dealerHand;
    int pot = 0;
    int maxReplacements = 3;
    int replacementsDone = 0;

public:
    PokerGame() {
        srand(static_cast<unsigned>(time(0)));
        createDeck();
        shuffleDeck();
    }

    int getMaxReplacements() { return maxReplacements; }
	int getReplacementsDone() { return replacementsDone; }

    void createDeck() {
        deck.clear();
        for (const auto& value : values) {
            for (const auto& suit : suits) {
                deck.push_back({ suit, value });
            }
        }
    }

    void shuffleDeck() {
        random_shuffle(deck.begin(), deck.end());
    }

    void dealCards(vector<Card>& hand, int count = 2) {
        for (int i = 0; i < count; ++i) {
            hand.push_back(deck.back());
            deck.pop_back();
        }
    }

    void showHand(const vector<Card>& hand, const string& owner) {
        cout << owner << "'s Hand: ";
        for (const auto& card : hand) {
            cout << card.toString() << " ";
        }
        cout << endl;
    }

    void askBet() {
        int bet;
        while (true)
        {
			cout << "Enter your bet: ";
			cin >> bet;
            if (cin.fail())
            {
				cin.clear(); // clear the error flag
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
				cout << RED << "Invalid input. Please enter a valid bet amount: " << RESET << endl;
				continue;
            }

			if (bet < 0)
			{
				cout << RED << "Bet must be a positive number. Please enter a valid bet amount: " << RESET << endl;
				continue;
			}
             
	        break;
        }
        pot = bet * 2; // player + dealer (simplified)
    }

    int evaluateHand(const vector<Card>& hand) {
		const int totalSum = accumulate(hand.begin(), hand.end(), 0, [](int sum, const Card& card) {
			return sum + cardValueRank(card.value);
			});
        return totalSum;
    }

    void determineWinner() {
        int playerScore = evaluateHand(playerHand);
        int dealerScore = evaluateHand(dealerHand);

        cout << "Player score: ";
        for (size_t i = 0; i < playerHand.size(); ++i) {
            cout << playerHand[i].toString();
            if (i < playerHand.size() - 1)
                cout << " + ";
            else
                cout << " = ";
        }
        cout << playerScore << endl;
        cout << "Dealer score: ";
        for (size_t i = 0; i < dealerHand.size(); ++i) {
            cout << dealerHand[i].toString();
            if (i < dealerHand.size() - 1)
                cout << " + ";
            else
                cout << " = ";
        }
        cout<< dealerScore << endl;

        if (playerScore > dealerScore) {
            cout << "Player wins! Gains $" << pot << endl;
        }
        else if (dealerScore > playerScore) {
            cout << "Dealer wins!" << endl;
        }
        else {
            cout << "It's a tie! Pot is split: $" << pot / 2 << " each." << endl;
        }
    }

    void play() {
        dealCards(playerHand);
        dealCards(dealerHand);
        showHand(playerHand, "Player");
        if (maxReplacements <= 0 || replacementsDone >= 3)
        {
			cout << "You cannot replace cards." << endl;
		}
		else
		{
			replaceCards(playerHand, deck);
        }

        askBet();

        showHand(dealerHand, "Dealer");
        determineWinner();
    }

    void replaceCards(vector<Card>& playerHand, vector<Card>& deck) {
        

        while (replacementsDone < maxReplacements) {
            cout << "Ви можете замінити карту(и). Введіть індекси карт через пробіл (0 щоб пропустити): ";

            string line;
            getline(cin, line);
			line = strip(line);

			if (!isValidIndicesLine(line))
			{
				cout << RED << "Invalid input. Please enter valid card indices: " << RESET << endl;
				continue;
			}

            if (line == "0" || line.empty()) {
                // Користувач не хоче міняти
                break;
            }

            // Парсимо індекси з рядка
            vector<int> indices;
            size_t pos = 0;
            while ((pos = line.find(' ')) != string::npos) {
                string token = line.substr(0, pos);
                indices.push_back(stoi(token));
                line.erase(0, pos + 1);
            }
            if (!line.empty()) {
                indices.push_back(stoi(line));
            }

            // Перевірка індексів
            bool valid = true;
            for (int idx : indices) {
                if (idx < 1 || idx >(int)playerHand.size()) {
                    valid = false;
                    break;
                }
            }
            if (!valid) {
                cout << "Некоректний індекс карт. Спробуйте ще раз." << endl;
                continue;
            }

            // Заміна карт
            for (int idx : indices) {
                if (deck.empty()) {
                    cout << "Колода порожня, більше карт немає." << endl;
                    break;
                }
                // Заміна карти на нову з колоди
                playerHand[idx - 1] = deck.back();
                deck.pop_back();
                ++replacementsDone;
                cout << "Ваші карти після заміни:" << endl;
                showHand(playerHand, "Player");
                if (replacementsDone >= maxReplacements) break;
            }

            cout << "Ви зробили " << replacementsDone << " замін(и) з 3 можливих." << endl;
            if (replacementsDone >= maxReplacements) {
                cout << "Ви вичерпали всі заміни." << endl;
                break;
            }
        }
    }
};

int main() {
    system("chcp 65001 > nul");
    srand(time(0));
    char playAgain = 'y';
    PokerGame game;
    while (playAgain == 'y') {
        
        game.play();
        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline character from the input buffer
    }
    return 0;
}


