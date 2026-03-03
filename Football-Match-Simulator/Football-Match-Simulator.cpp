#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

class Player {
public:
    // Intialise player attributes
    std::string name;
    int rating;

    // Constructor to initialise player attributes
    Player(std::string n, int r) {
        name = n;
        rating = r;
    }

    void printStats() {
        std::cout << "Player: " << name << " | Rating: " << rating << std::endl;
    }
};


// --- CLASS 2: TEAM ---
class Team {
public:
    std::string teamName;
    std::vector<Player> squad;
    int goalsScored; // NEW: Keep track of goals

    Team(std::string name) : teamName(name), goalsScored(0) {}

    void addPlayer(std::string n, int s) {
        Player newPlayer(n, s);
        squad.push_back(newPlayer);
    }
};
class MatchSimulator {
private:

    Team* homeTeam;
    Team* awayTeam;
    std::mt19937 rng; // The C++ Random Number Generator

public:
    MatchSimulator(Team* home, Team* away) {
        homeTeam = home;
        awayTeam = away;
        // Seed the random generator with the current time so every match is different
        rng.seed(std::time(nullptr));
    }

    void playMatch() {
        std::cout << "--- KICK OFF: " << homeTeam->teamName << " vs " << awayTeam->teamName << " ---\n\n";

        // THE GAME LOOP: 1 to 90 minutes
        for (int minute = 1; minute <= 90; ++minute) {

            // Generate a random number between 1 and 100
            std::uniform_int_distribution<int> chanceDist(1, 100);
            int eventRoll = chanceDist(rng);

            // A 2% chance per minute that the Home Team scores
            if (eventRoll <= 2) {
                homeTeam->goalsScored++;
                std::cout << "[" << minute << "'] GOAL for " << homeTeam->teamName << "!\n";
            }
            // A 2% chance per minute that the Away Team scores (numbers 3 and 4)
            else if (eventRoll > 2 && eventRoll <= 4) {
                awayTeam->goalsScored++;
                std::cout << "[" << minute << "'] GOAL for " << awayTeam->teamName << "!\n";
            }
        }

        // The match is over, print the final score
        std::cout << "\n--- FULL TIME ---\n";
        std::cout << homeTeam->teamName << ": " << homeTeam->goalsScored << "\n";
        std::cout << awayTeam->teamName << ": " << awayTeam->goalsScored << "\n";
    }
};
int main() {
    // 1. Create a Team
    Team home("Arsenal");

    // 2. Add Players to the Team
    home.addPlayer("Bukayo Saka", 88);
    home.addPlayer("Martin Odegaard", 87);
    home.addPlayer("Declan Rice", 86);

    Team away("Manchester City");
    away.addPlayer("Erling Haaland", 91);
    away.addPlayer("Kevin De Bruyne", 90);

    // 2. Create the Engine & pass the teams
    // We use the '&' symbol to pass the "memory address" (pointer) of the teams
	MatchSimulator simulator(&home, &away);

    // 3. Play the game!
    simulator.playMatch();

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}