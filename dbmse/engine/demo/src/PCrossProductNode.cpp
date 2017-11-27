
#include "PCrossProductNode.h"

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p)
        : PGetNextNode(left, right, p) {
    Initialize();
}

PCrossProductNode::~PCrossProductNode() {
    delete left;
    delete right;
}

Block PCrossProductNode::ProduceNextBlock() {
    Block res;
    while (leftBlock.size() > 0 && res.size() < BLOCK_SIZE) {
        while (posInLBlock < leftBlock.size() && res.size() < BLOCK_SIZE) {
            std::vector<Value> tmp;
            tmp.insert(tmp.end(), leftBlock[posInLBlock].begin(), leftBlock[posInLBlock].end());
            tmp.insert(tmp.end(), rightBlock[posInRBlock].begin(), rightBlock[posInRBlock].end());
            res.push_back(tmp);

            if (posInRBlock + 1 == rightBlock.size()) {
                posInLBlock++;
                posInRBlock = 0;
            } else {
                posInRBlock++;
            }
        }

        if (posInLBlock == leftBlock.size()) {
            rightBlock = right->GetNext();
            if (rightBlock.size() == 0) {
                leftBlock = left->GetNext();
                right->Reset();
                rightBlock = right->GetNext();
            }
            posInLBlock = 0;
            posInRBlock = 0;
        }
    }
    outputSize += res.size();
    return res;
}

void PCrossProductNode::Initialize() {
    leftBlock = left->GetNext();
    rightBlock = right->GetNext();
    inputSize = left->outputSize + right->outputSize;
}

void PCrossProductNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "CROSS-PRODUCT: " << std::endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}
