// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include "csvstream.h"
#include <set>
#include <cmath>
#include <string>
#include <cassert> 
using namespace std;

class Classifier{
    public:

    Classifier(){
        total_num_posts = 0;
        num_unique_words = 0;
    }

    set<string> unique_words(const string &str){
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
          words.insert(word);
        }
        return words;
    }

    void train(csvstream* csvin, bool debug){
        //csvstream csvin(filename);
        map<string, string> post;
        if (debug) {
            cout << "training data:" << endl;
        }
        while (*csvin >> post) {
            const string label = post["tag"];
            const string content = post["content"];
            set<string> words = unique_words(content);
            
            for (auto &i : words) {
                overall_set[i] += 1;
            }

            labels[label].insert(words);
            num_unique_words = overall_set.size();
            total_num_posts++;

            if (debug) {
                cout << "  label = " << label << ", content = " << content << endl;
            }
        }

        cout << "trained on " << total_num_posts << " examples" << endl;
        if (debug) {
            cout << "vocabulary size = " << num_unique_words << endl;
        }
        cout << endl;
    }


    void test(csvstream* csvin) {
        cout << "test data:" << endl;
        //csvstream csvin(filename);
        int num_posts = 0;
        int correct_posts = 0;
        map<string, string> post;
        while (*csvin >> post) {
            const string &label = post["tag"];
            const string &content = post["content"];
            set<string> words = unique_words(content);
            const pair<string, double> predicted_ans = expected_label(content);
            
            cout << "  correct = " << label << ", predicted = " 
            << predicted_ans.first << ", log-probability score = " 
            << predicted_ans.second << endl;
           
            cout << "  content = " << content << endl << endl;
            num_posts++;
            if (label == predicted_ans.first) {
                correct_posts++;
            }
        }
        cout << "performance: " << correct_posts << " / " << num_posts 
        << " posts predicted correctly" << endl;
    }

 
    int num_posts_with_word(string word) {
        int total = 0;
        for (auto &w : labels) {
            total += w.second.words[word];
        }
        return total;
    }

    int num_posts_with_label(string label) {
        return labels[label].total_num_posts;
    }

    int num_posts_label_and_word(string word, string label) {
        return labels[label].words[word];
    }

    double log_sum(set<string> posts, string label){
        double sum = log_score(label);
        for (auto &p : posts){
            string word = p;
            double additional_sum = log_conditional(word, label);
            sum += additional_sum;
        }
        return sum;
    }

    double log_score(string label){
        double tnp = total_num_posts;
        double np_withL = num_posts_with_label(label);
        if (np_withL != 0) {
            return log(np_withL / tnp);
        } else {
            return log(1.0 / tnp);
        }
    }

    double log_conditional(string word, string label) {
        double tnp = total_num_posts;
        if(overall_set.find(word) == overall_set.end()) {
            return log(1.0/tnp);
        } else if (num_posts_label_and_word(word, label) == 0) {
            return log(num_posts_with_word(word) / tnp);
        } else {
            double npl_w = num_posts_label_and_word(word, label);
            double npl = num_posts_with_label(label);
            return log(npl_w / npl);
        }
    }

    pair<string, double> expected_label(string content) {
        set<string> words = unique_words(content);
        string guessed_label = (*labels.begin()).first;
        double max = log_sum(words, guessed_label);
        for (auto &p : labels) {
            string label = p.first;
            if (log_sum(words, label) > max) {
                max = log_sum(words, label);
                guessed_label = label;
            }
        }
        pair<string, double> ans;
        ans.first = guessed_label;
        ans.second = max;
        return ans;
    }

    void debug(bool debug) {
        if (debug) {
            cout << "classes:" << endl;
            for (auto &i : labels) {
                cout << "  " << i.first << ", " << i.second.total_num_posts 
                << " examples, log-prior = " << log_score(i.first) << endl;
            }
            cout << "classifier parameters:" << endl;
            for (auto &i : labels) {
                for (auto &w : i.second.words) {
                    cout << "  " << i.first << ":" << w.first << ", count = " 
                    << w.second << ", log-likelihood = " << 
                    log_conditional(w.first, i.first) << endl;
                }
            }
            cout << endl;
        }
        else {
            return;
        }
    }

    private:

    class Label {
        friend Classifier;
    private:
        map<string, int> words;
        int total_num_posts;
        
    public:
        Label() {
            total_num_posts = 0;
        }

        void insert(set<string> post) {
            total_num_posts++;
            for (auto &i : post) {
                words[i] +=1;
            }
        }
    };


    map<string, int> overall_set;
    map<string, Label> labels;
    int total_num_posts;
    int num_unique_words;
};


int main(int argc, char* argv[]){
    cout.precision(3);
    bool debug = false;
        
    if(argc != 3 && argc != 4){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    if(argc == 4){
        if(!strcmp(argv[3],"[--debug]")) {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
        debug = true;
    }
        string trainFile = argv[1];
        string testFile = argv[2];

        csvstream* traincsv;
        csvstream* testcsv;

        try{
            traincsv = new csvstream(trainFile);
        }
        catch(const csvstream_exception &e){
            cout << "Error opening file: " << trainFile << endl;
            return 1;
        }

        try{
            testcsv = new csvstream(testFile);
        }
        catch(const csvstream_exception &e){
            cout << "Error opening file: " << testFile << endl;
            return 1;
        }
    
        Classifier c;
        c.train(traincsv, debug);
        c.debug(debug);
        c.test(testcsv);

        delete traincsv;
        delete testcsv;

    /*
        
*/
    return 0;
}
