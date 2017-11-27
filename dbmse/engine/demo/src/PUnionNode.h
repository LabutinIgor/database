

#ifndef ENGINE_PUNIONNODE_H
#define ENGINE_PUNIONNODE_H


#include "pgetnextnode.h"

class PUnionNode : public PGetNextNode {
public:
    PUnionNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PUnionNode();
    virtual void Initialize();
    virtual void Print(int indent);
    virtual Block ProduceNextBlock();
private:
    Block currentBlock;
    bool isCurrentBlockLeft = true;
};


#endif //ENGINE_PUNIONNODE_H
