// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include "csvstream.h"
#include <utility>
#include <set>
#include <cmath>
#include <string>
#include <cassert>  //assert
using namespace std;

// EFFECTS: Return a set of unique whitespace delimited words.x
set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}

class Classifier {
public:
//default constructor
    Classifier(){
        num_posts = 0;
        num_unique_words = 0;
    }

//returns a set of strings with all unique words
    set<string> make_bag_of_words(const string &str){
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
          words.insert(word);
        }
        return words;
      }

    //trains the classifier with the file from filename
    void train(string filename, bool debug){
        csvstream csvin(filename);
        map<string, string> map;
        if (debug) {
            cout << "training data:" << "\n";
        }
        while (csvin >> map) {
            const string label = map["tag"];
            const string post = map["content"];
            insert(label, post);
            if (debug) {
                cout << "  label = " << label << ", content = " << post << endl;
            }
        }

        cout << "trained on " << num_posts << " examples" << "\n";
        if (debug) {
            cout << "vocabulary size = " << num_unique_words << "\n";
        }
        cout << "\n";
    }
    void test(string filename){
        cout << "test data:" << "\n";
        int posts = 0;
        int correct = 0;
        csvstream csvin(filename);
        map<string, string> map;
        while (csvin >> map) {
            const string &post = map["content"];
            set<string> bag = make_bag_of_words(post);
            const string &label = map["tag"];
            const string predicted_label = most_likely_label(post);
            
            cout << "  correct = " << label << ", predicted = ";
            cout << predicted_label << ", log-probability score = ";
            cout << log_prob(bag, predicted_label) << endl;
            cout << "  content = " << post << endl << endl;
            posts++;
            if (label == predicted_label) {
                correct++;
            }
        }
        cout << "performance: " << correct << " / " << posts;
        cout << " posts predicted correctly" << endl;
      }
    
    void debug(bool d) {
        if (d) {
            cout << "classes:" << endl;
            for (auto &p : labels) {
                cout << "  " << p.first << ", " << p.second.num_posts;
                cout << " examples, log-prior = " << log_prob(p.first) << endl;
            }
            cout << "classifier parameters:" << endl;
            for (auto &p : labels) {
                for (auto &q : p.second.words) {
                    cout << "  " << p.first << ":" << q.first << ", count = ";
                    cout << q.second << ", log-likelihood = ";
                    cout << log_prob(p.first, q.first) << endl;
                }
            }
            cout << endl;
        }
        else {
            return;
        }
    }
    
    //inserts a new post into the classifier
    void insert(const string label, const string post) {
        set<string> bag = make_bag_of_words(post);
        for (auto &p : bag) {
           allwords[p] += 1;
        }
        labels[label].insert(bag);
        num_posts++;
        num_unique_words = static_cast<int>(allwords.size());
    }

    string most_likely_label(string post) {
        set<string> bag = make_bag_of_words(post);
        string best_label = labels.begin()->first;
        double max = log_prob(bag, best_label);
        for (auto &p : labels) {
            string label = p.first;
            if (log_prob(bag, label) > max) {
                max = log_prob(bag, label);
                best_label = label;
            }
        }
        return best_label;
    }
    
    //returns the sum of the log probabilities of the post having the label
    double log_prob(set<string> post, string label){
        double sum = log_prob(label);
        for (auto &p : post){
            string w = p;
            double a = log_prob(label, w);
            sum = sum + a;
        }
        return sum;
    }
    
    //finds the log probability of finding a post
    //with the given label
    //see formula on the spec
    // word is read in map in training data
    // word wasnt seen with label, but was seen in training data
    // No word was seen at all
    double log_prob(string label){
    //get log prob of that label and content
    //go through all the labels in the training and
    //see which ones have the highest probability score
        double np = static_cast<double>(num_posts);
        double npl = static_cast<double>(num_posts_label(label));
        double aw = allwords[label];
        if(npl != 0){
            return log(npl/np);
        }
        else if(aw != 0){
        //    (Use when  does not occur in posts labeled
            //but does occur in the training data overall.)
            return log(aw/np);
        }
        else{
            return log(1.0/np);
        }

    }
    
    //finds the log probability of finding a post with the given word
    //based on the given label
    //see formula on the spec
    double log_prob(string label, string word) {
        double np = static_cast<double>(num_posts);
        //word not in whole data set
        if (allwords.find(word) == allwords.end()){
            return log(1.0/np);
        }
        //word in whole set but not in label
        else if (num_posts_label_and_word(label, word) == 0) {
         //   double b = static_cast<double>(num_posts_word(word));
            return log(static_cast<double>(num_posts_word(word))/np);
        }
        else{
            double a = static_cast<double>(num_posts_label_and_word(label, word));
            double b = static_cast<double>(num_posts_label(label));
            return log(a/b);
        }
    }
    
    //number of posts in the whole set with word
    int num_posts_word(string word) {
        int total = 0;
        for (auto &p : labels) {
            total += p.second.posts_with_word(word);
        }
        return total;
    }
    
    //number of posts in whole set with label
    int num_posts_label(string label) {
        return labels[label].get_num_posts();
    }
    
    //number of posts with label that contain word
    int num_posts_label_and_word(string label, string word) {
        return labels[label].posts_with_word(word);
    }
    
