#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
private:
    string getMessage(string command);
    int findClient(vector<Client> clients,const string& id);    //find clients index in clients array
    int createFrames(const string &message, vector<Client> &clients, Client &sender, const Client &receiver, const int &message_limit, const string &sender_port, const string &receiver_port);    //returns number of frames
    int findLoga(const string &message, Client client,int number, int msgLimit);
};

#endif  // NETWORK_H
