#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <ctime>
using namespace std;

struct Candidate {
    string name;
    int votes;
};

struct Voter {
    string userID;
    string password;
    bool hasVoted;
};
const string adminUsername = "Biplob";
const string adminPassword = "11";

void displayLeaderboard(const vector<Candidate>& candidates) {
    bool votesExist = false;

    cout << "\nLeaderboard:\n";

    for (const auto& candidate : candidates) {
        if (candidate.votes > 0) {
            votesExist = true;
            cout << candidate.name << ": " << candidate.votes << " votes\n";
        }
    }

    if (!votesExist) {
        cout << "No votes yet.\n";
    }
}

void vote(vector<Candidate>& candidates, unordered_map<string, Voter>& voters, const string& userID) {
    if (voters[userID].hasVoted) {
        cout << "Vote already casted.\n";
        return;
    }

    cout << "Enter candidate name: ";
    string candidateName;
    cin >> candidateName;

    auto it = find_if(candidates.begin(), candidates.end(), [&candidateName](const Candidate& c) {
        return c.name == candidateName;
    });

    if (it != candidates.end()) {
        it->votes++;
        voters[userID].hasVoted = true;
        cout << "Vote registered for " << candidateName << "!\n";
    } else {
        cout << "Candidate not found.\n";
    }
}

void loadCandidates(vector<Candidate>& candidates) {
    ifstream file("candidates.txt");
    string name;
    int votes;

    if (file.is_open()) {
        while (file >> name >> votes) {
            candidates.push_back({name, votes});
        }
        file.close();
    }
}

void saveCandidates(const vector<Candidate>& candidates) {
    ofstream file("candidates.txt");

    if (file.is_open()) {
        for (const auto& candidate : candidates) {
            file << candidate.name << " " << candidate.votes << endl;
        }
        file.close();
    }
}

void loadVoters(unordered_map<string, Voter>& voters) {
    ifstream file("voters.txt");
    string userID, password;
    bool hasVoted;

    if (file.is_open()) {
        while (file >> userID >> password >> hasVoted) {
            voters[userID] = {userID, password, hasVoted};
        }
        file.close();
    }
}

void saveVoters(const unordered_map<string, Voter>& voters) {
    ofstream file("voters.txt");

    if (file.is_open()) {
        for (const auto& voter : voters) {
            file << voter.first << " " << voter.second.password << " " << voter.second.hasVoted << endl;
        }
        file.close();
    }
}

bool adminLogin() {
    string username, password;
    int attempts = 0;

    while (attempts < 3) {
        cout << "Enter Admin Username: ";
        cin >> username;
        cout << "Enter Admin Password: ";
        cin >> password;

        if (username == adminUsername && password == adminPassword) {
            return true;
        } else {
            cout << "Incorrect username or password. Try again.\n";
            attempts++;
        }
    }

    cout << "Too many attempts. Program will end.\n";
    return false;
}

void registerVoter(unordered_map<string, Voter>& voters) {
    string userID, password;

    cout << "Enter new voter user ID (number): ";
    cin >> userID;
    cout << "Enter new voter password: ";
    cin >> password;

    voters[userID] = {userID, password, false};
    cout << "Voter " << userID << " registered successfully.\n";
}

void registerCandidate(vector<Candidate>& candidates) {
    string name;
    cout << "Enter new candidate name: ";
    cin >> name;
    candidates.push_back({name, 0});
    cout << "Candidate " << name << " registered successfully.\n";
}

void viewVoterDetails(const unordered_map<string, Voter>& voters) {
    if (voters.empty()) {
        cout << "\nNo voters registered yet.\n";
    } else {
        cout << "\nRegistered Voters:\n";
        for (const auto& voter : voters) {
            cout << "User ID: " << voter.first;
            if (voter.second.hasVoted) {
                cout << " (voted)\n";
            } else {
                cout << "\n";
            }
        }
    }
}

void viewCandidateDetails(const vector<Candidate>& candidates) {
    if (candidates.empty()) {
        cout << "\nNo candidates registered yet.\n";
    } else {
        cout << "\nRegistered Candidates:\n";
        for (const auto& candidate : candidates) {
            cout << candidate.name << "\n";
        }
    }
}

bool voterLogin(const unordered_map<string, Voter>& voters, string& userID) {
    string password;

    cout << "Enter Voter User ID: ";
    cin >> userID;
    cout << "Enter Voter Password: ";
    cin >> password;

    if (voters.find(userID) != voters.end() && voters.at(userID).password == password) {
        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Invalid user ID or password.\n";
        return false;
    }
}

void tieBreaker() {
    string candidate1, candidate2, choice;

    cout << "Enter the name of the candidate to flip the coin: ";
    cin >> candidate1;
    cout << "Enter the name of the candidate who prays: ";
    cin >> candidate2;

    cout << "Choose heads or tails (h/t): ";
    cin >> choice;

    while (choice != "h" && choice != "t") {
        cout << "Invalid choice. Please choose 'h' for heads or 't' for tails: ";
        cin >> choice;
    }

    srand(static_cast<unsigned int>(time(0)));
    int coinFlip = rand() % 2;

    cout << "Coin is flipped...\n";
    if ((coinFlip == 0 && choice == "h") || (coinFlip == 1 && choice == "t")) {
        cout << candidate1 << " is the winner of the tiebreaker!\n";
    } else {
        cout << candidate2 << " is the winner of the tiebreaker!\n";
    }
}

int main() {
    vector<Candidate> candidates;
    unordered_map<string, Voter> voters;
    loadCandidates(candidates);
    loadVoters(voters);

    while (true) {
        cout << "\n===============================\n";
        cout << "   Welcome to 'Vote The GOAT'\n";
        cout << "===============================\n";
        cout << "\n1. Admin Section\n2. Voter Section\n3. Tie Breaker\n4. Exit\nChoose an option: ";
        int mainChoice;
        cin >> mainChoice;

        if (mainChoice == 1) {
            if (!adminLogin()) {
                break;
            }

            int adminChoice;
            do {
                cout << "\nAdmin Menu:\n1. Register Voter\n2. Register Candidate\n3. View Voter Details\n4. View Candidate Details\n5. Display Leaderboard \n6. Logout\nChoose an option: ";
                cin >> adminChoice;

                if (adminChoice == 1) {
                    registerVoter(voters);
                    saveVoters(voters);
                } else if (adminChoice == 2) {
                    registerCandidate(candidates);
                    saveCandidates(candidates);
                } else if (adminChoice == 3) {
                    viewVoterDetails(voters);
                } else if (adminChoice == 4) {
                    viewCandidateDetails(candidates);
                } else if (adminChoice == 5) {
                    displayLeaderboard(candidates);    
                } else if (adminChoice == 6) {
                    cout << "Logging out...\n";
                    break;
                } else {
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (adminChoice != 5);
        } else if (mainChoice == 2) {
            string userID;
            if (voterLogin(voters, userID)) {
                if (voters[userID].hasVoted) {
                    cout << "Vote already casted.\n";
                    continue;
                }
                vote(candidates, voters, userID);
                saveVoters(voters);
                saveCandidates(candidates);
            }
        } else if (mainChoice == 3) {
            tieBreaker();
        } else if (mainChoice == 4) {
            cout << "Exiting...\n";
            saveCandidates(candidates);
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
