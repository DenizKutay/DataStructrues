#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    Sector* lastAdded;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* findInsertion(Sector* bstRoot,int x,int y,int z, Sector* parent);
    bool checkAvailability(Sector* sector);
    void inOrder(Sector* root);
    void preOrder(Sector* root);
    void postOrder(Sector* root);
    bool colorFlip(Sector* parent);
    bool findPathRecursive(Sector *current, std::string target, std::vector<Sector *> &path);
    Sector* changeRoot();
    void deleteTree(Sector* sector);
};

#endif // SPACESECTORLLRBT_H
