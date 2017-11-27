
#include "PProjectNode.h"

PProjectNode::PProjectNode(PGetNextNode* left, LAbstractNode* p) : PGetNextNode(left, NULL, p) {
    Initialize();
}

Block PProjectNode::ProduceNextBlock() {
    Block res;
    while (leftBlock.size() > 0 && res.size() < BLOCK_SIZE) {
        while (posInLBlock < leftBlock.size() && res.size() < BLOCK_SIZE) {
            std::vector<Value> tmp;
            for (int k = 0; k < ln.size(); k++) {
                if (std::find(names.begin(), names.end(), ln[k]) != names.end()) {
                    tmp.push_back(leftBlock[posInLBlock][k]);
                }
            }
            res.push_back(tmp);
            inputSize++;
            posInLBlock++;
        }

        if (posInLBlock == leftBlock.size()) {
            leftBlock = left->GetNext();
            posInLBlock = 0;
        }
    }
    outputSize += res.size();
    return res;
}

void PProjectNode::Initialize() {
    PGetNextNode* l = left;
    LAbstractNode* lp = l->prototype;
    ln = lp->fieldNames;
    leftBlock = l->GetNext();
    names = prototype->fieldNames;
}

PProjectNode::~PProjectNode() {
    delete left;
}

void PProjectNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::vector<std::string> tokeep = ((LProjectNode*) prototype)->offsets;
    std::cout << "PROJECT, tokeep: ";
    for (int i = 0; i < tokeep.size(); i++) {
        std::cout << tokeep[i] << " ";
    }
    std::cout << "\n";
    left->Print(indent + 2);
}
