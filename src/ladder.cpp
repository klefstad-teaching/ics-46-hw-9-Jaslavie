#include "ladder.h"
#include <fstream>

void error(string word1, string word2, string msg) {
    cerr << "Error for " << word1 << " and " << word2 << ": " << msg << endl;
}

// check that two strings are valid edit distances between each other
// d = levenshtein distance between two strings
bool edit_distance_within(const std::string& str1, const std::string& str2, int d){
    // check if length distance is greater than d
    if (abs(int(str1.length()) - int(str2.length())) > d) return false;
    // check if strings are the same
    if (str1 == str2) return d >= 0;
    // check if strings are one letter apart
    if (d==1 || d==0) return is_adjacent(str1, str2); 
    // default case (d > 1)
    return false; 
}

// check that two words are exactly one letter apart
bool is_adjacent(const string& word1, const string& word2){
    // check if two words are one letter apart
    if (word1.length() == word2.length()){
        int diff = 0;
        for (size_t i = 0; i < word1.length(); ++i) {
            if (word1[i] != word2[i]) {
                ++diff;
                if(diff > 1) return false;
            }
        }
        return diff <=1; // true if diff is 0 or 1
    }

    // check if one word is one letter longer
    if (abs(int(word1.length()) - int(word2.length())) != 1) return false;
    // check which is longer
    const string& shorter = word1.length() < word2.length() ? word1 : word2;
    const string& longer = word1.length() > word2.length() ? word1 : word2;

    // check if word can be formed by adding a letter
    size_t short_index = 0, long_index = 0;
    bool skipped = false;

    while (short_index < shorter.length() && long_index < longer.length()) {
        if (shorter[short_index] != longer[long_index]) {
            if(skipped) return false; // more than one letter diff
            skipped = true;
            ++long_index;
        } else {
            ++short_index;
            ++long_index;
        }
    }

    // words are adjacent
    return true;
}

// create a word ladder from the first word to last word only using words from word_list
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list){
    // ladder queue of potential ladder of strings to visit from a word through BFS
    // input: "cat"
    // output: [ ["cat", "bat"], ["cat", "mat"], ["cat", "cot"], ... ]
    queue<vector<string>> ladder_queue;
    ladder_queue.push(vector<string>{begin_word});

    // set to store visited words
    set<string> visited;
    visited.insert(begin_word);

    while(!ladder_queue.empty()){
        // remove the top ladder element
        vector<string> ladder = ladder_queue.front(); // ex: ["cat", "bat"]
        ladder_queue.pop();
        // get the last word in the ladder
        string last_word = ladder.back(); // ex: "bat"

        // process each word in the word_list to find an element adjacent to the most recently added element
        // create a new ladder when: 
        // 1. word is adjacent (i.e. accessible from current vertex)
        // 2. word has not been visited before
        for (const string& word : word_list){
            if (is_adjacent(last_word, word)){
                // check word has not been visited
                if(visited.count(word) == 0){
                    visited.insert(word);
                    // create new ladder vector to store (word, adjacent word) pair
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    // return and break function when the ladder is complete (found the end word)
                    if (word == end_word)  
                        return new_ladder;
                    ladder_queue.push(new_ladder);
                }
            }
        };
    }

    cerr << "no ladders found" << endl;
    return vector<string>();
}   

// load valid words from file into the word_list set
void load_words(set<string> & word_list, const string& file_name){
    ifstream in(file_name);
    if (!in) error("load_words", file_name, "Can't open input file");
    string word;
    while (in >> word) 
        word_list.insert(word);
    in.close();
}

void print_word_ladder(const vector<string>& ladder){
    for (const string& word : ladder)
        cout << word << " ";
}

// verify that ladder is valid 
void verify_word_ladder(const vector<string>& ladder, const set<string>& word_list){
   // empty or only one word
   if(ladder.size() <=1) error("verify_word_ladder", "", "Ladder is empty or only one word");

   // repeated words
   set<string> used_words;
   for (const string& word : ladder) {
    if (used_words.count(word) > 0) error("verify_word_ladder", "", "Repeated word found in ladder");
    used_words.insert(word);
   }

   // all words must be in dictionary
   for (size_t i = 1; i < ladder.size(); ++i) {
    if (word_list.count(ladder[i]) == 0) error("verify_word_ladder", "", "Word not found in dictionary");
   }

   // check adjacency
   for (size_t i = 0; i < ladder.size() - 1; ++i) {
    if (!is_adjacent(ladder[i], ladder[i+1])) error("verify_word_ladder", "", "Words are not adjacent");
   }
}