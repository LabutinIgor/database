
#ifndef ENGINE_PSORTNODE_H
#define ENGINE_PSORTNODE_H


#include "pgetnextnode.h"

class PSortNode : public PGetNextNode {
public:
    PSortNode(PGetNextNode* left, LAbstractNode* p);
    ~PSortNode();
    virtual void Initialize();
    virtual void Print(int indent);
};


#endif //ENGINE_PSORTNODE_H
