#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
bool getWrapAround() {
    char wrapAround;
    do {
        cout << "Would you like to play with wraparound? (y/n)" << endl;
        cin >> wrapAround;
    } while (wrapAround != 'y' && wrapAround != 'n');
    bool wA = (wrapAround == 'y'? true : false);
    return wA;
}
vector<char> createAlphabet() {
    vector<char> alphabet = {'a'};
    char letter = alphabet.at(0);
    for (int i = 0; i < 25; i++) {
        letter++;
        alphabet.push_back(letter);
    }
    return alphabet;
}
vector<char> getLeft(vector<char> alphabet, char letter, bool wrapAround, int surroundingLetters, int index) {
    vector<char> leftSide;
    int i;
    if (wrapAround) {
        if (index - surroundingLetters < 0) {
            i = 26 + (index - surroundingLetters);
        } else {
            i = index - surroundingLetters;
        }
        while (i != index) {
            leftSide.push_back(alphabet.at(i));
            i = (i + 1) % 26;
        }
    } else {
        i = (index - surroundingLetters < 0 ? 0 : index - surroundingLetters);
        while(i < index) {
            leftSide.push_back(alphabet.at(i));
            i++;
        }
    }
    return leftSide;
}
vector<char> getRight(vector<char> alphabet, char letter, bool wrapAround, int surroundingLetters, int index) {
    vector<char> rightSide;
    index++;
    int i;
    if (wrapAround) {
        i = (index + surroundingLetters) % 26;
        index %= 26;
        while (index != i) {
            rightSide.push_back(alphabet.at(index));
            index = (index + 1) % 26;
        }
    } else {
        i = (index + surroundingLetters > 26 ? 26 : index + surroundingLetters);
        while(index < i) {
            rightSide.push_back(alphabet.at(index));
            index++;
        }
    }
    return rightSide;
}
vector<char> inputLeft(bool wrapAround, int surround, int index) {
    int i;
    char response;
    vector<char> left;
    if (wrapAround) {
        if (surround == 1) {
            cout << "Please guess the letter to the left of the letter" << endl;
        } else {
            cout << "Please guess the " << surround << " letters to the left of the letter (separated by spaces)" << endl;
        }
        cin.sync();
        for (i = 0; i < surround; i++) {
            cin >> response;
            left.push_back(response);
        }
    } else {
        if (index == 1 || surround == 1 && index != 0) {
            cout << "Please guess the letter to the left of the letter" << endl;
        } else if (index - surround >= 0) {
            cout << "Please guess the " << surround << " letters to the left of the letter (separated by spaces)" << endl;
        } else if (index - surround < 0 && index > 1) {
            cout << "Please guess the " << index << " letters to the left of the letter (separated by spaces)" << endl;
        }
        cin.sync();
        for (i = 0; i < surround; i++) {
            if (index - i > 0) {
                cin >> response;
                left.push_back(response);
            }
        }
    }
    return left;
}
vector<char> inputRight(bool wrapAround, int surround, int index) {
    int i;
    vector<char> right;
    char response;
    if (wrapAround) {
        if (surround == 1) {
            cout << "Please guess the letter to the right of the letter" << endl;
        } else {
            cout << "Please guess the " << surround << " letters to the right of the letter (separated by spaces)" << endl;
        }
        cin.sync();
        for (i = 0; i < surround; i++) {
            cin >> response;
            right.push_back(response);
        }
    } else {
        if (index == 24 || surround == 1 && index != 25) {
            cout << "Please guess the letter to the right of the letter" << endl;
        } else if (index + surround <= 25) {
            cout << "Please guess the " << surround << " letters to the right of the letter (separated by spaces)" << endl;
        } else if (index + surround > 25 && index < 24) {
            cout << "Please guess the " << 25 - index << " letters to the left of the letter (separated by spaces)" << endl;
        }
        cin.sync();
        for (i = 0; i < surround; i++) {
            if (i + index < 25) {
                cin >> response;
                right.push_back(response);
            }
        }
    }
    return right;
}
void printCorrect(vector<char> alphabet, int currentLetter, vector<char> left, vector<char> right, vector<char> rightLeft, vector<char> rightRight) {
    cout << "You guessed: ";
    for (int i = 0; i < left.size(); i++) {
        cout << left.at(i) << " ";
    }
    cout << alphabet.at(currentLetter) << " ";
    for (int i = 0; i < right.size(); i++) {
        cout << right.at(i) << " ";
    }
    cout << endl << "The correct answer was: ";
    for (int i = 0; i < rightLeft.size(); i++) {
        cout << rightLeft.at(i) << " ";
    }
    cout << alphabet.at(currentLetter) << " ";
    for (int i = 0; i < rightRight.size(); i++) {
        cout << rightRight.at(i) << " ";
    }
    cout << endl;
}
bool updateNotSeen(vector<char> &notSeen, vector<char> alphabet, int unusedLetter, int &incorrect) {
    char response;
    bool playAgain = true;
    notSeen.erase(notSeen.begin() + unusedLetter);
    if (notSeen.size() == 0) {
        notSeen = alphabet;
        cout << "End of alphabet. You made "<< incorrect << " errors. Would you like to play again? (y/n)" << endl;
        incorrect = 0;
        do {
            cin >> response;
        } while (response != 'y' && response != 'n');
        playAgain = (response == 'y'? true : false);
    }
    return playAgain;
}
int main() {
    srand(time(0));
    vector<char> alphabet = createAlphabet();
    cout << "Welcome to the game that helps you learn the surrounding letters of the alphabet." << endl;
    int surround;
    int incorrect = 0;
    do {
        cout << "How many surrounding letters would you like to play with?" << endl;
        cin >> surround;
    } while (surround < 1);
    bool wA = getWrapAround();
    char response;
    vector<char> notSeen = alphabet;
    bool correctLeft, correctRight, playAgain;
    int currentLetter, unusedLetter;
    do {

        unusedLetter = rand() % notSeen.size();
        cout << "Your letter is: " << notSeen.at(unusedLetter) << endl;
        currentLetter = static_cast<int>(notSeen.at(unusedLetter)) - 97;

        vector<char> left = inputLeft(wA, surround, currentLetter);

        vector<char> rightLeft = getLeft(alphabet, alphabet.at(currentLetter), wA, surround, currentLetter);
        correctLeft = (rightLeft == left ? true : false);

        vector<char> right = inputRight(wA, surround, currentLetter); //crashed after y
        vector<char> rightRight = getRight(alphabet, alphabet.at(currentLetter), wA, surround, currentLetter);
        correctRight = (rightRight == right ? true : false);

        if (correctLeft && correctRight) {
            cout << "You are correct!" << endl;
        } else if (!correctLeft && !correctRight) {
            cout << "Sorry, both of those were incorrect." << endl;
            printCorrect(alphabet, currentLetter, left, right, rightLeft, rightRight);
        } else if (!correctLeft) {
            cout << "Your left letter(s) were incorrect" << endl;
            printCorrect(alphabet, currentLetter, left, right, rightLeft, rightRight);
        } else if (!correctRight) {
            cout << "Your right letter(s) were incorrect" << endl;
            printCorrect(alphabet, currentLetter, left, right, rightLeft, rightRight);
        }
        if (!correctLeft || !correctRight) {
            incorrect++;
        }
        playAgain = updateNotSeen(notSeen, alphabet, unusedLetter, incorrect);

    } while (playAgain); //response != '!'



    return 0;
}
