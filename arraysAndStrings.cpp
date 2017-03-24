#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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
 * Problem 1.4 - Palindrome Permutation
 * Given a string, check if it is a permutation of a palindrome.
 *
 * Solution
 * --------
 *
 * If we analyze the anatomy of a palindrom, we can see it is 123...(x)...321 -> all chars
 * must come in pair except for the middle one, which may or not be there.
 *
 * That means there can be max 1 (so 0 or 1) chars with an odd count -> if that is satisfied, the given
 * string is a palindrome.
 *
 * Time complexity: O(N) -> we must check each character in string.
 * Space complexity: O(1) -> we need a data structure to store character count (sth like int[alphabetSize]),
 *                           but it doesn't depend on the size of the input, so it's O(1).
 *
 */
bool isPalindromePermutation (string str) {
    // TODO: is default value false?
    //
    // NOTE: We don't actually need the exact count of the each char,
    // but just the binary information whether the count is odd or even.
    // This is why we can use bool[] instead of int[].
    bool isCharFreqOdd[asciiSize] = { false };

    for (int i = 0; i < str.length(); i++) {
        int asciiCode = getAsciiCode(str[i]);
        isCharFreqOdd[asciiCode] = !isCharFreqOdd[asciiCode];
    }

    // Check the number of chars with the odd occurence frequency.
    int oddCount = 0;
    for (int i = 0; i < asciiSize; i++) {
        if (isCharFreqOdd[i] == true) oddCount++;
    }

    return (oddCount <= 1);
}

/*
 * Code that generates all permutations of a given palindrome.
 */

vector<int> getCharFrequency (string str) {
    vector<int> charFrequency (asciiSize, 0);

    // NOTE: Playing with C++11 here!
    for (char& c : str) {
        charFrequency[getAsciiCode(c)]++;
    }
    return charFrequency;
}

int countCharsAvailable(vector<int>& charFrequency) {
    int charsAvailable = 0;

    for (int& charCount : charFrequency) {
        charsAvailable += charCount;
    }
    return charsAvailable;
}

vector<string> generateAllPPWorker (vector<int>& charFrequency) {
    vector<string> permutations;

    // Base cases - zero or one characters available.
    int charsAvailable = countCharsAvailable(charFrequency);

    if (charsAvailable == 0) {
        permutations.push_back(""); // Not really semantically cool?
        return permutations;
    }
    if (charsAvailable == 1) {
        // Find that one character.
        for (int i = 0; i < charFrequency.size(); i++) {
            if (charFrequency[i] == 1) {
                permutations.push_back(string(1, i));
                return permutations;
            }
        }
    }

    // General case - try all available chars at most outer positions.
    for (int i = 0; i < charFrequency.size(); i++) {
        if (charFrequency[i] > 1) {
            charFrequency[i] -= 2;

            string outerChar = string(1, i);
            for (string& innerPerm : generateAllPPWorker(charFrequency)) {
                permutations.push_back(outerChar + innerPerm + outerChar);
            }

            charFrequency[i] += 2;
        }
    }
    return permutations;
}

vector<string> generateAllPalindromePermutations (string str) {
    vector<string> palindromePermutations;

    if (!isPalindromePermutation(str)) return palindromePermutations; // Empty vector

    // Count char occurence frequency.
    vector<int> charFrequency = getCharFrequency(str);

    // Recursively generate all palindroms.
    return generateAllPPWorker(charFrequency);
}

/*
 * End of code that generates all permutations of a given palindrome.
 */

void isPalindromePermutationTestAndOutput (string str) {
    cout    << "'" + str + "'" << " is" << (isPalindromePermutation(str) ? "" : " NOT")
            << " a palindrome permutation" << endl;

    if (isPalindromePermutation(str)) {
        cout << "Palindrome permutations are:" << endl;

        for (string& permutation : generateAllPalindromePermutations(str)) {
            cout << "'" + permutation + "'" << endl;
        }
        cout << endl;
    }
}

