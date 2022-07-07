#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cctype>

using namespace std;

class Question {
private:
    string question;
    int difficulty;
    vector<string> answers;
    int correct_answer;
public:
    Question(const string& question, const int& difficulty, const vector<string>& answers, const int& correct_answer)
    :question(question)
    ,difficulty(difficulty)
    ,answers(answers)
    ,correct_answer(correct_answer)
    {
        if (question.empty()) {
            throw invalid_argument("Provided question is invalid!");
        }
    
        if (difficulty < 1 && difficulty > 10) {
            throw invalid_argument("Provided difficulty is invalid!");
        }

        if(answers.size() != 4) {
            throw invalid_argument("Provided answers are invalid!");
        }

        if (correct_answer < 0 && correct_answer > 3) {
            throw invalid_argument("Provided index is invalid!");
        }
    }
    Question(const Question& other) {
        this->question = other.question;
        this->difficulty = other.difficulty;
        this->answers = other.answers;
        this->correct_answer = other.correct_answer;
    }
    Question& operator=(const Question& other) {
        if (this != &other) {
            this->question = other.question;
            this->difficulty = other.difficulty;
            this->answers = other.answers;
            this->correct_answer = other.correct_answer;
        } return *this;
    }

    const string& get_question() const { 
        return this->question; 
    }
    const int& get_difficulty() const { 
        return this->difficulty; 
    }
    const vector<string>& get_answers() const {
        return this->answers; 
    }
    const int& get_correct_answer() const {
        return this->correct_answer;
    }

    void set_question(const string& question) {
        if (question.empty()) {
            throw invalid_argument("Provided question is invalid!");
        }
        this->question = question;
    }
    void set_difficulty(const int& difficulty) {
        if (difficulty < 1 && difficulty > 10) {
            throw invalid_argument("Provided difficulty is invalid!");
        }
        this->difficulty = difficulty;
    }
    void set_answers(const vector<string>& answers) {
        if(answers.size() != 4) {
            throw invalid_argument("Provided answers are invalid!");
        }
        
        this->answers = answers;
    }
    void set_correct_answer(const int& correct_answer) {
        if (correct_answer < 0 && correct_answer > 3) {
            throw invalid_argument("Provided index is invalid!");
        }
        this->correct_answer = correct_answer;
    }
};

class Game {
private:
    unordered_map<string, vector<Question> > questions;

    char cipherChar(char plaintext, const char key)
    {
        char result;

        if (isalpha(plaintext))
        {
            result = (result - 'A' + key) % 26 + 'A';
        }
        else
        {
            result = (result - '0' + key) % 10 + '0';
        }

        return result;
    }
    string cipher(const string plaintext, const string key)
    {
        string result;
        size_t index = 0;

        size_t sizeOfPlaintext = plaintext.size();
        size_t sizeOfKey = key.size();
        for (int i = 0; i < sizeOfPlaintext; i++)
        {
            result += cipherChar(plaintext[i], key[index]);
            
            index++;
            if (index == sizeOfKey)
            {
                index = 0;
            }
        }

        return result;
    }
    char decipherChar(char plaintext, const char key)
    {
        char result;

        if (isalpha(plaintext))
        {
            result = (result - 'A' - key) % 26 + 'A';
        }
        else
        {
            result = (result - '0' - key) % 10 + '0';
        }

        return result;
    }
    string decipher(const string plaintext, const string key)
    {
        string result;
        size_t index = 0;

        size_t sizeOfPlaintext = plaintext.size();
        size_t sizeOfKey = key.size();
        for (int i = 0; i < sizeOfPlaintext; i++)
        {
            result += decipherChar(plaintext[i], key[index]);
            
            index++;
            if (index == sizeOfKey)
            {
                index = 0;
            }
        }

        return result;
    }
public:
    Game(const vector<Question>& questions) {
    	vector<Question> easy, medium, hard;
    	
    	for (Question q: questions) {
    		if (q.get_difficulty() >= 1 && q.get_difficulty() <= 3) {
    			easy.push_back(q);
    		} else if (q.get_difficulty() >= 4 && q.get_difficulty() <= 6) {
    			medium.push_back(q);
    		} else {
    			hard.push_back(q);
    		}
    	}
    	
    	this->questions["easy"] = easy;
    	this->questions["medium"] = medium;
    	this->questions["hard"] = hard;
    }
    Game(const Game& other) {
        this->questions = other.questions;
    }
    Game operator=(const Game& other) {
        if (this != &other) {
            this->questions = other.questions;
        } return *this;
    }
    
    string _get_mode(const int& difficulty) {
        if (difficulty >= 1 && difficulty <= 3) {
    		return "easy";
    	} else if (difficulty >= 4 && difficulty <= 6) {
    		return "medium";
    	} else {
    		return "hard";
    	}
    }
    
    void add_question(const string& question, const int& difficulty, const vector<string>& answers, const int& correct_answer) {
        string mode = _get_mode(difficulty);
        
        for (Question q: this->questions[mode]) {
    		if (q.get_question() == question) {
    			throw invalid_argument("There is already such a question!");
    		}
        } this->questions[mode].push_back(Question(question, difficulty, answers, correct_answer));   
    }
    
