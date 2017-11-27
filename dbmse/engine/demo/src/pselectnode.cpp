// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>

#include <string.h>
#include <vector>
#include <sstream>

#include "pselectnode.h"

PSelectNode::PSelectNode() {}

PSelectNode::PSelectNode(LAbstractNode* p) : PGetNextNode() {
    this->table = ((LSelectNode*) p)->GetBaseTable();
    this->prototype = p;
    pos = 0;
    data.clear();
    Initialize();
}

PSelectNode::~PSelectNode() {
}

bool predicateAccepts(Predicate predicate, Value val) {
    Value valToCompare = predicate.valueToCompare;
    if (predicate.ptype == PT_EQUALS) {
        return valToCompare == val;
    } else if (predicate.ptype == PT_GREATERTHAN) {
        return valToCompare < val;
    }
    return false;
}

void PSelectNode::Initialize() {
    std::string line, word;
    std::ifstream f("../data/" + table.relpath);
    if (f.is_open()) {
        // skipping first 4 lines
        getline(f, line);
        getline(f, line);
        getline(f, line);
        getline(f, line);

        LSelectNode* p = (LSelectNode*) prototype;
        while (getline(f, line)) {
            bool predicateTrue = true;
            std::vector<Value> tmp;
            std::istringstream iss(line, std::istringstream::in);
            int i = 0;
            while (iss >> word) {
                Value h;
                if (prototype->fieldTypes[i] == VT_INT)
                    h = Value(std::stoi(word));
                else
                    h = Value(word);

                p->ResetIterator();
                std::tuple<int, Predicate> tup = p->GetNextPredicate();
                while (std::get<0>(tup) == 0) {
                    Predicate predicate = std::get<1>(tup);
                    if (predicate.attribute == i && predicate.valueToCompare.vtype == h.vtype) {
                        if (!predicateAccepts(predicate, h)) {
                            predicateTrue = false;
                            break;
                        }
                    }
                    tup = p->GetNextPredicate();
                }

                tmp.push_back(h);
                i++;
            }
            inputSize++;
            outputNoPredicateSize++;
            if (predicateTrue) {
                if (data.size() == 0 || data.back().size() == BLOCK_SIZE) {
                    data.push_back(Block());
                }
                data.back().push_back(tmp);
                outputSize++;
            }
        }
        f.close();
    } else std::cout << "Unable to open file";
}

void PSelectNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "SCAN " << table.relpath << " with predicate ";

    LSelectNode* p = (LSelectNode*) prototype;
    p->ResetIterator();
    std::tuple<int, Predicate> tup = p->GetNextPredicate();
    if (std::get<0>(tup) == 1) {
        std::cout << "NULL" << std::endl;
    } else {
        while (std::get<0>(tup) == 0) {
            std::cout << std::get<1>(tup);
            tup = p->GetNextPredicate();
        }
        std::cout << "\n";
    }

    if (left != NULL) left->Print(indent + 2);
    if (right != NULL) right->Print(indent + 2);
}

