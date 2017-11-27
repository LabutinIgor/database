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

#include <tuple>

#include "pgetnextnode.h"

PGetNextNode::PGetNextNode() {
}

PGetNextNode::PGetNextNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* prototype) :
        left(left), right(right), prototype(prototype) {

    Initialize();
}

void PGetNextNode::Initialize() {
    return;
}

Block PGetNextNode::GetNext() {
    if (data.size() > currentBlockPos) {
        return data[currentBlockPos++];
    }
    Block res;
    if (finished) {
        return res;
    }
    res = ProduceNextBlock();
    if (res.size() > 0) {
        currentBlockPos++;
        data.push_back(res);
    } else {
        finished = true;
    }
    return res;
}

int PGetNextNode::GetAttrNum() {
    return prototype->fieldNames.size();
}

void PGetNextNode::Reset() {
    currentBlockPos = 0;
    pos = 0;
}

Block PGetNextNode::ProduceNextBlock() {
    return Block();
}


std::tuple<ErrCode, std::vector<Value>> PGetNextNode::GetRecord() {
    std::vector<Value> vals;
    if (currentBlockPos == 0 && pos == 0) {
        currentBlockToReturn = GetNext();
    }
    if (currentBlockToReturn.size() == 0) {
        return std::make_tuple(EC_FINISH, vals);
    }
    for (int i = 0; i < GetAttrNum(); i++) {
        vals.push_back(currentBlockToReturn[pos][i]);
    }
    pos++;
    if (pos == currentBlockToReturn.size()) {
        currentBlockToReturn = GetNext();
        pos = 0;
    }
    return std::make_tuple(EC_OK, vals);
}

PGetNextNode::~PGetNextNode() {
}
