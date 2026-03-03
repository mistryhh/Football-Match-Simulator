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

class Team {
public:
    std::string teamName;
    std::vector<Player> squad;
    // Keep track of goals
    int goalsScored; 

    Team(std::string name) : teamName(name), goalsScored(0) {}

    void addPlayer(std::string n, int s) {
        Player newPlayer(n, s);
        squad.push_back(newPlayer);
    }

    int getTeamRating() {
        if (squad.empty()) {
            // Avoid division by zero
            return 0; 
        }
        int totalRating = 0;
        for (const Player& p : squad) {
            totalRating += p.rating;
        }
        // Average = total / number of players
		return totalRating / squad.size(); 
	}

};
class MatchSimulator {
private:
    Team* homeTeam;
    Team* awayTeam;
    // The C++ Random Number Generator
    std::mt19937 rng; 


public:
    MatchSimulator(Team* home, Team* away) {
        homeTeam = home;
        awayTeam = away;
        // Seed the random generator with the current time so every match is different
        rng.seed(std::time(nullptr));
    }

    void playMatch() {
		int homeRating = homeTeam->getTeamRating();
		int awayRating = awayTeam->getTeamRating();
		int totalRating = homeRating + awayRating;

        std::cout << "--- KICK OFF ---\n";
        std::cout << homeTeam->teamName << " (OVR: " << homeRating << ") vs ";
        std::cout << awayTeam->teamName << " (OVR: " << awayRating << ")\n\n";

        // THE GAME LOOP: 1 to 90 minutes
        for (int minute = 1; minute <= 90; ++minute) {

            // 5 % chance of an "Attack" happening every minute
            std::uniform_int_distribution<int> eventDist(1, 100);
            if (eventDist(rng) <= 5) {
				std::uniform_int_distribution<int> teamDist(1, totalRating);
				int attackRoll = teamDist(rng);

                if (attackRoll <= homeRating) {
					simulateEvent(homeTeam, minute);
                }
                else {
					simulateEvent(awayTeam, minute);
                }

            }
        }

        // The match is over, print the final score
        std::cout << "\n--- FULL TIME ---\n";
        std::cout << homeTeam->teamName << ": " << homeTeam->goalsScored << "\n";
        std::cout << awayTeam->teamName << ": " << awayTeam->goalsScored << "\n";
    }
    //A separate function just for handling a shot on goal
    void simulateEvent(Team* attackingTeam, int minute) {
        std::uniform_int_distribution<int> shotDist(1, 100);

        // 30% chance that an attack turns into a goal
        if (shotDist(rng) <= 30) {
            attackingTeam->goalsScored++;
            std::cout << "[" << minute << "'] GOAL!!! " << attackingTeam->teamName << " scores!\n";
        }
        else {
            std::cout << "[" << minute << "'] " << attackingTeam->teamName << " attacks, but the shot is saved.\n";
        }
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

    // 3. Play the game
    simulator.playMatch();

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}