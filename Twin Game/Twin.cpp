#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <vector>
#include <conio.h>
#include <stdlib.h>
using namespace std;

enum class Signs
{
    spade = 1,
    heart,
    club,
    diamond,
};

enum class Ranks
{
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
};

struct Card
{
    Signs suit;
    Ranks rank;
    int value;
};

void printDeck(Card *deck, int size)
{
    for (int i = 0; i < size; ++i)
    {

        switch (deck[i].value)
        {
        case 1:
            cout << "[ "
                 << "A"
                 << " " << (char)deck[i].suit << " ]  ";
            break;
        case 11:
            cout << "[ "
                 << "J"
                 << " " << (char)deck[i].suit << " ]  ";
            break;
        case 12:
            cout << "[ "
                 << "Q"
                 << " " << (char)deck[i].suit << " ]  ";
            break;
        case 13:
            cout << "[ "
                 << "K"
                 << " " << (char)deck[i].suit << "]  ";
            break;
        default:
            cout << "[ " << deck[i].value << " " << (char)deck[i].suit << " ]  ";
        }
    }
}

void shuffleDeck(Card *deck, int size)
{
    std::shuffle(deck, deck + size, std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

void pickJoker(Card *deck, int &deckSize, Card &joker)
{
    // Choose a random card from the deck as a joker
    int chosen;
AskAgain:
    cout << "\nLet's choose the Joker\nEnter a random number from 1 to 52:";
    cin >> chosen;
    if ((chosen > 0) && (chosen < 52))
    {
        joker = deck[chosen - 1];

        // Remove the joker from the deck
        rotate(deck + chosen, deck + chosen + 1, deck + deckSize);
        --deckSize;
    }
    else
    {
        cout << "Please enter a valid number\n";

        goto AskAgain;
    }
}

void dealCardsToPlayers(Card *deck, Card *Hand, int &deckSize, int handSize)
{
    // Deal cards to the player
    for (int i = 0; i < handSize; ++i)
    {
        Hand[i] = deck[i];
    }

    // Remove dealt cards from the deck
    rotate(deck, deck + handSize, deck + deckSize);
    deckSize -= handSize;
}

void Sort(Card *hand, int size)
{
    Card temp[1];

    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {

            if (hand[i].value == hand[j].value)
            {
                temp[0] = hand[i + 1];
                hand[i + 1] = hand[j];
                hand[j] = temp[0];
            }
        }
    }
}

bool checkPairs(Card *hand, int handSize,int joker)
{
    
    int jokercount = 0;

    for (int i = 0; i < handSize; i++)
    { /// Counting jokers
        if (hand[i].value == joker)
        {
            jokercount++;
        }

        for (int i = 0; i < handSize; i++)
        {
            if (hand[i].value != joker)
            {
                switch (i)
                {
                case 0:
                    if (hand[i].value != hand[i + 1].value)
                    {
                        --jokercount;
                    }
                    break;
                case 5:
                    if (hand[i].value != hand[i - 1].value)
                    {
                        --jokercount;
                    }
                    break;

                default:
                    if ((hand[i].value != hand[i - 1].value) && (hand[i].value != hand[i + 1].value))
                    {
                        --jokercount;
                    }
                }
            }
        }

        if ((jokercount < 0))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

void printPileTop(vector<Card> &discardPile)
{
    if (!discardPile.empty())
    {
        switch (discardPile.back().value)
        {
        case 1:
            cout << "\nTop Card in Discard Pile: "
                 << "["
                 << "A"
                 << " "
                 << (char)discardPile.back().suit << "]";
            break;
        case 11:
            cout << "\nTop Card in Discard Pile: "
                 << "["
                 << "J"
                 << " "
                 << (char)discardPile.back().suit << "]";
            break;
        case 12:
            cout << "\nTop Card in Discard Pile: "
                 << "["
                 << "Q"
                 << " "
                 << (char)discardPile.back().suit << "]";
            break;
        case 13:
            cout << "\nTop Card in Discard Pile: "
                 << "["
                 << "K"
                 << " "
                 << (char)discardPile.back().suit << "]";
            break;
        default:
            cout << "\nTop Card in Discard Pile: "
                 << "[" << discardPile.back().value << " "
                 << (char)discardPile.back().suit << "]";
        }
    }
    else
    {
        cout << "Discard Pile is empty.\n";
    }
}

bool getPlayerDecision()
{
    char choice;
    cout << "\nDo you want to accept the card? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
        return true;

    else if (choice == 'n' || choice == 'N')
    {
        return false;
    }

    else
    {
        cout << "\nPlease enter right keywords!!";
        getPlayerDecision();
    }
}

bool chooseFromDeckOrPile()
{
    char choice;
    cout << "\nChoose from Deck (D) or Discard Pile (P): ";
    cin >> choice;
    if (choice == 'd' || choice == 'D')
    {
        return false;
    }
    else if (choice == 'p' || choice == 'P')
    {
        return true;
    }
    else
    {
        cout << "\nPlease make a proper choice!!";
        chooseFromDeckOrPile();
    }
}

void removeCardFromDeck(Card *deck, int &deckSize, const Card &assignedCard)
{
    // Find the position of the assigned card in the deck
    int cardIndex = -1;
    for (int i = 0; i < deckSize; ++i)
    {
        if (deck[i].value == assignedCard.value && deck[i].suit == assignedCard.suit)
        {
            cardIndex = i;
            break;
        }
    }

    // If the card is found, remove it from the deck
    if (cardIndex != -1)
    {
        // Move the cards after the assigned card one position to the left
        for (int i = cardIndex; i < deckSize - 1; ++i)
        {
            deck[i] = deck[i + 1];
        }

        // Decrease the deck size
        --deckSize;
    }
}

void pickMatchingCards(Card *deck, int &deckSize, Card *hand, vector<Card> &discardPile)
{
    // Pick a card from the deck for the player
    Card playerCard = deck[deckSize - 1];
    int handSize = 6;

    // Display the top card in the discard pile
    printPileTop(discardPile);

    // Allow the player to choose from deck or pile
    bool chooseFromPile = chooseFromDeckOrPile();

    // Show the player the card they chose
    if (chooseFromPile && !discardPile.empty())
    {
        switch (discardPile.back().value)
        {
        case 1:
            cout << "\nYou chose from Discard Pile: "
                 << "[ "
                 << "A"
                 << " "
                 << (char)discardPile.back().suit << " ]";
            break;
        case 11:
            cout << "\nYou chose from Discard Pile: "
                 << "[ "
                 << "J"
                 << " "
                 << (char)discardPile.back().suit << " ]";
            break;
        case 12:
            cout << "\nYou chose from Discard Pile: "
                 << "[ "
                 << "Q"
                 << " "
                 << (char)discardPile.back().suit << " ]";
            break;
        case 13:
            cout << "\nYou chose from Discard Pile: "
                 << "[ "
                 << "K"
                 << " "
                 << (char)discardPile.back().suit << " ]";
            break;
        default:
            cout << "\nYou chose from Discard Pile: " << discardPile.back().value << " "
                 << (char)discardPile.back().suit << " ]";
        }
        playerCard = discardPile.back();
        discardPile.pop_back();
    }
    else
    {
        cout << "\nYou chose from Deck: " << (int)playerCard.rank << " "
             << (char)playerCard.suit;
        removeCardFromDeck(deck, deckSize, playerCard);
    }

    // Ask if the player wants to keep the card
    bool acceptCard = getPlayerDecision();

    if (acceptCard)
    {
        // Add the card to the player's hand
        ++handSize;
        hand[handSize - 1] = playerCard;

        // Sort the player's hand
        Sort(hand, handSize);

        // Display the player's hand
        cout << "\nYour Hand:";
        printDeck(hand, handSize);

        // Ask which card to put in the discard pile
        int discardChoice;
    Dumb:
        cout << "\nChoose a card to put in the Discard Pile (1-" << handSize << "): ";
        cin >> discardChoice;

        // Put the chosen card in the discard pile
        if (discardChoice >= 1 && discardChoice <= handSize)
        {
            discardPile.push_back(hand[discardChoice - 1]);
            std::rotate(hand + discardChoice - 1, hand + discardChoice, hand + handSize);
            --handSize;
            cout << "\nYou put a card in the Discard Pile.";
        }
        else
        {
            cout << "\nInvalid choice. No card put in the Discard Pile.";
            goto Dumb;
        }
    }

    else
    {
        // Put the card in the discard pile
        discardPile.push_back(playerCard);
        cout << "\nYou put the card in the Discard Pile.";
    }
}

bool computerDecision(Card &cardFromPile, Card *computerHand, int computerHandSize, int joker)
{
    // Check if the card from the pile can form a pair in the computer's hand
    int count = 0;
    if (cardFromPile.value == joker)
    {
        return true;
    }
    else
    {
        for (int i = 0; i < computerHandSize; ++i)
        {
            if (cardFromPile.value == computerHand[i].value)
            {
                count++;
            }
        }

        if ((count > 0) && (count < 2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void computerTurn(Card *deck, int &deckSize, Card *computerHand, std::vector<Card> &discardPile,int joker)
{
   
    int computerHandSize = 6; 
    // Computer logic to decide if it wants to pick from the pile
    bool acceptCardFromPile = computerDecision(discardPile.back(), computerHand, computerHandSize, joker);
    // If the computer wants the card from the pile

    if (acceptCardFromPile)
    {
        // Add the top card from the discard pile to the computer's hand
        ++computerHandSize;
        computerHand[computerHandSize - 1] = discardPile.back();
        discardPile.pop_back();

        // Sort the computer's hand
        Sort(computerHand, computerHandSize);

        // Display the computer's hand
        cout << "\nComputer's Hand:";
        printDeck(computerHand, computerHandSize);

        cout << "\nComputer accepted the card from the Pile.";

        // Decide which card to put in the discard pile
        bool cardPutInPile = false;
        // Deciding to throw the card in the pile;
        for (int i = 0; i < computerHandSize; ++i)
        {
            if (computerHand[i].value != joker)
            {
                if (i == 0)
                {
                    if ((computerHand[i].value != computerHand[i + 1].value))
                    {
                        // The card hasn't formed a pair with any other card
                        discardPile.push_back(computerHand[i]);
                        rotate(computerHand + i, computerHand + i + 1, computerHand + computerHandSize);
                        --computerHandSize;
                        cardPutInPile = true;
                        cout << "\nComputer put a card in the Pile.";
                        break;
                    }
                }
                else if (i == (computerHandSize - 1))
                {
                    if ((computerHand[i].value != computerHand[i - 1].value))
                    {
                        // The card hasn't formed a pair with any other card
                        discardPile.push_back(computerHand[i]);
                        rotate(computerHand + i, computerHand + i + 1, computerHand + computerHandSize);
                        --computerHandSize;
                        cardPutInPile = true;
                        cout << "\nComputer put a card in the Pile.";
                        break;
                    }
                }
                else
                {
                    if ((computerHand[i].value != computerHand[i - 1].value))
                    {
                        // The card hasn't formed a pair with any other card
                        discardPile.push_back(computerHand[i]);
                        rotate(computerHand + i, computerHand + i + 1, computerHand + computerHandSize);
                        --computerHandSize;
                        cardPutInPile = true;
                        cout << "\nComputer put a card in the Pile.";
                        break;
                    }
                }
            }
        }
    }

    else
    {
        cout << "\nComputer chose to accept the card from the deck.";

        cout << "\nComputer HAnd:";
        printDeck(computerHand, computerHandSize);
        Card computerCard = deck[deckSize - 1];

        // Show the computer the card it chooses
        cout << "\nComputer chose from Deck: " << (int)computerCard.rank << " " << (char)computerCard.suit;
        bool validCard = computerDecision(deck[deckSize - 1], computerHand, computerHandSize, joker);

        if (validCard)
        {

            // Add the card to the computer's hand
            ++computerHandSize;
            computerHand[computerHandSize - 1] = computerCard;
            // Assuming you have a Card variable assignedCard representing the card assigned to the player
            removeCardFromDeck(deck, deckSize, computerCard);

            // Sort the computer's hand
            Sort(computerHand, computerHandSize);

            // Decide which card to put in the discard pile
            for (int i = 0; i < computerHandSize; ++i)
            {
                if (computerHand[i].value != joker)
                {
                    if (i == 0)
                    {
                        if ((computerHand[i].value != computerHand[i + 1].value))
                        {
                            // The card hasn't formed a pair with any other card
                            discardPile.push_back(computerHand[i]);
                            rotate(computerHand + i, computerHand + i + 1, computerHand + computerHandSize);
                            --computerHandSize;
                            cout << "\nComputer put a card in the Pile.";
                            break;
                        }
                    }
                    else if (i == (computerHandSize - 1))
                    {
                        if ((computerHand[i].value != computerHand[i - 1].value))
                        {
                            // The card hasn't formed a pair with any other card
                            discardPile.push_back(computerHand[i]);
                            rotate(computerHand + i, computerHand + i + 1, computerHand + computerHandSize);
                            --computerHandSize;
                            cout << "\nComputer put a card in the Pile.";
                            break;
                        }
                    }

                    else
                    {
                        if ((computerHand[i].value != computerHand[i - 1].value) && (computerHand[i].value != computerHand[i + 1].value))
                        {
                            // The card hasn't formed a pair with any other card
                            discardPile.push_back(computerHand[i]);
                            rotate(computerHand + i, computerHand + i + 1, computerHand + computerHandSize);
                            --computerHandSize;
                            cout << "\nComputer put a card in the Pile.";
                            break;
                        }
                    }
                }
            }
        }

            else
            {
                discardPile.push_back(deck[deckSize - 1]);
                removeCardFromDeck(deck, deckSize, computerCard);

                cout << "\nComputer put the same card in the Pile.";
            }
    }
}

int main()
{
    int handSize = 6;
    Card playerHand[handSize];
    Card computerHand[handSize];
    int deckSize = 52;
    vector<Card> discardPile;
    Card deck[deckSize];
    int k = 0; // (static_cast<int>) is used instead of C- type conversion. Supposedly it's the best conversion method
    for (int i = static_cast<int>(Signs::spade); i <= static_cast<int>(Signs::diamond); ++i)
    {
        for (int j = static_cast<int>(Ranks::Ace); j <= static_cast<int>(Ranks::King); ++j)
        {
            deck[k].suit = static_cast<Signs>(i);
            deck[k].rank = static_cast<Ranks>(j);
            deck[k].value = j;
            ++k;
        }
    } // Formation of deck.

    cout << "\t\t\t\t\t\t\tWELCOME TO TWIN GAME!!!\n";
    cout << "\t\t\t\t\t\t\tPress Enter to Continue";
    cin.ignore();

    system("CLS");
    cout << "\t\t\t\t\t\t\tTWIN GAME";
    // Shuffle the deck
    shuffleDeck(deck, deckSize);
    Card joker;
    pickJoker(deck, deckSize, joker); // Finding a joker;
    int Joker=joker.value;
    switch (Joker)
    {
    case 1:
        cout << "\nJoker For This Game: "
             << "[ "
             << "A"
             << " " << (char)joker.suit << " ]  ";
        break;
    case 11:
        cout << "\nJoker For This Game: "
             << "[ "
             << "J"
             << " " << (char)joker.suit << " ]  ";
        break;
    case 12:
        cout << "\nJoker For This Game: "
             << "[ "
             << "Q"
             << " " << (char)joker.suit << " ]  ";
        break;
    case 13:
        cout << "\nJoker For This Game: "
             << "[ "
             << "K"
             << " " << (char)joker.suit << "]  ";
        break;
    default:
        cout << "\nJoker For This Game: "
             << "[ " << joker.value << " " << (char)joker.suit << " ]  ";
    }

    dealCardsToPlayers(deck, playerHand, deckSize, handSize);
    dealCardsToPlayers(deck, computerHand, deckSize, handSize);
    // After receiving a card or any other relevant point in your code
    Sort(playerHand, handSize);
    Sort(computerHand, handSize);

    cout << "\nPlayer's Hand: ";
    printDeck(playerHand, handSize);
    int screen = 0;
    while (true)
    {
        cout << "\n"; // Pick matching cards
        pickMatchingCards(deck, deckSize, playerHand, discardPile);

        if (checkPairs(playerHand, handSize,Joker))
        {
            cout << "\nCongratulations! You have formed all pairs. Game over!\n";
            break;
        }
        cout << "\nPlayer's Hand:\n";
        printDeck(playerHand, handSize);
        cout << endl;

        printPileTop(discardPile);

        computerTurn(deck, deckSize, computerHand, discardPile,Joker);
        // Check if all pairs are formed in the player's hand
        if (checkPairs(computerHand, handSize,Joker))

        {
            cout << "\nComputer Won\n";
            break;
        }
        cout << "\nPlayer's Hand:\n";
        printDeck(playerHand, handSize);
        printPileTop(discardPile);
        screen++;
    }
    return 0;
}