/*
 * Problem 1.5 - One Away
 * Given two strings, check if they are one or zero edits away.
 * Available edit operations are: insert, remove or replace a character.
 *
 * Solution
 * --------
 *
 * First obvious fact is that if the length difference between strings is
 * greater than 1, they cannot be one away.
 * Now we have two cases:
 * 1)   the length is the same - only thing we have to do now is go in parallel through
 *      the strings and count the differences - there may not be more than 1. 
 *
 * 2)   the length difference is 1 - check if the longer string contains all chars from
 *      the shorter one in the right order.
 *
 * Time complexity: O(N) - just go through the both strings.
 * Space complexity: O(1) - no additional data structures needed.
 *
 * What can be learned: Not much, maybe the trickiest part is to come up with the algorithm
 * for "length diff 1" case. It is also needed to understand that insert/remove operations are
 * inverses and do not have to be viewed separately. 
 * But this is all very specific, there isn't some general learning here.
 */

bool areOneReplacementAway (string str1, string str2) {
    // NOTE: It is assumed str1.length() == str2.length()
    int diffCount = 0;
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] !=  str2[i]) {
            diffCount++;
            if (diffCount > 1) return false;
        }
    } 
    return true;
}

bool areOneInsertAway (string shortStr, string longStr) {
    // NOTE: It is assumed that shortStr.length() + 1 = longStr.length()

    int longStrIdx = 0;
    for (char& shortStrChar : shortStr) {
        // Try to find shortStrChar in the long string, respecting the order.
        bool foundInLong = false;
        for (; longStrIdx < longStr.length() && foundInLong == false; longStrIdx++) {
            if (shortStrChar == longStr[longStrIdx]) foundInLong = true;
        }
        if (!foundInLong) return false;
    }
    return true;
}

bool areOneAway (string str1, string str2) {
    if (str1.length() == str2.length()) {
        return areOneReplacementAway(str1, str2);
    }
    if (str1.length() - str2.length() == 1) {
        return areOneInsertAway(str2, str1);
    }
    if (str2.length() - str1.length() == 1) {
        return areOneInsertAway(str1, str2);
    }

    return false;
}

