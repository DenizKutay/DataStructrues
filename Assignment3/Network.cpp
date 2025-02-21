#include "Network.h"
#include <cstdlib>

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    for(int i = 0 ; i < commands.size() ; i++) {
        std::istringstream iss(commands[i]);
        std::string value;
        vector<string> command;
        //split string according to space
        iss >> value;
        command.push_back(value);
        string strip;
        for(int j = 0 ; j < commands[i].size() + 9;j++) {
            strip += "-";
        }

        cout << strip << "\n";
        cout <<"Command: " << commands[i] << "\n";
        cout << strip << "\n";

        if (command[0] == "MESSAGE") {
            //sender
            iss >> value;
            command.push_back(value);
            string senderName = command[1];
            Client* sender = &clients[findClient(clients,senderName)];  //sender client
            //receiver
            iss >> value;
            command.push_back(value);
            string receiverName = command[2];
            Client* receiver = &clients[findClient(clients,receiverName)];  //receiver client

            //getting message details
            string message = getMessage(commands[i]);
            cout << "Message to be sent: " << '"' << message << '"' << "\n\n";

            int nof = createFrames(message, clients, *sender, *receiver, message_limit, sender_port, receiver_port); //set frames and get the number of frames

            //timestamp
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            char buffer[20];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
            std::string timestamp(buffer);

            //log
            Log log(timestamp,message,nof,0,senderName,receiverName, true,ActivityType::MESSAGE_SENT);
            sender->log_entries.push_back(log);

        }
        else if (command[0] == "SHOW_FRAME_INFO")
        {
            //client name
            iss >> value;
            command.push_back(value);
            string name = command[1];
            Client* client = &clients[findClient(clients,name)];

            //select queue
            iss >> value;
            command.push_back(value);
            string selectionQueue = command[2];


            //frame number
            iss >> value;
            int number = stoi(value);

            string choose;
            if(selectionQueue == "out") {
                choose = "outgoing";
            } else {
                choose = "incoming";
            }

            int size = 0;
            if(selectionQueue == "out") {
                size = client->outgoing_queue.size();
            } else if(selectionQueue == "in") {
                size = client->incoming_queue.size();
            }
            if (selectionQueue == "out") {
                if(number > client->outgoing_queue.size()) {
                    cout<< "No such frame.\n";
                } else {
                    cout << "Current Frame #" << number <<" on the "<< choose <<" queue of client " << name << "\n";
                    queue<stack<Packet *>> temp = client->outgoing_queue;
                    for (int frames = 0; frames < size; frames++) {
                        stack<Packet *> frame = temp.front();
                        if (frames + 1 == number) {
                            vector<Packet *> tempFrame;
                            for (int packet = 0; packet < 4; packet++) {
                                Packet *packet1 = frame.top();
                                tempFrame.push_back(packet1);
                                frame.pop();
                            }
                            string msg;
                            int hop = 0;
                            for (int packet = 3; packet >= 0; packet--) {
                                Packet *packet1 = tempFrame[packet];
                                if (packet == 3) {
                                    ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(packet1);
                                    msg = applicationLayerPacket->message_data;
                                    cout << "Carried Message: " << '"' << applicationLayerPacket->message_data << '"'
                                         << "\n";
                                    hop = applicationLayerPacket->hopCount;
                                }
                                cout << "Layer " << packet1->layer_ID << " info: ";
                                packet1->print();
                            }
                            cout << "Number of hops so far: " << hop << "\n";
                        }
                        temp.pop();
                    }
                }
            } else if (selectionQueue == "in") {
                if(number > client->incoming_queue.size()) {
                    cout<< "No such frame.\n";
                } else {
                    cout << "Current Frame #" << number << " on the " << choose << " queue of client " << name << "\n";
                    queue<stack<Packet *>> temp = client->incoming_queue;
                    for (int frames = 0; frames < size; frames++) {
                        stack<Packet *> frame = temp.front();
                        if (frames + 1 == number) {
                            vector<Packet *> tempFrame;
                            for (int packet = 0; packet < 4; packet++) {
                                Packet *packet1 = frame.top();
                                tempFrame.push_back(packet1);
                                frame.pop();
                            }
                            int hop = 0;
                            for (int packet = 3; packet >= 0; packet--) {
                                Packet *packet1 = tempFrame[packet];
                                string msg;

                                if (packet == 3) {
                                    ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(packet1);
                                    msg = applicationLayerPacket->message_data;
                                    cout << "Carried Message: " << '"' << applicationLayerPacket->message_data << '"'
                                         << "\n";
                                    hop = applicationLayerPacket->hopCount;

                                }
                                cout << "Layer " << packet1->layer_ID << " info: ";
                                packet1->print();
                            }
                            cout << "Number of hops so far: " << hop << "\n";
                        }
                        temp.pop();

                    }
                }
            }
        }
        else if (command[0] == "SHOW_Q_INFO") {
            iss >> value;
            command.push_back(value);
            string clientName = command[1];
            Client* client = &clients[(findClient(clients,clientName))];

            iss >> value;
            command.push_back(value);
            string selectQueue = command[2];
            if(selectQueue == "out") {
                selectQueue = "Outgoing";
            } else if (selectQueue == "in") {
                selectQueue = "Incoming";
            }
            cout << "Client " << client->client_id << " " << selectQueue << " Queue Status\n";
            if(selectQueue == "Outgoing") {
                cout << "Current total number of frames: " << client->outgoing_queue.size() << "\n";
            } else if(selectQueue == "Incoming") {
                cout << "Current total number of frames: " << client->incoming_queue.size() << "\n";
            }


        } else if (command[0] == "SEND") {
            for(Client& client:clients) {
                if(client.outgoing_queue.empty()) {
                    continue;
                } else {
                    int temp = client.outgoing_queue.size();
                    int frameCount = 0;
                    for(int j = 0 ; j < temp;j++) {
                        frameCount++;
                        stack<Packet*> packet = client.outgoing_queue.front();
                        client.outgoing_queue.pop();
                        PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(packet.top());
                        Client* s = &clients[findClient(clients,physicalLayerPacket->receiver_MAC_address)];
                        s->incoming_queue.push(packet);

                        packet.pop();
                        Packet* network = packet.top();

                        packet.pop();
                        Packet* transport = packet.top();

                        packet.pop();
                        ApplicationLayerPacket *application = dynamic_cast<ApplicationLayerPacket*>(packet.top());
                        cout << "Client " << client.client_id  << " sending frame #"<< to_string(frameCount) <<" to client "<<s->client_id << "\n";
                        physicalLayerPacket->print();
                        network->print();
                        transport->print();
                        application->print();
                        application->hopCount++;
                        cout << "Message chunk carried: " <<'"' << application->message_data << '"'<<"\n";
                        cout << "Number of hops so far: " << application->hopCount << "\n";
                        cout << "--------\n";
                        if(application->message_data.back() == '.' ||application->message_data.back() == '!' || application->message_data.back() == '?') {
                            frameCount = 0;
                        }
                    }
                }
            }
        } else if (command[0] == "RECEIVE") {
            for(Client& client:clients) {
                if(client.incoming_queue.empty()) {
                    continue;
                } else {
                    int size = client.incoming_queue.size();
                    int frameCount = 0;
                    string message;
                    for(int j = 0 ; j < size;j++) {
                        stack<Packet*> frame = client.incoming_queue.front();
                        stack<Packet*> tempFrame = frame;
                        PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(frame.top());
                        frame.pop();
                        NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket*>(frame.top());
                        frame.pop();
                        TransportLayerPacket* transport = dynamic_cast<TransportLayerPacket*>(frame.top());
                        frame.pop();
                        ApplicationLayerPacket *application = dynamic_cast<ApplicationLayerPacket*>(frame.top());
                        Client* aimClient = &clients[findClient(clients,application->receiver_ID)];
                        bool kuto = true;


                        //case that message sent to the receiver
                        if(client.client_ip == aimClient->client_ip) {
                            frameCount++;
                            Client* pre = &clients[findClient(clients,physicalLayerPacket->sender_MAC_address)];
                            cout << "Client " << client.client_id <<" receiving frame #"<< frameCount<<" from client " <<pre->client_id << ", originating from client " << application->sender_ID << "\n";
                            physicalLayerPacket->print();
                            networkLayerPacket->print();
                            transport->print();
                            application->print();
                            message += application->message_data;
                            cout << "Message chunk carried: " << '"' << application->message_data << '"' << "\n";
                            cout << "Number of hops so far: " << application->hopCount << "\n";
                            cout << "--------\n";
                            stack<Packet*> stacks = client.incoming_queue.front();
                            client.incoming_queue.pop();

                            if(application->message_data.back() == '.' ||application->message_data.back() == '!' || application->message_data.back() == '?') {
                                std::time_t now = std::time(nullptr);
                                std::tm* localTime = std::localtime(&now);
                                char buffer[20];
                                std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
                                std::string timestamp(buffer);
                                Log log1(timestamp,message,frameCount,application->hopCount,application->sender_ID,application->receiver_ID,true,ActivityType::MESSAGE_RECEIVED);
                                cout << "Client " << log1.receiver_id <<" received the message "<< '"' << log1.message_content << '"' <<" from client " << log1.sender_id<<".\n";
                                cout << "--------\n";
                                message = "";
                                client.log_entries.push_back(log1);
                                frameCount = 0;
                            }
                        }
                        //case that Forwarding
                        else {
                            frameCount++;
                            Client* pre = &clients[findClient(clients,physicalLayerPacket->sender_MAC_address)];
                            stack<Packet*> frame = client.incoming_queue.front();
                            stack<Packet*> tempFrame = frame;
                            PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(frame.top());
                            frame.pop();
                            NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket*>(frame.top());
                            frame.pop();
                            TransportLayerPacket* transport = dynamic_cast<TransportLayerPacket*>(frame.top());
                            frame.pop();
                            ApplicationLayerPacket *application = dynamic_cast<ApplicationLayerPacket*>(frame.top());
                            string nextRouteName = client.nextRoute(application->receiver_ID);
                            if(nextRouteName == "") {
                                int logClientIndex;
                                if(!kuto) {
                                    logClientIndex = findLoga(application->message_data, client, frameCount,
                                                              message_limit);
                                    client.log_entries[logClientIndex].activity_type = ActivityType::MESSAGE_DROPPED;
                                    client.log_entries[logClientIndex].success_status = false;
                                    kuto = true;
                                }
                                message += application->message_data;
                                client.incoming_queue.pop();
                                cout << "Client "<< client.client_id<< " receiving frame #"<< frameCount << " from client "<<pre->client_id<<", but intended for client "<< application->receiver_ID <<". Forwarding... \n";
                                cout << "Error: Unreachable destination. Packets are dropped after "<< application->hopCount <<" hops!\n";
                                if (application->message_data.back() == '.' || application->message_data.back() == '!' || application->message_data.back() == '?') {
                                    std::time_t now = std::time(nullptr);
                                    std::tm* localTime = std::localtime(&now);
                                    char buffer[20];
                                    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
                                    std::string timestamp(buffer);
                                    Log log1(timestamp,message,frameCount,application->hopCount,application->sender_ID,application->receiver_ID,false,ActivityType::MESSAGE_DROPPED);
                                    message = "";
                                    client.log_entries.push_back(log1);
                                    frameCount = 0;
                                    cout << "--------\n";
                                }
                            } else {
                                Client *nextRoute = &clients[findClient(clients, nextRouteName)];
                                physicalLayerPacket->sender_MAC_address = client.client_mac;
                                physicalLayerPacket->receiver_MAC_address = nextRoute->client_mac;
                                if(frameCount == 1) {
                                    cout << "Client "<< client.client_id<< " receiving a message from client "<<pre->client_id<<", but intended for client "<< application->receiver_ID <<". Forwarding... \n";
                                }
                                cout << "Frame #" << frameCount << " MAC address change: New sender MAC "
                                     << physicalLayerPacket->sender_MAC_address << ", new receiver MAC "
                                     << physicalLayerPacket->receiver_MAC_address << "\n";
                                message += application->message_data;
                                tempFrame.pop();
                                tempFrame.push(physicalLayerPacket);
                                client.outgoing_queue.push(tempFrame);

                                client.incoming_queue.pop();
                                if (application->message_data.back() == '.' ||
                                    application->message_data.back() == '!' ||
                                    application->message_data.back() == '?') {
                                    std::time_t now = std::time(nullptr);
                                    std::tm* localTime = std::localtime(&now);
                                    char buffer[20];
                                    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
                                    std::string timestamp(buffer);
                                    Log log1(timestamp,message,frameCount,application->hopCount,application->sender_ID,application->receiver_ID,true,ActivityType::MESSAGE_FORWARDED);
                                    message = "";
                                    client.log_entries.push_back(log1);
                                    frameCount = 0;
                                    cout << "--------\n";
                                }
                            }
                        }

                    }
                }
            }

        } else if (command[0] == "PRINT_LOG") {
            iss >> value;
            command.push_back(value);
            string name = command[1];
            Client* client = &clients[findClient(clients,name)];
            int logCount = 1;
            if(!client->log_entries.empty()) {
                cout << "Client " << name << " Logs:\n";
            }
            for(Log log1:client->log_entries) {
                cout << "--------------\n";
                string activity;
                if(log1.activity_type == ActivityType::MESSAGE_FORWARDED) {
                    activity = "Message Forwarded";
                } else if(log1.activity_type == ActivityType::MESSAGE_RECEIVED) {
                    activity = "Message Received";
                } else if(log1.activity_type == ActivityType::MESSAGE_SENT) {
                    activity = "Message Sent";
                } else {
                    activity = "Message Dropped";
                }
                cout << "Log Entry #"<< logCount<<":\n";
                cout << "Activity: " << activity<<"\n";
                cout << "Timestamp: " << log1.timestamp <<"\n";
                cout << "Number of frames: " << log1.number_of_frames<<"\n";
                cout << "Number of hops: " << log1.number_of_hops << "\n";
                cout << "Sender ID: " << log1.sender_id << "\n";
                cout << "Receiver ID: " << log1.receiver_id << "\n";
                string bools;
                if(log1.success_status) {
                    bools = "Yes";
                } else {
                    bools = "No";
                }
                cout << "Success: " << bools << "\n";
                if(name == log1.receiver_id || name ==log1.sender_id) {
                    cout << "Message: " << '"' << log1.message_content << '"' << "\n";
                }
                logCount++;
            }
        } else {
            cout << "Invalid command.\n";
        }
    }

    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;     //vector of clients
    std::ifstream inputFile(filename);  //input file
    std::string line;//current line
    string memberCount;
    //if file can not open
    if (!inputFile.is_open()) {
        cerr << "Can't open the file." << endl;
        exit(-1);   //exit
    }

    std::getline(inputFile,memberCount);
    //read the input file read by line
    for (int i = 0 ; i < std::stoi(memberCount) ; i++) {
        std::getline(inputFile,line );
        std::istringstream iss(line);
        std::string value;
        vector<string> details;
        //details
        while (iss >> value) {
            details.push_back(value);
        }
        string id = details[0];
        string ip = details[1];
        string mac = details[2];
        Client client(id,ip,mac);
        clients.push_back(client);
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    std::ifstream inputFile(filename);
    std::string line;
    std::vector<std::vector<std::vector<bool>>> shapes;
    if (!inputFile.is_open()) {
        cerr << "Can't open the file." << endl;
        exit(-1);
    }
    int clientCount = 0;
    while (std::getline(inputFile, line)) {
        if(line == "-") {
            clientCount++;
        } else {
            std::istringstream iss(line);
            std::string value;
            vector<string> details;
            while (iss >> value) {
                details.push_back(value);
            }
            string receiverId = details[0];
            string senderId = details[1];
            clients[clientCount].routing_table[receiverId] = senderId;
        }
    }
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    vector<Client> clients;     //vector of clients
    std::ifstream inputFile(filename);  //input file
    std::string line;//current line
    string commandCount;
    //if file can not open
    if (!inputFile.is_open()) {
        cerr << "Can't open the file." << endl;
        exit(-1);   //exit
    }
    std::getline(inputFile, commandCount);
    //read the input file read by line
    for (int i = 0; i < std::stoi(commandCount);i++) {
        std::getline(inputFile, line);
        commands.push_back(line);
        // TODO: Read commands from the given input file and return them as a vector of strings.
    }
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}

