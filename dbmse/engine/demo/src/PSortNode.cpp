
#include "PSortNode.h"


PSortNode::PSortNode(PGetNextNode* left, LAbstractNode* p) : PGetNextNode(left, NULL, p) {
    Initialize();
}

PSortNode::~PSortNode() {
    delete left;
}

void PSortNode::Initialize() {
    LAbstractNode* lp = left->prototype;
    std::vector<std::vector<std::string>> ln = lp->fieldNames;
    int lpos = 0;

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

    Block block = left->GetNext();
    Block allData = Block();
    while (block.size() != 0) {
        allData.insert(allData.end(), block.begin(), block.end());
        block = left->GetNext();
    }
    inputSize += allData.size();
    outputSize += allData.size();

    std::sort(allData.begin(), allData.end(),
              [lpos](std::vector<Value> x, std::vector<Value> y) -> bool {
                  return x[lpos] < y[lpos];
              }
    );

    for (int i = 0; i < allData.size(); i++) {
        if (data.size() == 0 || data.back().size() == BLOCK_SIZE) {
            data.push_back(Block());
        }
        data.back().push_back(allData[i]);
    }
}

void PSortNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "SORT:" << std::endl;
    left->Print(indent + 2);
}
