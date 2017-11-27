
#include "PUnionNode.h"

PUnionNode::PUnionNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p) : PGetNextNode(left, right, p) {
    Initialize();
}

PUnionNode::~PUnionNode() {
    delete left;
    delete right;
}

Block PUnionNode::ProduceNextBlock() {
    Block res = currentBlock;

    if (isCurrentBlockLeft) {
        currentBlock = left->GetNext();
        if (currentBlock.size() == 0) {
            currentBlock = right->GetNext();
            isCurrentBlockLeft = false;
        }
    } else {
        currentBlock = right->GetNext();
    }

    inputSize += res.size();
    outputSize += res.size();
    return res;
}

void PUnionNode::Initialize() {
    currentBlock = left->GetNext();
    if (currentBlock.size() == 0) {
        currentBlock = right->GetNext();
        isCurrentBlockLeft = false;
    }
}

void PUnionNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "UNION: " << std::endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}
