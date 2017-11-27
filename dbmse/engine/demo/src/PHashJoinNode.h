
#ifndef ENGINE_PHASHJOINNODE_H
#define ENGINE_PHASHJOINNODE_H


#include <unordered_map>
#include "pgetnextnode.h"

class PHashJoinNode : public PGetNextNode {
public:
    PHashJoinNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PHashJoinNode();
    virtual void Initialize();
    virtual void Print(int indent);

    int accumulatedSize = 0;
private:
    void addToJoin(std::vector<Value> l, std::vector<Value> r);
    Block leftBlock;
    Block rightBlock;
    int lpos = 0;
    int rpos = 0;
    std::vector<std::vector<std::string>> ln;
    std::vector<std::vector<std::string>> rn;
    ValueType vt;
};


#endif //ENGINE_PHASHJOINNODE_H