private:
//    Need to be able to store:
//      -The total number of posts in the entire training set.
//      -The number of unique words in the entire training set. (The vocabulary size.)
//      -For each word, the number of posts in the entire training set that contain .
//      -For each label, the number of posts with that label.
//      -For each label and word, the number of posts with label that contain that word.
    //Label which stores a map of all the words in that label
    //and the number of posts they appear in, as well as the
    //total number of posts in the label
    class Label {
        friend Classifier;
    private:
        //map of all words under that label, with the
        //number of posts that contain that word
        map<string,int> words;
        int num_posts;
        
    public:
        Label() {
            num_posts = 0;
        }
        
        void insert(set<string> post) {
            num_posts++;
            for (auto &p : post) {
                words[p] +=1;
            }
        }

        int posts_with_word (string word) {
            return words[word];
        }
        int get_num_posts() {
            return num_posts;
        }
    };
    //map of all words with number of occurances in the whole set
    map<string, int> allwords;
    
    //map of all labels with a corresponding class Label, which stores a
    //map of all the words in that label and the number of posts they appear
    //in, as well as the total number of posts in the label
    map<string, Label> labels;
//  map of each label with a map of all the words in
//  that set 
    
    int num_posts;          //number of posts
    int num_unique_words;   //number of unique words across all posts
};

void tests();
int main (int argc, char * argv[]) {
    cout.precision(3);
    
    bool debug = false;
    if(!(argc == 3 || argc == 4)) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }

    if (argc == 4) {
        if(!strcmp(argv[3],"[--debug]")) {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
        else {
            debug = true;
        }
    }
    ifstream infile;
    ifstream infile2;
    infile.open(argv[1]);
    infile2.open(argv[2]);

    if(!infile.is_open()){
        cout << "Error opening file: " << argv[1] << endl;
    }
    
    else if(!infile2.is_open()){
        cout << "Error opening file: " << argv[2] << endl;
    }
    
    string train_file = argv[1];
    string test_file = argv[2];
    Classifier classifier;
    classifier.train(train_file,debug);
    classifier.debug(debug);
    classifier.test(test_file);
    return 0;
}

 //GO TO LINE 229 AND 207 AND REMOVE COMMENT WHEN TEST
 void tests() {
     cout << "testing. . ." << endl;
     Classifier classifier;
     classifier.insert("euchre", "the left bower took the trick");
     classifier.insert("euchre", "the right bower took the trick");
     classifier.insert("calculator", "how do you add the first one");
     assert(classifier.num_posts_word("the") == 3);
     assert(classifier.num_posts_label("euchre") == 2);
     assert(classifier.num_posts_label_and_word("euchre", "trick") == 2);
     assert(classifier.num_posts_label_and_word("euchre", "left") == 1);
     assert(classifier.most_likely_label("left bower") == "euchre");
     assert(classifier.most_likely_label("add") == "calculator");
     assert(classifier.most_likely_label("the") == "euchre");
    
     cout << "test passed!!!!!!" << endl << endl;
 }


// // Project UID db1f506d06d84ab787baf250c265e24e

// #include <iostream>
// #include "csvstream.h"
// #include <utility>
// #include <set>
// #include <cmath>
// #include <string>
// #include <cassert> 
// using namespace std;

// class Classifier{
//     public:

//     Classifier(){
//         total_num_posts = 0;
//         num_unique_words = 0;
//     }

//     set<string> make_bag_of_words(const string &str){
//         istringstream source(str);
//         set<string> words;
//         string word;
//         while (source >> word) {
//           words.insert(word);
//         }
//         return words;
//     }

//     //trains the classifier with the file from filename
//     void train(string filename, bool debug){
//         csvstream csvin(filename);
//         map<string, string> map;
//         if (debug) {
//             cout << "training data:" << "\n";
//         }
//         while (csvin >> map) {
//             const string label = map["tag"];
//             const string post = map["content"];
//             set<string> bag = make_bag_of_words(post);
//             for (auto &p : bag) {
//             overall_set[p] += 1;
//             }
//             labels[label].insert(bag);
//             total_num_posts++;
//             num_unique_words = static_cast<int>(overall_set.size());
//             if (debug) {
//                 cout << "  label = " << label << ", content = " << post << endl;
//             }
//         }

//         cout << "trained on " << total_num_posts << " examples" << "\n";
//         if (debug) {
//             cout << "vocabulary size = " << num_unique_words << "\n";
//         }
//         cout << "\n";
//     }


