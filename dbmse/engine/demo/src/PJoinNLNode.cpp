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

#include<cstddef>
#include<algorithm>

#include "PJoinNLNode.h"

PJoinNLNode::PJoinNLNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p) : PGetNextNode(left, right, p) {
    Initialize();
}

PJoinNLNode::~PJoinNLNode() {
    delete left;
    delete right;
}

void PJoinNLNode::addToJoin(int i, int j, Block& res) {
    bool join = false;
    if (vt == VT_INT) {
        if ((int) leftBlock[i][lpos] == (int) rightBlock[j][rpos]) join = true;
    } else {
        if ((std::string) leftBlock[i][lpos] == (std::string) rightBlock[j][rpos]) join = true;
    }

    if (!join) return;

    std::vector<Value> tmp;

    for (int k = 0; k < ln.size(); k++) {
        if (k != lpos) {
            tmp.push_back(leftBlock[i][k]);
        }
    }

    for (int k = 0; k < rn.size(); k++) {
        if (k != rpos) {
            tmp.push_back(rightBlock[j][k]);
        }
    }

    tmp.push_back(leftBlock[i][lpos]);

    res.push_back(tmp);
}

Block PJoinNLNode::ProduceNextBlock() {
    Block res;

    while (leftBlock.size() > 0 && res.size() < BLOCK_SIZE) {
        while (posInLBlock < leftBlock.size() && res.size() < BLOCK_SIZE) {
            addToJoin(posInLBlock, posInRBlock, res);
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

void PJoinNLNode::Initialize() {
    leftBlock = left->GetNext();
    rightBlock = right->GetNext();

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
    inputSize = left->outputSize + right->outputSize;
}

void PJoinNLNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "NL-JOIN: " << ((LJoinNode*) prototype)->offset1 << "=" << ((LJoinNode*) prototype)->offset2
              << std::endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}
