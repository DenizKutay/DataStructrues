#include <stack>
#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    deleteTree(root);
    // Free any dynamically allocated memory in this class.
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        std::exit(-1);
    }
    std::string line;
    std::getline(inputFile,line);   //first line is X,Y,Z
    while (std::getline(inputFile, line)) {
        int x;
        int y;
        int z;
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
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    findInsertion(root,x,y,z);
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    Sector* sector = findSectorParent(sector_code, root);  //find the parent
    Sector* child;
    Sector* temp = new Sector(-1,-1,-1);    //temp root
    temp->right = root;
    root = temp;
    if(sector == nullptr) {
        if(sector_code == root->right->sector_code) {
            sector = temp;
        } else {
            root = root->right;
            return;
        }
    }
    if (sector->left != nullptr && sector->left->sector_code == sector_code) {
        child = sector->left;
    }
    else {
        child = sector->right;
    }
    //no child scenario
    if(child->right == nullptr && child->left == nullptr) {
        if(sector->right == child) {
            sector->right = nullptr;
        } else {
            sector->left = nullptr;
        }
        delete child;
    }
    //has 2 two child scenario
    else if (child->right != nullptr && child->left != nullptr) {
        Sector* inorderSuccesorSector = inorderSuccesor(child->right);
        Sector* inorderSuccessorParent = findSectorParent(inorderSuccesorSector->sector_code,root);
        if(sector->right == child) {
            sector->right = inorderSuccesorSector;
        } else {
            sector->left = inorderSuccesorSector;
        }
            if(inorderSuccessorParent == child) {
                inorderSuccessorParent->right = nullptr;
                if(inorderSuccesorSector->right != nullptr) {
                    inorderSuccessorParent->right = inorderSuccesorSector->right;
                }
            } else {
                inorderSuccessorParent->left = nullptr;
            }
            if(inorderSuccesorSector->right != nullptr && inorderSuccessorParent != child) {
                inorderSuccessorParent->left = inorderSuccesorSector->right;
            }
            inorderSuccesorSector->right = child->right;
            inorderSuccesorSector->left = child->left;
        delete child;
    }
    //has 1 child scenario
    else {
        Sector* childChild;
        if(child->right!= nullptr) {
            childChild = child->right;
        } else {
            childChild = child->left;
        }
        if(sector->right == child) {
            sector->right = childChild;
        } else {
            sector->left = childChild;
        }
        delete child;
    }
    root = temp->right;
    delete temp;
    // TODO: Delete the sector given by its sector_code from the BST.
}

void SpaceSectorBST::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    inOrder(root);
    std::cout << "\n";
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
}

void SpaceSectorBST::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    preOrder(root);
    std::cout << "\n";
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorBST::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    postOrder(root);
    std::cout << "\n";
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    findPathRecursive(root,sector_code,path);
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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
Sector *SpaceSectorBST::findInsertion(Sector* bstRoot,int x,int y,int z) {
    if(root == nullptr) {
        Sector* sector = new Sector(x,y,z);
        this->root = sector;
        return root;
    }if(bstRoot == nullptr) {
        Sector* kutay = new Sector(x,y,z);
        return kutay;
    }else {
        if(bstRoot->isGraterThanSector(x,y,z)) {
            bstRoot->left = findInsertion(bstRoot->left,x,y,z);
        } else {
            bstRoot->right = findInsertion(bstRoot->right,x,y,z);
        }
    }
    return bstRoot;
}



Sector *SpaceSectorBST::findSectorParent(const std::string& name, Sector *root) {
    std::stack<Sector*> stack;
    stack.push(root);
    while(!stack.empty()) {
        Sector* current = stack.top();
        stack.pop();
        if(current->right != nullptr) {
            if(current->right->sector_code == name) {
                return current;
            }
        }if(current->left != nullptr) {
            if(current->left->sector_code == name) {
                return current;
            }
        }
        if(current->right != nullptr) {
            stack.push(current->right);
        }
        if(current->left != nullptr) {
            stack.push(current->left);
        }
    }
    return nullptr;
}

Sector *SpaceSectorBST::inorderSuccesor(Sector *sector) {
    Sector* temp;
    while(sector!= nullptr) {
        temp = sector;
        sector = sector->left;
    }
    return temp;
}

void SpaceSectorBST::inOrder(Sector* root) {
    if(root == nullptr) {
        return;
    }
    inOrder(root->left);
    std::cout << root->sector_code << "\n";
    inOrder(root->right);
}

void SpaceSectorBST::preOrder(Sector *root) {
    if(root == nullptr) {
        return;
    }
    std::cout << root->sector_code << "\n";
    preOrder(root->left);
    preOrder(root->right);
}

void SpaceSectorBST::postOrder(Sector *root) {
    if(root == nullptr) {
        return;
    }

    postOrder(root->left);
    postOrder(root->right);
    std::cout << root->sector_code << "\n";
}

bool SpaceSectorBST::findPathRecursive(Sector *current, std::string target, vector<Sector *> &path) {
    if (current == nullptr) {
        return false;
    }

    path.push_back(current);

    if (current->sector_code == target) {
        // Node found, return true to indicate success
        return true;
    }

    // Recursively search in the left and right subtrees
    if (findPathRecursive(current->left, target, path) || findPathRecursive(current->right, target, path)) {
        return true;
    }

    // If the target is not found in the current subtree, remove the current node from the path
    path.pop_back();
    return false;
}

void SpaceSectorBST::deleteTree(Sector *sector) {
    if(sector!= nullptr) {
        deleteTree(sector->left);
        deleteTree(sector->right);
        delete sector;
    }

}


