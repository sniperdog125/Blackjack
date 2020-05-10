#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h>
using namespace std;

//function prototypes
void shuffledeck(vector<int> &);
void resetdeck(vector<int> &);
void printdeck(vector<int>);
string getsuit(int);
string getfacevalue(int);
int getcardvalue(int);
void menu();
void displaymenu();
string getmenuinput();
void game();
void dealcards(vector<int> &, vector<int> &, vector<int> &);
void hit(vector<int> &, vector<int> &);
void printplayerhand(vector<int>, bool);
void printdealerhand(vector<int>);
int gethandvalue(vector<int>);
void hitstaybust(vector<int> &, vector<int> &);
void dealerai(vector<int> &, vector<int> &);
void checkwinner(int, int);
string playagain();

bool debug = 0;


int main() {
  game();
}

//outer function that calls the neccessary functions in order to play blackjack
void game()
{
  string input;
  vector<int> deck, playerhand, dealerhand;
  do
  {
    shuffledeck(deck);
    dealcards(playerhand, dealerhand, deck);
    cout << "The dealer shuffles and deals the cards" << endl;
    usleep(1000000);
    if (debug)
    {
      printdeck(deck);
      printdeck(playerhand);
      printdeck(dealerhand);
    }

    printdealerhand(dealerhand);
    printplayerhand(playerhand, 1);
    hitstaybust(playerhand, deck);
    if (gethandvalue(playerhand) < 22)
      dealerai(dealerhand, deck);
    checkwinner(gethandvalue(playerhand), gethandvalue(dealerhand));
    input = playagain();
  } while (input == "y" || input == "Y");
}

//clears the player and dealers hand before adding two cards to each hand
void dealcards(vector<int> &player, vector<int> &dealer, vector<int> &deck)
{
  if (debug)
    cout << endl << "initial dealing of cards";
  player.clear();
  dealer.clear();
  hit(player, deck);
  hit(dealer, deck);
  hit(player, deck);
  hit(dealer, deck);
}

//takes the top card from the deck and adds it to the appropriate hand
void hit(vector<int> &hand, vector<int> &deck)
{
  if (debug)
    cout << endl << "hit";
  hand.push_back(deck[0]);
  deck.erase(deck.begin());
}

//if player has less than 22 the function asks if the player wants another card or whether they want to stay
void hitstaybust(vector<int> &hand, vector<int> &deck)
{
  string input;
  while (gethandvalue(hand) < 22)
  {
    do 
    {
      cout << "1) hit" << endl
           << "2) stay" << endl;
      cin >> input;
    } while (input != "1" && input != "2");

    if (input == "1")
    {
      hit(hand, deck);
      printplayerhand(hand, 1);
    }
    else if (input == "2")
      break;
    else
      cout << "Hit Stay Bust ERROR" << endl;
  }
}

//simple dealer ai that makes the dealer hit when they have 16 or less
void dealerai(vector<int> &hand, vector<int> &deck)
{
  printplayerhand(hand, 0);
  while (gethandvalue(hand) < 17)
  {
    cout << "Dealer hits..." << endl;
    usleep(100000);
    hit(hand, deck);
    printplayerhand(hand, 0);
  }
}

//checks who won this hand
void checkwinner(int player, int dealer)
{
  if (player > 21)
    cout << "Player has busted with " << player << endl;
  else if (dealer > 21)
    cout << "Dealer has busted with " << dealer << endl << "You win this hand!" << endl;
  else if (player > dealer)
  {
    cout << "Player has " << player << "\nDealer has " << dealer << "\nPlayer Wins!\n";
  }
  else if (dealer > player)
    cout << "Player has " << player << "\nDealer has " << dealer << "\nDealer Wins...\n";
  else if (dealer == player)
    cout << "Dealer and player both tie with " << player << endl;
  else
    cout << "ERROR Player has " << player << " Dealer has " << dealer;
}

void menu()
{
  string input;
  do
  {
    displaymenu();
    input = getmenuinput();
  } while (input != "9");
}

void displaymenu()
{
  cout << "-----------------------------------------------------------------------------\n";
  cout << "                               BlackJack\n";
  cout << "-----------------------------------------------------------------------------\n\n";
  cout << "\t\t\t\t1)\tNew Game\n";
  cout << "\t\t\t\t2)\tRules\n";
  cout << "\t\t\t\t9)\tExit\n\n\n";
}

string getmenuinput()
{
  bool error = 1;
  string input;
  do
  {
    cout << "\t\tPlease Enter The Number That Corresponds To Your Menu Choice: ";
    cin >> input;
    if (input == "1" || input == "2" || input == "9")
      error = 0;
    else
      cout << "\t\t\t\tBad Input\n";
  } while (error == 1);
  return input;
}

