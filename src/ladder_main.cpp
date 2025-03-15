#include "ladder.h"

using namespace std;

int main(){
    set<string> word_list;
    load_words(word_list, "words.txt");

    // prompt user for words
    string begin_word, end_word;
    cout << "Enter start word: ";
    cin >> begin_word;
    cout << "Enter end word: ";
    cin >> end_word;
    if (begin_word == end_word) error("main", "", "Start and end words are the same");
    
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);

    if(ladder.empty()) error("main", "", "No ladders found");
    
    verify_word_ladder(ladder, word_list);
    print_word_ladder(ladder);

    return 0;
}