
#include "PUniqueNode.h"

PUniqueNode::PUniqueNode(PGetNextNode* left, LAbstractNode* p) : PGetNextNode(left, NULL, p) {
    Initialize();
}

bool checkEqualData(std::vector<Value> v1, std::vector<Value> v2) {
    if (v1.size() != v2.size()) {
        return false;
    }

    for (int i = 0; i < v1.size(); i++) {
        if (v1[i].vtype != v2[i].vtype || v1[i].vint != v2[i].vint || v1[i].vstr != v2[i].vstr) {
            return false;
        }
    }

    return true;
}

void PUniqueNode::Initialize() {
    PGetNextNode* l = left;
    LAbstractNode* lp = l->prototype;
    std::vector<std::vector<std::string>> ln = lp->fieldNames;
    Block lres = l->GetNext();
    while (lres.size() > 0) {
        inputSize += lres.size();
        for (int i = 0; i < lres.size(); i++) {
            std::vector<Value> tmp = lres[i];
            bool duplicate = false;
            for (int j = 0; j < data.size(); j++) {
                for (int k = 0; k < data[j].size(); k++) {
                    if (checkEqualData(data[j][k], tmp)) {
                        duplicate = true;
                    }
                }
            }
            if (!duplicate) {
                if (data.size() == 0 || data.back().size() == BLOCK_SIZE) {
                    data.push_back(Block());
                }
                data.back().push_back(tmp);
                outputSize++;
            }
        }
        lres = l->GetNext();
    }
}

PUniqueNode::~PUniqueNode() {
    delete left;
}

void PUniqueNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "UNIQUE: " << std::endl;
    left->Print(indent + 2);
}