//     void test(string filename) {
//         cout << "test data: " << endl;
//         csvstream csvin(filename);
//         int num_posts = 0;
//         int correct_posts = 0;
//         map<string, string> post;
//         while (csvin >> post) {
//             const string &label = post["tag"];
//             const string &content = post["content"];
//             set<string> words = make_bag_of_words(content);
//             const string predicted_label = expected_label(content);

//             cout << "  correct = " << label << ", predicted = ";
//             cout << predicted_label << ", log-probability score = ";
//             double answer = log_sum(words, predicted_label);
//             cout << answer << endl;
//             cout << "  content = " << content << endl << endl;
//             num_posts++;
//             if (label == predicted_label) {
//                 correct_posts++;
//             }
//         }
//         cout << "performance: " << correct_posts << " / " << num_posts;
//         cout << " posts predicted correctly" << endl;
//     }

 
//     int num_posts_with_word(string word) {
//         int total = 0;
//         for (auto &w : labels) {
//             total += w.second.words[word];
//         }
//         return total;
//     }

//     int num_posts_with_label(string label) {
//         return labels[label].total_num_posts;
//     }

//     int num_posts_label_and_word(string word, string label) {
//         return labels[label].words[word];
//     }


//     //returns the sum of the log probabilities of the post having the label
//     double log_sum(set<string> posts, string label){
//         double sum = log_score(label);
//         for (auto &p : posts){
//             string word = p;
//             double additional_sum = log_conditional(label, word);
//             sum = sum + additional_sum;
//         }
//         return sum;
//     }

//     double log_score(string label){
//         double tnp = total_num_posts;
//         double np_withL = num_posts_with_label(label);
//         if (np_withL != 0) {
//             return log(np_withL / tnp);
//         } else {
//             return log(1.0 / tnp);
//         }
//     }

//     double log_conditional(string label, string word) {
//         double tnp = total_num_posts;
//         if(overall_set.find(word) == overall_set.end()) {
//             return log(1.0/tnp);
//         } else if (num_posts_label_and_word(word, label) == 0) {
//             return log(num_posts_with_word(word) / tnp);
//         } else {
//             double n1 = (num_posts_label_and_word(word, label));
//             double n2 = (num_posts_with_label(label));
//             return log(n1 / n2);
//         }
//     }

//     string expected_label(string content) {
//         set<string> words = make_bag_of_words(content);
//         string best_label = labels.begin()->first;
//         double max = log_sum(words, best_label);
//         for (auto &p : labels) {
//             string label = p.first;
//             if (log_sum(words, label) > max) {
//                 max = log_sum(words, label);
//                 best_label = label;
//             }
//         }
//         return best_label;
//     }

//     void debug(bool debug) {
//         if (debug) {
//             cout << "classes:" << endl;
//             for (auto &p : labels) {
//                 cout << "  " << p.first << ", " << p.second.total_num_posts;
//                 cout << " examples, log-prior = " << log_score(p.first) << endl;
//             }
//             cout << "classifier parameters:" << endl;
//             for (auto &p : labels) {
//                 for (auto &q : p.second.words) {
//                     cout << "  " << p.first << ":" << q.first << ", count = ";
//                     cout << q.second << ", log-likelihood = ";
//                     cout << log_conditional(p.first, q.first) << endl;
//                 }
//             }
//             cout << endl;
//         }
//         else {
//             return;
//         }
//     }

// private:

//     class Label {
//         friend Classifier;
//     private:
//         map<string, int> words;
//         int total_num_posts;
        
//     public:
//         Label() {
//             total_num_posts = 0;
//         }

//         void insert(set<string> post) {
//             total_num_posts++;
//             for (auto &p : post) {
//                 words[p] +=1;
//             }
//         }
//     };

//     map<string, int> overall_set;
//     map<string, Label> labels;
//     int total_num_posts;
//     int num_unique_words;
//     map<string, int> map_of_words;
// };


// int main(int argc, char* argv[]){
//     cout.precision(3);

//     bool debug = false;
//     if(argc != 3 && argc != 4){
//         cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
//         return -1;
//     }
//     if(argc == 4){
//         if(!strcmp(argv[3],"[--debug]")) {
//             cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
//             return -1;
//         }
//         debug = true;
//     }
    

//     ifstream file1;
//     ifstream file2;
//     file1.open(argv[1]);
//     file2.open(argv[2]);

//     if(!file1.is_open()){
//         cout << "Error opening file: " << argv[1] << endl;
//     }
    
//     else if(!file2.is_open()){
//         cout << "Error opening file: " << argv[2] << endl;
//     }

//     string trainFile = argv[1];
//     string testFile = argv[2];
//     Classifier c;
//     c.train(trainFile, debug);
//     c.debug(debug);
//     c.test(testFile);


//     return 0;

// }