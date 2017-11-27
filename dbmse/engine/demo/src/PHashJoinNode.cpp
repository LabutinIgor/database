
#include "PHashJoinNode.h"

PHashJoinNode::PHashJoinNode(PGetNextNode* left, PGetNextNode* right,
                             LAbstractNode* p) : PGetNextNode(left, right, p) {
    Initialize();
}

PHashJoinNode::~PHashJoinNode() {
    delete left;
    delete right;
}

void PHashJoinNode::Initialize() {
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

    std::unordered_multimap<std::string, std::vector<Value>> ht_str;
    std::unordered_multimap<int, std::vector<Value>> ht_int;
    leftBlock = left->GetNext();
    while (leftBlock.size() != 0) {
        for (int i = 0; i < leftBlock.size(); i++) {
            accumulatedSize++;
            if (vt == VT_INT) {
                ht_int.insert({{leftBlock[i][lpos].vint, leftBlock[i]}});
            } else {
                ht_str.insert({{leftBlock[i][lpos].vstr, leftBlock[i]}});
            }
        }
        inputSize += leftBlock.size();
        leftBlock = left->GetNext();
    }

    rightBlock = right->GetNext();
    while (rightBlock.size() != 0) {
        for (int i = 0; i < rightBlock.size(); i++) {
            if (vt == VT_INT) {
                auto range = ht_int.equal_range(rightBlock[i][rpos].vint);
                for (auto it = range.first; it != range.second; it++) {
                    addToJoin(it->second, rightBlock[i]);
                }
            } else {
                auto range = ht_str.equal_range(rightBlock[i][rpos].vstr);
                for (auto it = range.first; it != range.second; it++) {
                    addToJoin(it->second, rightBlock[i]);
                }
            }
        }
        inputSize += rightBlock.size();
        rightBlock = right->GetNext();
    }
}

void PHashJoinNode::addToJoin(std::vector<Value> l, std::vector<Value> r) {
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

    if (data.size() == 0 || data.back().size() == BLOCK_SIZE) {
        data.push_back(Block());
    }
    data.back().push_back(tmp);
    outputSize++;
}

void PHashJoinNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "SORT-JOIN: " << ((LJoinNode*) prototype)->offset1 << "=" << ((LJoinNode*) prototype)->offset2
              << std::endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}