//ask the user if they want to play another hand and returns their answer
string playagain()
{
  string input;
  do
  {
    cout << "Play another hand?(y/n) ";
    cin >> input;
  } while (input != "n" && input != "N" && input != "y" && input != "Y");

  return input;
}

//reset the unshuffled deck in preparation for being transferred to the shuffled deck
void resetdeck(vector<int> &deck) {
  deck.clear();
  for (int i = 0; i < 52; i++)
  {
    deck.push_back(i);
  }
}

//shuffles the deck into a random assortment of cards
void shuffledeck(vector<int> &deck)
{
  //initialize a vector for the unshuffled deck and a random variable
  vector<int> unshuffled_deck;
  int random;
  srand(time(NULL));

  // call the reset deck function to fill the unshuffled deck
  //cout << "reset deck" << endl;
  resetdeck(unshuffled_deck);
  //cout << "reset complete" << endl;

  for (int i = 52; unshuffled_deck.size() > 0; i--)
  {
    if (debug)
      cout << i << endl;
    random = rand() % i;
    deck.push_back(unshuffled_deck[random]);
    unshuffled_deck.erase(unshuffled_deck.begin() + random);
    if (debug)
      cout << unshuffled_deck.size();
  }
  if (debug)
  {
    cout << "shuffle complete" << endl;
    printdeck(deck);
    cout << "print complete";
  }
}

//debug function which prints the decks(or any hands) contents
void printdeck(vector<int> deck)
{
  cout << endl << "printing" << endl;
  for (int i = 0; i < deck.size(); i++)
  {
    cout << endl << i << " " << deck[i] << " is the " << getfacevalue(deck[i]) << " of " << getsuit(deck[i]) << " with a value of " << getcardvalue(deck[i]);
  }
}

//prints out the player or dealers hand to output
void printplayerhand(vector<int> hand, bool player)
{
  if (player)
  {
    cout << "Player has ";
  } else
  {
    cout << "Dealer has ";
  }
  for (int i = 0; i < hand.size(); i++)
  {
    cout << getfacevalue(hand[i]) << " of " << getsuit(hand[i]) << ", ";
  }
  cout << " with a total value of " << gethandvalue(hand) << endl;
}

//totals up the value of a hand. checks if the ace is worth 1 or 11
int gethandvalue(vector<int> hand)
{
  int total = 0;
  bool ace = 0;

  for (int i = 0; i < hand.size(); i++)
  {
    total += getcardvalue(hand[i]);
    if (getfacevalue(hand[i]) == "Ace")
      ace = 1;
  }
  if (ace && total + 10 < 22)
    total += 10;
  
  return total; 
}

//lets the player know the face up card of the dealer in the initial deal
void printdealerhand(vector<int> hand)
{
  cout << "Dealer has " << getfacevalue(hand[1]) << " of " << getsuit(hand[1]) << " and a face down card" << endl;
}

//returns the suit of the card
string getsuit(int card)
{
  if (card < 0)
    return "ERROR";
  else if (card < 13)
    return "Spades";
  else if (card < 26)
    return "Diamonds";
  else if (card < 39)
    return "Clubs";
  else if (card <52)
    return "Hearts";
  else
    return "ERROR";
}

//returns the face of the card 
string getfacevalue(int card)
{
  card %= 13;

  switch (card)
  {
  case 0: return "Ace";
    break;
  case 1: return "Two";
    break;
  case 2: return "Three";
    break;
  case 3: return "Four";
    break;
  case 4: return "Five";
    break;
  case 5: return "Six";
    break;
  case 6: return "Seven";
    break;
  case 7: return "Eight";
    break;
  case 8: return "Nine";
    break;
  case 9: return "Ten";
    break;
  case 10: return "Jack";
    break;
  case 11: return "Queen";
    break;
  case 12: return "King";
    break;
  default: return "ERROR";
  }
}

//returns the value of the card for calculations
//aces are valued at one. in the gethandvalue function if the player has an ace the function will check whether 1 or 11 is more useful for the hand
int getcardvalue(int card)
{
  card %= 13;

  switch (card)
  {
  case 0: return 1;
    break;
  case 1: return 2;
    break;
  case 2: return 3;
    break;
  case 3: return 4;
    break;
  case 4: return 5;
    break;
  case 5: return 6;
    break;
  case 6: return 7;
    break;
  case 7: return 8;
    break;
  case 8: return 9;
    break;
  case 9: return 10;
    break;
  case 10: return 10;
    break;
  case 11: return 10;
    break;
  case 12: return 10;
    break;
  default: return -1;
  }
}