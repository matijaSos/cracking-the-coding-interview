#include <iostream>
#include <string>
#include <sstream>

using namespace std;

static const int asciiSize = 256;

int getAsciiCode (char c) {
    return c;
}

/*
 * Problem 1.1
 * Implement an algorithm that determines if a string has all unique characters.
 *
 * Solution
 * --------
 *
 * We just go through the given string, and for each character we encounter we mark it, e.g.
 * ('a' -> occured).
 * If we encounter again a character that has already occurred, then the string is not unique.
 *
 * We're using a separate data structure to keep track of char occurrences - it's basically
 * an array bool[totalCharNum], where we put 'true' for each character we encounter.
 * 
 * We could have used a hash table for this as well, but this is such a simple case that we've
 * actually implemented it ourselves.
 *
 * Time complexity: O(N, N being string length) - we just go through each char and that's it.
 * Space complexity: O(1) - we are using array bool[totalCharNum] which is of constant size,
 * doesn't depend on the input size.
 *
 * Twist:   What if we can't use any additional data structures?
 * Answer:  Then we'd just check each char against every other -> if two are the same, false.
 *          Time complexity here is O(N^2).
 *
 * NOTE(matija): Author also proposed to use bitvector instead of bool array to reduce the
 * memory footpring. My question: Why bool takes a byte, and not just a single bit?
 */

bool isUnique (string input) {
    // If there is more characters in the given string than the total number
    // of distinct characters that ASCII can represent, there will certainly be
    // repeated characters.
    if (input.length() > asciiSize) return false;

    // Stores for every char whether it has already occurred in a given string.
    // Basically a hash table, only we implemented it ourselves since it's simple and intuitive.
    bool charOccurred[asciiSize] = {false};

    for (int i = 0; i < input.length(); i++) {
        int charAsciiCode = getAsciiCode(input[i]);

        if (charOccurred[charAsciiCode]) {
            return false;
        } else {
            charOccurred[charAsciiCode] = true;
        }
    }

    return true;
}

void isUniqueTestAndOutput (string input) {
    cout << input << ": " << (isUnique(input) ? "true" : "false") << endl;
    return;
}

/*
 * Problem 1.2
 * Given two strings, check if one is a permutation of another.
 *
 * Solution
 * --------
 *
 * Super simple solution: sort two strings and check if they're equal.
 * Very elegant, but O(n log n) for sorting.
 *
 * Optimal solution: Compare character count of each string, they have to be equal in order
 * to be palindromes.
 */

bool checkPermutation (string s1, string s2) {
    // Can't be anagrams if they are not the same length.
    if (s1.length() != s2.length()) return false;

    // NOTE(matija): this initializes all elements to the default value, which is 0.
    //
    int charCount[asciiSize] = {};

    // s1 adds, while s2 subtracts. If anagrams, there will be only zeros at the end in charCount.
    for (int i = 0; i < s1.size(); i++) {
        charCount[getAsciiCode(s1[i])]++;
    }
    for (int i = 0; i < s2.size(); i++) {
        // NOTE: I could've stuffed everyting in only one for loop, but I felt it will be more
        // verbose this way.
        charCount[getAsciiCode(s2[i])]--;
    }

    // Check if charCount contains only zeros.
    for (int i = 0; i < asciiSize; i++) {
        if (charCount[i] != 0) return false;
    }

    return true;
}

void checkPermutationTestAndOutput (string s1, string s2) {
    cout << s1 + ", " + s2 + " " 
         << (checkPermutation(s1, s2) ? "are " : "are NOT ") << "anagrams" 
         << endl;
    return;
}

/*
 * Problem 1.3 - URLify
 * Given a string with spaces, replace each space with string "%20". There will be extra white space at the
 * end of the string - do it in place.
 *
 * Solution
 * --------
 *
 * If we could make a new string, this would be easier and nicer. This way we have to shift stuff around to make space
 * for the new characters.
 * The idea is to move every char to its "new" position - we have to calculate that position in some way.
 *
 * This is how to do it:
 *      1. Count the number of spaces between the words.
 *      2. Start from the "true" end of the string (moving backwards) and shift each char to the "new" position
 *          - "new" position is: oldPosition + numOfSpacesInFront * 2
 *      3. After I've shifted the single word, also add "%20" in front of it.
 *
 * The point is to operate from the end of the string towards the beginning, so I don't overwrite anything.
 *
 * Time complexity: O(n) - doing two passes through the given string
 * Space complexity: O(1) - not using anything but the given string
 */


