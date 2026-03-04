#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>

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
	// A simple function to print player stats
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
	// A function to calculate the average team rating based on the players in the squad
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

        std::cout << "\n==================================================\n";
        std::cout << "KICK OFF: " << homeTeam->teamName << " vs " << awayTeam->teamName << "\n";
        std::cout << "==================================================\n\n";

        // THE GAME LOOP: 1 to 90 minutes
        for (int minute = 1; minute <= 90; ++minute) {

			// Halftime at 45 minutes
            if (minute == 45) {
                std::cout << "\n[45'] HALFTIME WHISTLE! Score is " << homeTeam->teamName << " " << homeTeam->goalsScored << " - " << awayTeam->goalsScored << " " << awayTeam->teamName << ".\n\n";
                continue; 
            }

            // 5 % chance of an "Attack" happening every minute
            std::uniform_int_distribution<int> eventDist(1, 100);
			int eventRoll = eventDist(rng);
            // 10% chance of an event happening
            if (eventRoll <= 10) {
				std::uniform_int_distribution<int> teamDist(1, totalRating);

				Team* attacker = (teamDist(rng) <= homeRating) ? homeTeam : awayTeam;
				Team* defender = (attacker == homeTeam) ? awayTeam : homeTeam;

				simulateEvent(attacker, defender, minute);
            }
        }

        // The match is over, print the final score
        std::cout << "\n==================================================\n";
        std::cout << "FULL TIME\n";
        std::cout << homeTeam->teamName << "  " << homeTeam->goalsScored << " - " << awayTeam->goalsScored << "  " << awayTeam->teamName << "\n";
        std::cout << "==================================================\n\n";
    }
    //A separate function just for handling a shot on goal
    void simulateEvent(Team* attacker, Team* defender, int minute) {
        std::uniform_int_distribution<int> chanceDist(1, 100);
        int outcome = chanceDist(rng);

        // Pick a random player 
        std::uniform_int_distribution<int> playerDist(0, attacker->squad.size() - 1);
        std::string activePlayer = attacker->squad[playerDist(rng)].name;
        
        if (outcome <= 30) {
			// 30% chance of a goal being scored
            attacker->goalsScored++;
            std::cout << "[" << minute << "'] GOAL!!! Absolute screamer from " << activePlayer << " for " << attacker->teamName << "!\n";
        }
        else if (outcome <= 60) {
			// 30% chance of a save being made
            std::cout << "[" << minute << "'] " << activePlayer << " gets a shot off, but the " << defender->teamName << " keeper makes a great save!\n";
        }
        else if (outcome <= 80) {
			// 20% chance of a foul being committed
            std::cout << "[" << minute << "'] Heavy tackle! " << activePlayer << " goes into the referee's book with a yellow card.\n";
        }
        else if(outcome <= 90) {
			// 10% chance of a shot going wide
            std::cout << "[" << minute << "'] " << activePlayer << " takes a shot, but it goes wide of the post.\n";
        }
        else {
			// 10% chance of an offside
            std::cout << "[" << minute << "'] " << activePlayer << " is caught offside. Free kick for " << defender->teamName << ".\n";
		}
    }
};
std::vector<Team> loadTeamsDatabase(std::string fileName) {
	std::vector<Team> database;
	std::fstream file(fileName);
	std::string line;

	// Check if the file was opened successfully
    if(!file.is_open()) {
        std::cout << "Error: Could not open " << fileName << ". Make sure it is in the same folder as the code!\n";
        return database;
	}

	//Read file line by line
    while (std::getline(file, line)) {
		std::stringstream ss(line);
        std::string teamName, playerName, playerRating;

        //Split by comma
		std::getline(ss, teamName, ',');
		std::getline(ss, playerName, ',');
		std::getline(ss, playerRating, ',');

        if(teamName.empty() || playerName.empty() || playerRating.empty()) {
            continue; 
		}

		int skill = std::stoi(playerRating);

		// Check if team already exists in the database
		bool teamExists = false;
        for (Team& t : database) {
            if(t.teamName == teamName) {
                t.addPlayer(playerName, skill);
				teamExists = true;
                break;
			}
        }

		// If team doesn't exist, create it and add the player
        if (!teamExists) {
            Team newTeam(teamName);
            newTeam.addPlayer(playerName, skill);
			database.push_back(newTeam);
        }
    }
	return database;
}
int main() {
    // Load the database
    std::vector<Team> allTeams = loadTeamsDatabase("teams.txt");

    if(allTeams.size() < 2) {
        std::cout << "Not enough teams loaded to play a match. Press Enter to exit.";
		std::cin.get();
        return 0;
	}   
    // Main Menu Loop
    while (true) {
        std::cout << "WELCOME TO THE FOOTBALL MATCH SIMULATOR\n";
        std::cout << "Available Teams:\n";
        for (int i = 0; i < allTeams.size(); i++) {
            std::cout << i + 1 << ". " << allTeams[i].teamName << " (OVR: " << allTeams[i].getTeamRating() << ")\n";
        }
        
		int homeChoice, awayChoice;
        std::cout << "\nEnter number for HOME team: ";
		std::cin >> homeChoice;
		std::cout << "Enter number for AWAY team: ";
        std::cin >> awayChoice;

		// Validate Input
        if(homeChoice < 1 || homeChoice > allTeams.size() || awayChoice < 1 || awayChoice > allTeams.size() || homeChoice == awayChoice) {
            std::cout << "\nInvalid choices. Try again.\n\n";
            continue;
		}
        
        // Setup and play the match
        // Arrays are 0-indexed, so we subtract 1 from the user's choice
		Team home = allTeams[homeChoice - 1];
        Team away = allTeams[awayChoice - 1];
        
        // Reset goals to 0 in case we play multiple matches
		home.goalsScored = 0;
        away.goalsScored = 0;

		MatchSimulator simulator(&home, &away);
		simulator.playMatch();

		// Ask to play again
		char playAgain;
        std::cout << "Play another match? (y/n): ";
        std::cin >> playAgain;
        if (playAgain != 'y' && playAgain != 'Y') {
            break; // Exit the loop
        }
        std::cout << "\n\n";
    }
    return 0;
}