string Network::getMessage(string command) {
    string message;
    bool inMessage = false;
    for(char ch : command) {
        if(ch == '#') {
            inMessage = !inMessage;
            continue;
        }if(inMessage) {
            message += ch;
        }
    }
    return message;
}

int Network::createFrames(const string &message, vector<Client> &clients, Client &sender, const Client &receiver, const int &message_limit, const string &sender_port, const string &receiver_port) {
    string chunkMessage = message;
    string nextRouteName = sender.nextRoute(receiver.client_id);
    Client* nextRoute = &clients[findClient(clients,nextRouteName)];    //next path
    int frameCount = 0;
    while(!chunkMessage.empty()) {
        stack<Packet*> frame;   //frame
        string messagePart = chunkMessage.substr(0, message_limit); //splitting the string
        chunkMessage.erase(0, message_limit);
        //creating packets
        ApplicationLayerPacket* applicationLayerPacket = new ApplicationLayerPacket(0,sender.client_id,receiver.client_id,messagePart);
        Packet* transportLayerPacket = new TransportLayerPacket(1,sender_port,receiver_port);
        Packet* networkLayerPacket = new NetworkLayerPacket(2,sender.client_ip,receiver.client_ip);
        Packet* physicalLayerPacket = new PhysicalLayerPacket(3,sender.client_mac,nextRoute->client_mac);

        //adding to them to frame
        frame.push(applicationLayerPacket);
        frame.push(transportLayerPacket);
        frame.push(networkLayerPacket);
        frame.push(physicalLayerPacket);

        //printing the details
        cout << "Frame #" << frameCount + 1 << "\n";
        physicalLayerPacket->print();
        networkLayerPacket->print();
        transportLayerPacket->print();
        applicationLayerPacket->print();
        cout<< "Message chunk carried: " << '"' << applicationLayerPacket->message_data << '"' << "\n";
        cout << "Number of hops so far: 0\n";
        cout << "--------" << "\n";
        frameCount++;
        sender.outgoing_queue.push(frame);
    }
    return frameCount;
}

int Network::findClient(vector<Client> clients, const string &id) {
    int index = 0;
    for(Client &client:clients) {
        if(client.client_id == id || client.client_mac == id || client.client_ip == id) {
            return index;
        }
        index++;
    }
}

int Network::findLoga(const string &message, Client client,int number, int msgLimit) {
    int index = 0;
    for (Log& log1: client.log_entries) {
        string deneme = log1.message_content.substr(msgLimit * (number - 1),  message.size());
        if (message == log1.message_content.substr(msgLimit * (number - 1),  message.size())) {
            return index;
        }
        index++;
    }
}


