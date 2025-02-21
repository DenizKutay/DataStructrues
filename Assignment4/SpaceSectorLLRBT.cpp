#include <stack>
#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        std::exit(-1);
    }
    std::string line;
    std::getline(inputFile,line);   //first line is X,Y,Z
    while (std::getline(inputFile, line)) {
        int x, y ,z;
        int commaCount = 0;
        string number;
        //scan the characters in line
        for(char ch : line) {
            if(ch != ',') { //if it is not comma
                number += ch;
            }
            else {  //if it is comma
                if(commaCount == 0) {
                    x = stoi(number);   //initialize x when there is no comma in the line
                    number = "";            //reset the number
                } else if (commaCount == 1) {
                    y = stoi(number);   //initialize y when there is 1 comma in the line
                    number = "";            //reset the number
                }
                commaCount++;               //increase the comma count
            }
        }
        z = stoi(number);   //initialize when line is finished without comma
        insertSectorByCoordinates(x,y,z);
    }
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    deleteTree(root);
    // TODO: Free any dynamically allocated memory in this class.
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    Sector* newRoot = findInsertion(root,x,y,z, nullptr);
    root = newRoot;
    checkAvailability(lastAdded);

    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    inOrder(root);
    std::cout << "\n";
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    preOrder(root);
    std::cout << "\n";
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    postOrder(root);
    std::cout << "\n";
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    vector<Sector*> world;
    vector<Sector*> target;
    findPathRecursive(root,"0SSS",world);
    findPathRecursive(root,sector_code,target);
    if(target.empty()) {
        return path;
    }
    if("0SSS" == sector_code) {
        path.push_back(world.back());
        return path;
    }
    Sector* last;
    int index = 0;
    while (world[index] == target[index]) {
        last = world[index];
        index++;
        if(world.size() == index || target.size() == index) {
            break;
        }
    }
    int a = world.size() - index;
    for (int i = 0 ; i < a;i++ ) {
        path.push_back(world.back());
        world.pop_back();
    }
    path.push_back(last);
    while(index < target.size()) {
        path.push_back(target[index]);
        index++;
    }
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if(path.empty()) {
        std::cout << "A path to Dr. Elara could not be found.\n\n";
    } else {
        std::cout << "The stellar path to Dr. Elara: ";
        int pathSize = 0;
        for (Sector *sector: path) {
            pathSize++;
            std::cout << sector->sector_code;
            if (pathSize != path.size()) {
                std::cout << "->";
            }
        }
        cout << "\n";
    }
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}
Sector* SpaceSectorLLRBT::findInsertion(Sector* bstRoot, int x, int y, int z, Sector* parent) {
    if (root == nullptr) {
        Sector* sector = new Sector(x, y, z);
        sector->parent = parent;  // Set the parent pointer
        this->root = sector;
        root->color = BLACK;
        lastAdded = sector;
        return root;
    } else if (bstRoot == nullptr) {
        Sector* kutay = new Sector(x, y, z);
        kutay->color = RED;
        kutay->parent = parent;  // Set the parent pointer
        lastAdded = kutay;
        return kutay;
    } else {
        if (bstRoot->isGraterThanSector(x, y, z)) {
            bstRoot->left = findInsertion(bstRoot->left, x, y, z, bstRoot);  // Pass current node as parent
        } else {
            bstRoot->right = findInsertion(bstRoot->right, x, y, z, bstRoot);  // Pass current node as parent
        }
    }
    return bstRoot;
}

bool SpaceSectorLLRBT::checkAvailability(Sector *sector) {
    Sector *parent = sector->parent;
    bool colorFlipDone = false;
    if(sector != root) {
        if(colorFlip(parent)) {
            colorFlipDone = true;
        }
    }
    if(sector != root) {
        //right lean red tree
        if (sector->isGraterThanSector(parent->x, parent->y, parent->z) && sector->color) {
            Sector* subTree = parent->parent;
            parent->right = sector->left;
            if(sector->left != nullptr) {
                sector->left->parent = parent;
            }
            sector->parent = parent->parent;
            if(subTree != nullptr) {
                if (subTree->right == parent) {
                    subTree->right = sector;
                } else {
                    subTree->left = sector;
                }
            }
            parent->parent = sector;
            sector->left = parent;
            sector->color = parent->color;
            parent->color = RED;
            Sector* temp = sector;
            sector = parent;
            parent = temp;
        }
    }
    changeRoot();
    //two red links in a row
    if(parent != root && parent != nullptr && sector != root) {
        if(parent->color && sector->color) {
            Sector* parentParent = parent->parent;
            Sector* subTreeRoot = parentParent->parent;
            if(subTreeRoot != nullptr) {
                if (subTreeRoot->right == parentParent) {
                    subTreeRoot->right = parent;
                } else {
                    subTreeRoot->left = parent;
                }
            }
            parent->parent = subTreeRoot;
            if(parent->right != nullptr) {
                Sector *parentOfRight = parent->right;
                parentOfRight->parent = parentParent;
                parentParent->left = parentOfRight;
            } else {
                parentParent->left = nullptr;
            }
            parent->right = parentParent;
            parentParent->parent = parent;
            parent->color = parentParent->color;
            parentParent->color = RED;
        }
    }
    changeRoot();
    if(sector != root) {
            if (colorFlip(parent)) {
                colorFlipDone = true;
            }
    }
    if (colorFlipDone) {
        checkAvailability(parent);}
    return false;
}

void SpaceSectorLLRBT::inOrder(Sector *root) {
    if(root == nullptr) {
        return;
    }
    inOrder(root->left);
    std::string color;
    color = root->color ? "RED":"BLACK";
    std::cout << color << " sector: " << root->sector_code << "\n";
    inOrder(root->right);
}

void SpaceSectorLLRBT::preOrder(Sector *root) {
    if(root == nullptr) {
        return;
    }
    std::string color;
    color = root->color ? "RED":"BLACK";
    std::cout << color << " sector: " << root->sector_code << "\n";
    preOrder(root->left);
    preOrder(root->right);
}

void SpaceSectorLLRBT::postOrder(Sector *root) {
    if(root == nullptr) {
        return;
    }

    postOrder(root->left);
    postOrder(root->right);
    std::string color;
    color = root->color ? "RED":"BLACK";
    std::cout << color << " sector: " << root->sector_code << "\n";
    //BLACK sector:
}

bool SpaceSectorLLRBT::colorFlip(Sector *parent) {
    if(parent->left != nullptr && parent->right != nullptr) {
        if(parent->left->color && parent->right->color) {
            parent->left->color = BLACK;
            parent->right->color = BLACK;
            parent->color = !parent->color;
            return true;
        }
    }
    return false;
}

Sector *SpaceSectorLLRBT::changeRoot() {
    Sector* temp = root;
    while (temp->parent != nullptr) {
        temp = temp->parent;
    }
    temp->color = false;
    root = temp;
    return temp;
}

bool SpaceSectorLLRBT::findPathRecursive(Sector *current, std::string target, vector<Sector *> &path) {
    if (current == nullptr) {
        return false;
    }

    path.push_back(current);

    if (current->sector_code == target) {
        // Node found, return true to indicate success
        return true;
    }

    if (findPathRecursive(current->left, target, path) || findPathRecursive(current->right, target, path)) {
        return true;
    }
    path.pop_back();
    return false;
}

void SpaceSectorLLRBT::deleteTree(Sector *sector) {
    if (sector != nullptr) {
        deleteTree(sector->left);
        deleteTree(sector->right);
        delete sector;
    }

}

