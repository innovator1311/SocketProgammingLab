#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <algorithm>

using namespace std;

struct Question{
    string question;
    string answerSentence[4];
    char answer;
};

int randomQuestionNo(int min, int max) //range : [min, max]
{
    static bool first = true;
    if (first) 
    {  
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}
// for string delimiter
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

vector<Question> getQuestion (){

    std::ifstream inFile;
    inFile.open("data.txt");
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    string str = strStream.str();

    string delimiter = "===";
    vector<string> v = split (str, delimiter);

    vector<int> quesIdx;
    vector<Question> SetQues;
    for (int i = 1; i <= 10; i++){
        int quesNo = randomQuestionNo(0,v.size());

        while (std::count(quesIdx.begin(), quesIdx.end(), quesNo))
        {
            int quesNo = randomQuestionNo(0,v.size());
        }

        quesIdx.push_back(quesNo);

        vector<string> quesSet = split (v[quesNo], "\n");
        //vector<string> subvector = {quesSet.begin() + 1, quesSet.end() - 1};
        vector<string> subvector = quesSet;

        Question Q = {
            subvector[1],
            {subvector[5],subvector[2],subvector[3],subvector[4]},
            subvector[6][0]
        };


        SetQues.push_back(Q);
    }
    return SetQues;
}

int main() {

    vector<Question> v = getQuestion();

    for (auto i : v) cout << i.question << endl;

    return 0;
}