int countSpacesBetweenWords (string str, int trueLength) {
    int spaceCount = 0;
    for (int i = 0; i < trueLength; i++) {
        if (str[i] == ' ') spaceCount++;
    }
    return spaceCount;
}

string urlify (string str, int trueLength) {

    int spaceInFrontCount = countSpacesBetweenWords(str, trueLength);

    for (int pos = trueLength - 1; pos >= 0; pos--) {
        int newPos = pos + 2*spaceInFrontCount;

        if (str[pos] == ' ') {
            // In this case, newPos marks the end of the expanded space we're looking at.
            str[newPos] = '0';
            str[newPos - 1] = '2';
            str[newPos - 2] = '%';

            spaceInFrontCount--;
        } else {
            str[newPos] = str[pos];
        }
    }
    return str;
}

void urlifyTestAndOutput (string str, int trueLength) {
    cout << "'" + str + "' -> " << urlify(str, trueLength) << endl;
}

/*
 * Problem 1.6 - String Compression
 *
 * Compress sequences of repeated characters. E.g., aaabbc -> a3b2c1
 * Compress only if the resulting string is shorter than the original.
 *
 * Solution
 * --------
 *
 * The idea is to simply go through the string and count repeated characters, and in parallel build the
 * string holding a solution. When we encounter the different character (or the end of the string), we append
 * the "codified" version of that sequence.
 *
 * Time complexity: O(N) -> we just go through the string and do stuff.
 * Space complexity: O(N) -> we create a new string where the solution is stored.
 */

string intToString (int a) {
    stringstream ss;
    ss << a;
    return ss.str();
}

string getCodeForSequence (char sequenceChar, int sequenceLength) {
    return sequenceChar + intToString(sequenceLength);
}

string compressRepeatedChars (string str) {
    if (str.length() == 0) return str;

    string compressedStr = "";

    // NOTE: Because of this, we need check if string.length() > 0.
    char currentChar = str[0];
    int sequenceLength = 1;

    for (int i = 1; i < str.length(); i++) {
        if (str[i] != currentChar) {
            // NOTE: Unlike Java, strings are mutable in C++. Author provided the solution in
            // Java and argued about the concatenation complexity of String vs. StringBuffer.
            // In C++, we don't have to worry about this since C++ uses auto-resize, similar to
            // ArrayList in Java I suppose.
            compressedStr += getCodeForSequence(currentChar, sequenceLength);

            // Reset counter.
            currentChar = str[i];
            sequenceLength = 1;
        } else {
            sequenceLength++;
        }
    }
    // We've reached the end of the string - append the code of the last sequence.
    compressedStr += getCodeForSequence(currentChar, sequenceLength);

    // Check if bigger than original.
    //
    // NOTE: Author added check in the beginning that first went through the str and calculated the size
    // of the compressed string. That requires repeating logic and does not improve complexity, so I omitted
    // it for the sake of brevity.
    if (compressedStr.length() < str.length())
        return compressedStr;
    else
        return str;
}

void compressRepeatedCharsTestAndOutput (string str) {
    cout << str << " -> " << compressRepeatedChars(str) << endl;
}

int main() {

    // Testing problem 1 - isUnique
    isUniqueTestAndOutput("matija");
    isUniqueTestAndOutput("martin");

    cout << endl;

    // Testing problem 2 - checkPermutation
    checkPermutationTestAndOutput("matija", "ajitam");
    checkPermutationTestAndOutput("matija", "martin");

    cout << endl;

    // Testing problem 3 - URLify
    urlifyTestAndOutput(" matija    martin          ", 17);

    cout << endl;

    // Testing problem 6 - String Compression
    compressRepeatedCharsTestAndOutput("aaaabbbbccccd");
    compressRepeatedCharsTestAndOutput("a");

    return 0;
}
