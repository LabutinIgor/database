
#ifndef ENGINE_PCROSSPRODUCTNODE_H
#define ENGINE_PCROSSPRODUCTNODE_H


#include "pgetnextnode.h"

class PCrossProductNode : public PGetNextNode {
public:
    PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PCrossProductNode();
    virtual void Initialize();
    virtual void Print(int indent);
    virtual Block ProduceNextBlock();
private:
    Block leftBlock;
    Block rightBlock;
    int posInRBlock = 0;
    int posInLBlock = 0;
};


#endif //ENGINE_PCROSSPRODUCTNODE_H
