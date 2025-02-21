#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    LeaderboardEntry* temp = this->head_leaderboard_entry;
    int while_count = 0;
    //get the placement
    while(temp != NULL) {
        if(new_entry->score > temp->score) {
            break;
        }
        temp = temp->next_leaderboard_entry;
        while_count++;
    }
    //if it is not top 10
    if (while_count == 10) {
        return;

    }
    //if empty
    if(while_count == 0){
        new_entry->next_leaderboard_entry = this->head_leaderboard_entry;
        this->head_leaderboard_entry = new_entry;

    } else {
        //insert the new player
        LeaderboardEntry* last = this->head_leaderboard_entry;
        for(int i = 0; i < while_count - 1;i++) {
            last = last->next_leaderboard_entry;
        }
        LeaderboardEntry* next = last->next_leaderboard_entry;
        last->next_leaderboard_entry = new_entry;
        new_entry->next_leaderboard_entry = next;
    }
    temp = this->head_leaderboard_entry;
    for(int i = 0 ; i < 9 ; i++) {
        if(temp->next_leaderboard_entry != NULL) {
            temp = temp->next_leaderboard_entry;
        } else {
            break;
        }
    }
    if(temp->next_leaderboard_entry != NULL) {
        LeaderboardEntry* pop = temp->next_leaderboard_entry;
        delete pop;
        temp->next_leaderboard_entry = nullptr;
    }
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
}

void Leaderboard::write_to_file(const string& filename) {

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
    }
    LeaderboardEntry* temp = this->head_leaderboard_entry;
    while(temp != NULL) {
        outFile << temp->score << " " << temp->last_played << " " << temp->player_name << "\n";
        temp = temp->next_leaderboard_entry;
    }
    outFile.close();
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
}

void Leaderboard::read_from_file(const string& filename) {
    std::ifstream inputFile(filename);  //bunları değişcen
    std::string line;
    int whileCount = 0;
    while (std::getline(inputFile, line)){
        if(line == "" && whileCount == 0) {
            this->head_leaderboard_entry = NULL;
            break;
        }
        std::istringstream iss(line);
        std::vector<std::string>details;
        std::string detail;
        int columnCount = 0;
        while (iss >> detail) {
            columnCount++;
            details.push_back(detail);
        }
        long time = std::stoul(details[1]);

        if(whileCount == 0) {
            LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(std::stod(details[0]),time,details[2]);
            this->head_leaderboard_entry = leaderboardEntry;
        } else {
            LeaderboardEntry* temp = this->head_leaderboard_entry;
            LeaderboardEntry* leaderboardEntry = new LeaderboardEntry(std::stod(details[0]),time,details[2]);
            while (temp->next_leaderboard_entry != NULL) {
                temp = temp->next_leaderboard_entry;
            }
            temp->next_leaderboard_entry = leaderboardEntry;
        }
        whileCount++;
    }
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
}


void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard\n-----------\n";
    LeaderboardEntry* temp = this->head_leaderboard_entry;
    int count = 1;
    while(temp != NULL) {
        std::tm* lastPlayed_tm;
        time(&temp->last_played);
        lastPlayed_tm = localtime(&temp->last_played);
        char buffer[100];
        std::strftime(buffer, 100, "%H:%M:%S/%d.%m.%Y", lastPlayed_tm);

        std::cout << count <<". " << temp->player_name << " " << temp->score << " " << buffer << "\n";
        temp = temp->next_leaderboard_entry;
        count++;
    }
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry* temp = this->head_leaderboard_entry;
    while (temp != NULL) {
        LeaderboardEntry* next = temp->next_leaderboard_entry;
        delete temp;
        temp = next;
    }
    head_leaderboard_entry = nullptr;
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
}