void areOneAwayTestAndOutput (string str1, string str2) {
    cout    << str1 << ", " << str2 << " -> " 
            << (areOneAway(str1, str2) ? "true" : "false") << endl;
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
 *
 * What can be learned from this problem: The main trick was to understand what is palindrom and
 * observe it's properties. Implementation is then pretty straightforward.
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

/*
 * Problem 1.7 - Rotate Matrix
 * Given NxN matrix, write a method to rotate the image by 90 degrees. Can you do it in place?
 *
 * Solution
 * --------
 * 
 * Yes, it can be done in place since we have enough information to calculate the "new" position
 * of every element in the matrix.
 *
 * Now, if we want to put a specific element to its new position, we also have to move the element that
 * was originally there, in order to make place for the first one. That goes "in circle" for the 4 sides of a
 * matrix - in order to move one element, we will have to move 4 elements. We do that all corresponding quadruples
 * of elements and that's it!
 *
 * One way to do it is to start from the most outer "frame" of the matrix and then move towards the center.
 * This way things are nicely separated and we could even do it recursively - rotate(NxN) = rotate the most outer frame
 * and then rotate(N-1 x N-1).
 *
 * Time complexity: O(n^2) - we are touching all the elements of the matrix.
 * Space complexity: O(1) - no additional data structures, doing everything in place.
 *
 * What can be learned here: It is an interesting problem to decompose, but not sure if it
 * adds up to any general value. Implementation wise, it requires a bit of precision with the
 * matrix indices, that always takes me a moment to be sure I got it right.
 */

void exchangeCircular (int& a, int& b, int& c, int& d) {
    int tmp = b;

    b = a;
    a = d;
    d = c;
    c = tmp;

    return;
}

void rotateMatrix (vector<vector<int>>& matrix) {

    int numOfLayers = (matrix.size() + 1) / 2;
    for (int layer = 0; layer < numOfLayers; layer++) {

        int layerSize = matrix.size() - 2 * layer;
        int maxIdx = matrix.size() - 1;
        for (int i = 0; i < layerSize - 1; i++) {
            exchangeCircular(matrix[layer][layer + i],                      // Up
                             matrix[layer + i][maxIdx - layer],             // Right
                             matrix[maxIdx - layer][maxIdx - layer - i],    // Down
                             matrix[maxIdx - layer - i][layer]              // Left
                            );
        }
    }
    return;
}

void printMatrix (vector<vector<int>> matrix) {
    for (int x = 0; x < matrix.size(); x++) {
        for (int y = 0; y < matrix[0].size(); y++) {
            cout << matrix[x][y] << " ";
        }
        cout << endl;
    }
}

/*
 * Prints original and transformed matrix side by side.
 *
 * Assumption: Transformed matrix has the same dimensions as the original one.
 */
void printOriginalAndTransformedMatrix (vector<vector<int>>& original, vector<vector<int>>& transformed) {

    for (int x = 0; x < original.size(); x++) {
        // Print x-th row of the original matrix.
        for (int y = 0; y < original[0].size(); y++) {
            cout << original[x][y] << " ";
        }

        // Add some space between matrices.
        if (x == original.size() / 2) {
            cout << "  ->  ";
        } else {
            cout << "      ";
        }

        // Print x-th row of the rotated matrix.
        for (int y = 0; y < original[0].size(); y++) {
            cout << transformed[x][y] << " ";
        }
        cout << endl;
    }
}

void rotateMatrixTestAndOutput (vector<vector<int>>& matrix) {

    // Preserve original matrix.
    vector<vector<int>> originalMatrix = matrix;

    rotateMatrix(matrix);

    printOriginalAndTransformedMatrix(originalMatrix, matrix);
}

/*
 * Problem 1.8 - Zero Matrix
 * Given a MxN matrix, go through it and if an element is 0, set its entire row
 * and column to 0.
 *
 * Solution
 * --------
 *  
 * The solution here is straightforward - first make a pass through the matrix and
 * find out which rows and columns need to be set to zero, and then do it.
 *
 * We can't do it in just one pass, greedy - when you encounter 0, set row/col to 0,
 * because that would erase information (locations of the other zeros) from yet
 * unexplored cells of the matrix.
 *
 * Time complexity: O(N^2) (actually, O(N*M)) - we have to touch each cell.
 * Space complexity: O(N+M) - one array for rows and one for columns that should be
 * set to zero.
 *
 * As an additional memory optimization, author proposed an idea to use a specific
 * (typically first) row and column instead of auxilliary arrays. To do that, we also
 * have to use additional variables to preserve the data from this row and column.
 *
 * This works, but makes the logic more complicated. If we could afford to store the
 * whole matrix in the memory, we can probably afford to store one more row and column
 * as well.
 *
 * What can be learned: 
 * - don't go greedy without thinking in front - analyze the problem and observe the
 *   dependencies between the steps
 *
 * - to save on the memory, try reusing existing data structures instead of creating
 *   the new ones. In that case, we need to also develop an additional logic that 
 *   makes that posssible and makes sure no info gets lost.
 */
void nullifyMatrix (vector<vector<int>>& matrix) {
    int rowCount = matrix.size();
    int columnCount = matrix[0].size();

    vector<bool> isRowZero (rowCount, false);
    vector<bool> isColumnZero (columnCount, false);

    // Go through the matrix and find zeros -> marks rows
    // and columns to be nullified.
    for (int x = 0; x < rowCount; x++) {
        for (int y = 0; y < columnCount; y++) {
            if (matrix[x][y] == 0) {
                isRowZero[x] = true;
                isColumnZero[y] = true;
            }
        }
    }

    // Nullify rows.
    for (int x = 0; x < rowCount; x++) {
        if (isRowZero[x]) {
            for (int y = 0; y < columnCount; y++) {
                matrix[x][y] = 0;
            }
        }
    }

    // Nullify columns.
    for (int y = 0; y < columnCount; y++) {
        if (isColumnZero[y]) {
            for (int x = 0; x < rowCount; x++) {
                matrix[x][y] = 0;
            }
        }
    }
    return;
}

void nullifyMatrixTestAndOutput (vector<vector<int>>& matrix) {

    // Preserve original so we can compare it to the nullified matrix.
    vector<vector<int>> original = matrix;

    nullifyMatrix(matrix);

    printOriginalAndTransformedMatrix(original, matrix);

    return;
}

/*
 * Problem 1.9 - String Rotation
 * Given two strings s1 and s2, check whether s2 is a rotation of s1
 * using only call to isSubstring() method.
 * E.g: s1 = "waterbottle"
 *      s2 = "erbottlewat"
 *
 * Solution
 * --------
 *
 * If we analyze the anatomy of the rotated word, we can see it's "cut"
 * at the specific position and then the parts are switched - second part
 * is put before the first one.
 *
 * If we then try to match the rotated word to the original, we'll see that
 * the rotated word "extends" beyond the last character of the original one.
 *
 * The idea here is to duplicate the original (that way s2 will never protrude beyond s1), 
 * s1, and check whether s2 is its substring. So it's s2.isSubstring(s1+s1);
 *
 * Also, we have to make sure that the strings are of the same length.
 *
 * Time complexity: O(N) - we check matching via isSubstring()
 * Space complexity: O(N) - we make a duplicate of s1
 *
 * What can be learned here: Not really sure what is the point here, I suppose
 * generic problem solving ability. This is more like a neat trick, implementation is
 * super simple once we get this part.
 *
 * Note
 * ----
 * I also loved the simplicity of the author's explanation:
 * Original string is xy (x and y are parts before and after the rotation point), 
 * while the rotated is then yx - yx will always be substring of xyxy.
 */

bool isRotation (string s1, string s2) {
    if (s1.length() != s2.length()) return false;
    return ((s1 + s1).find(s2) != string::npos);
}

void isRotationTestAndOutput (string s1, string s2) {
    cout << s1 + ", " + s2 << " -> " << (isRotation(s1, s2) ? " true" : " false") << endl;
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

    // Testing problem 4 - Is Palindrome Permutation
    isPalindromePermutationTestAndOutput("tacocat");
    isPalindromePermutationTestAndOutput("matija");

    cout << endl;

    // Testing problem 5 - One Away
    areOneAwayTestAndOutput("pale", "ple");
    areOneAwayTestAndOutput("pales", "pale");
    areOneAwayTestAndOutput("pale", "bale");
    areOneAwayTestAndOutput("pale", "bake");
    areOneAwayTestAndOutput("matija", "martin");

    cout << endl;

    // Testing problem 6 - String Compression
    compressRepeatedCharsTestAndOutput("aaaabbbbccccd");
    compressRepeatedCharsTestAndOutput("a");

    cout << endl;

    // Testing problem 7 - Rotate Matrix 
    vector<vector<int>> matrix = 
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    rotateMatrixTestAndOutput(matrix);

    cout << endl;

    // Testing problem 8 - Zero Matrix
    vector<vector<int>> zeroMatrix = {
        {1, 2, 3, 4},
        {1, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 0}
    };
    nullifyMatrixTestAndOutput(zeroMatrix);

    cout << endl;

    // Testing problem 9 - String Rotation
    isRotationTestAndOutput("waterbottle", "erbottlewat");
    isRotationTestAndOutput("waterbottle", "xmbottlewat");
    isRotationTestAndOutput("waterbottle", "water");

    return 0;
}
