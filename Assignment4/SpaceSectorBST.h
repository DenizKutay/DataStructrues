#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* findInsertion(Sector* sector,int x,int y,int z);
    Sector* findSectorParent(const std::string& name, Sector *root);
    Sector* inorderSuccesor(Sector* sector);
    void inOrder(Sector* root);
    void preOrder(Sector* root);
    void postOrder(Sector* root);
    bool findPathRecursive(Sector* current, std::string target, std::vector<Sector*>& path);
    void deleteTree(Sector* sector);
};

#endif // SPACESECTORBST_H