    void _edit_question(Question& question) {
        string new_question;
        cout << "New question: ";
        cin >> new_question;
        question.set_question(new_question);
    }
    void _edit_difficulty(Question& question) {
        int new_difficulty;
        cout << "New difficulty: ";
        cin >> new_difficulty;
        question.set_difficulty(new_difficulty);
    }
    void _edit_answers(Question& question) {
        vector<string> new_answers;
        
        for (int i = 0; i < 4; i++) {
            string new_answer;
            cout << "New answer " << i << ": ";
            cin >> new_answer;
            new_answers.push_back(new_answer);
        }
        question.set_answers(new_answers);
    }
    void _edit_correct_answer(Question& question) {
        int new_correct_answer;
        cout << "New correct answer: ";
        cin >> new_correct_answer;
        question.set_correct_answer(new_correct_answer);
    }
    void edit_question(const string& question, const int& difficulty) {
        string mode = _get_mode(difficulty);
    	int size = this->questions[mode].size();
        
        for (int i = 0; i < size; i++) {
            if (this->questions[mode][i].get_question() == question) {
                int input;
                while(1) {
                    cout << "Q: " << this->questions[mode][i].get_question() << "\nDifficulty: " << this->questions[mode][i].get_difficulty() << "\nAs:\n";
                    
                    vector<string> old_answers = this->questions[mode][i].get_answers();
                    int size_answers = old_answers.size();
                    
                    for (int i = 0; i < size_answers; i++) {
                        cout << old_answers[i] << "\n";
                    }
                    
                    cout << "Correct answer: [" << this->questions[mode][i].get_correct_answer() << "] " 
                        << old_answers[this->questions[mode][i].get_correct_answer()] << "\n" 
                        << "What do you want to edit?\n" 
                        << "1) Question\n"
                        << "2) Difficulty\n"
                        << "3) Answers\n" 
                        << "4) Correct answer\n"
                        << "5) Save\n"
                        << "Enter your choice: ";
                    
                    cin >> input;
                    switch(input) {
                        case 1:
                            _edit_question(this->questions[mode][i]);
                            break;
                        case 2:
                            _edit_difficulty(this->questions[mode][i]);
                            break;
                        case 3:
                            _edit_answers(this->questions[mode][i]);
                            break;
                        case 4:
                            _edit_correct_answer(this->questions[mode][i]);
                            break;
                        case 5:
                            return;
                            break;
                        default: 
                            cout << "Incorect answer";
                    }
                }
            }
        }
        
    }

    void writeToFile(const string& filename)
    {
        if (this->questions.empty())
        {
            throw invalid_argument("There are no questions to be saved in the file");
        }

        ofstream outputFile(filename);

        if (outputFile.is_open())
        {
            for (string mode: {"easy", "medium", "hard"})
            {
                for (Question q: this->questions[mode])
                {
                    outputFile << q.get_question() << endl;
                    outputFile << q.get_difficulty() << endl;
                    
                    vector<string> answers = q.get_answers();
                    for (int i = 0; i < 4; i++)
                    {
                        outputFile << answers[i] << endl;
                    }
                    answers.clear();

                    outputFile << q.get_correct_answer() << endl;
                }
            }
        }
        else
        {
            throw invalid_argument("Can't open the file");
        }
        
        outputFile.close();
    }
    void readFromFile(const string& filename)
    {
        string line;

        vector<string> arguments;
        size_t sizeOfArguments;

        vector<string> answers;
        
        ifstream inputFile(filename);

        if (inputFile.is_open())
        {
            while (getline(inputFile, line))
            {
                arguments.push_back(line);
            }

            sizeOfArguments = arguments.size();
            for (int i = 0; i < sizeOfArguments; i += 7)
            {   
                for (int j = 0; j < 4; j++)
                {
                    answers.push_back(arguments.at(i + 2 + j));
                }

                add_question(arguments.at(i), stoi(arguments.at(i + 1)), answers, stoi(arguments.at(i + 6)));

                answers.clear();
            }
        }
        else
        {
            throw invalid_argument("Can't open the file");
        }

        inputFile.close();
    }

    void cipherFile(const string& filename, const string& key)
    {
        vector<string> answers;
        
        readFromFile(filename);

        for (string mode: {"easy", "medium", "hard"})
        {
            for (Question q: this->questions[mode])
            {
                q.set_question(cipher(q.get_question(), key));
                
                q.set_difficulty(stoi(cipher(to_string(q.get_difficulty()), key)));

                answers = q.get_answers();

                for (int i = 0; i < 4; i++)
                {
                    answers[i] = cipher(answers[i], key);
                }
                q.set_answers(answers);

                answers.clear();

                q.set_correct_answer(stoi(cipher(to_string(q.get_correct_answer()), key)));
            }
        }

        writeToFile(filename);
    }
    void decipherFile(const string& filename, const string& key)
    {
        vector<string> answers;
        
        readFromFile(filename);

        for (string mode: {"easy", "medium", "hard"})
        {
            for (Question q: this->questions[mode])
            {
                q.set_question(decipher(q.get_question(), key));
                
                q.set_difficulty(stoi(decipher(to_string(q.get_difficulty()), key)));

                answers = q.get_answers();

                for (int i = 0; i < 4; i++)
                {
                    answers[i] = decipher(answers[i], key);
                }
                q.set_answers(answers);

                answers.clear();

                q.set_correct_answer(stoi(decipher(to_string(q.get_correct_answer()), key)));
            }
        }

        writeToFile(filename);
    }
};

int main()
{
    return 0;
}
    
