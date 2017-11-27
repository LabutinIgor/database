
#ifndef ENGINE_PPROJECTIONNODE_H
#define ENGINE_PPROJECTIONNODE_H


#include "pgetnextnode.h"

class PProjectNode : public PGetNextNode {
public:
    PProjectNode(PGetNextNode* left, LAbstractNode* p);
    ~PProjectNode();
    virtual Block ProduceNextBlock();
    virtual void Initialize();
    virtual void Print(int indent);
private:
    std::vector<std::vector<std::string>> names;
    Block leftBlock;
    int posInLBlock = 0;
    std::vector<std::vector<std::string>> ln;
};


#endif //ENGINE_PPROJECTIONNODE_H
