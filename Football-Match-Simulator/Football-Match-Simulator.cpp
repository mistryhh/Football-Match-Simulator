#include <iostream>
#include <string>
#include <vector>

class Player {
public:
    std::string name;
    int skill;

    Player(std::string n, int s) : name(n), skill(s) {}

    void printStats() {
        std::cout << "  - " << name << " (Skill: " << skill << ")" << std::endl;
    }
};

// --- CLASS 2: TEAM ---
class Team {
public:
    std::string teamName;
    std::vector<Player> squad; // A list that holds 'Player' objects

    Team(std::string name) : teamName(name) {}

    // Function to add a player to the list
    void addPlayer(std::string n, int s) {
        // 1. Create the player
        Player newPlayer(n, s);
        // 2. Push them into the vector (list)
        squad.push_back(newPlayer); 
    }

    // Function to show the whole team
    void showSquad() {
        std::cout << "Squad for " << teamName << ":" << std::endl;

        // Call the player's own print function
        for (Player p : squad) {
            p.printStats(); 
        }
        std::cout << "-------------------------" << std::endl;
    }
};

int main() {
    // 1. Create a Team
    Team myTeam("Arsenal");

    // 2. Add Players to the Team
    myTeam.addPlayer("Bukayo Saka", 88);
    myTeam.addPlayer("Martin Odegaard", 87);
    myTeam.addPlayer("Declan Rice", 86);

    // 3. Show the Team
    myTeam.showSquad();

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}