
#ifndef ENGINE_PSORTJOINNODE_H
#define ENGINE_PSORTJOINNODE_H


#include "pgetnextnode.h"

class PSortJoinNode  : public PGetNextNode {
public:
    PSortJoinNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PSortJoinNode();
    virtual void Initialize();
    virtual void Print(int indent);
    virtual Block ProduceNextBlock();
private:
    void addToJoin(std::vector<Value> l, Block rBlock, Block& res);

    Block leftBlock;
    Block rightBlock;
    int posInLBlock = 0;
    int posInRBlock = 0;
    int lpos = 0;
    int rpos = 0;
    std::vector<std::vector<std::string>> ln;
    std::vector<std::vector<std::string>> rn;
    ValueType vt;
    Block equalValuesInRBlock = Block();
};


#endif //ENGINE_PSORTJOINNODE_H
