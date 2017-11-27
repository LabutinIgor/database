

#include "PSortJoinNode.h"

PSortJoinNode::PSortJoinNode(PGetNextNode* left, PGetNextNode* right,
                             LAbstractNode* p) : PGetNextNode(left, right, p) {
    Initialize();
}

PSortJoinNode::~PSortJoinNode() {
    delete left;
    delete right;
}

void PSortJoinNode::addToJoin(std::vector<Value> l, Block rBlock, Block& res) {
    for (std::vector<Value> r : rBlock) {
        std::vector<Value> tmp;
        for (int k = 0; k < ln.size(); k++) {
            if (k != lpos) {
                tmp.push_back(l[k]);
            }
        }
        for (int k = 0; k < rn.size(); k++) {
            if (k != rpos) {
                tmp.push_back(r[k]);
            }
        }
        tmp.push_back(l[lpos]);

        res.push_back(tmp);
    }
}

Block PSortJoinNode::ProduceNextBlock() {
    Block res;
    while (leftBlock.size() > 0 && rightBlock.size() > 0 && res.size() < BLOCK_SIZE) {
        while (rightBlock.size() > 0 && leftBlock[posInLBlock][lpos] > rightBlock[posInRBlock][rpos]) {
            posInRBlock++;
            if (posInRBlock == rightBlock.size()) {
                rightBlock = right->GetNext();
                inputSize += rightBlock.size();
                posInRBlock = 0;
            }
        }
        if (rightBlock.size() == 0) {
            break;
        }
        if (leftBlock[posInLBlock][lpos] == rightBlock[posInRBlock][rpos]) {
            if (equalValuesInRBlock.size() == 0 || equalValuesInRBlock[0][rpos] != leftBlock[posInLBlock][lpos]) {
                equalValuesInRBlock = Block();
                while (rightBlock.size() > 0 && leftBlock[posInLBlock][lpos] == rightBlock[posInRBlock][rpos]) {
                    equalValuesInRBlock.push_back(rightBlock[posInRBlock]);
                    posInRBlock++;
                    if (posInRBlock == rightBlock.size()) {
                        rightBlock = right->GetNext();
                        inputSize += rightBlock.size();
                        posInRBlock = 0;
                    }
                }
            }
            addToJoin(leftBlock[posInLBlock], equalValuesInRBlock, res);
        }
        posInLBlock++;
        if (posInLBlock == leftBlock.size()) {
            leftBlock = left->GetNext();
            inputSize += leftBlock.size();
            posInLBlock = 0;
        }
    }
    outputSize += res.size();
    return res;
}

void PSortJoinNode::Initialize() {
    leftBlock = left->GetNext();
    rightBlock = right->GetNext();
    inputSize += leftBlock.size() + rightBlock.size();

    LAbstractNode* lp = left->prototype;
    LAbstractNode* rp = right->prototype;
    ln = lp->fieldNames;
    rn = rp->fieldNames;

    for (int i = 0; i < lp->fieldNames.size(); i++) {
        if (std::find(ln[i].begin(), ln[i].end(), ((LJoinNode*) prototype)->offset1) != ln[i].end()) {
            lpos = i;
            break;
        }
        if (std::find(ln[i].begin(), ln[i].end(), ((LJoinNode*) prototype)->offset2) != ln[i].end()) {
            lpos = i;
            break;
        }
    }

    for (int i = 0; i < rp->fieldNames.size(); i++) {
        if (std::find(rn[i].begin(), rn[i].end(), ((LJoinNode*) prototype)->offset1) != rn[i].end()) {
            rpos = i;
            break;
        }
        if (std::find(rn[i].begin(), rn[i].end(), ((LJoinNode*) prototype)->offset2) != rn[i].end()) {
            rpos = i;
            break;
        }
    }

    vt = lp->fieldTypes[lpos];
}

void PSortJoinNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "SORT-JOIN: " << ((LJoinNode*) prototype)->offset1 << "=" << ((LJoinNode*) prototype)->offset2
              << std::endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}