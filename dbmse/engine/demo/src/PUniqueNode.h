
#ifndef ENGINE_PUNIQUENODE_H
#define ENGINE_PUNIQUENODE_H


#include "pgetnextnode.h"

class PUniqueNode : public PGetNextNode {
public:
    PUniqueNode(PGetNextNode* left, LAbstractNode* p);
    ~PUniqueNode();
    virtual void Initialize();
    virtual void Print(int indent);
private:
    std::vector<std::vector<std::string>> names;
    Block leftBlock;
    int posInLBlock = 0;
    std::vector<std::vector<std::string>> ln;
};


#endif //ENGINE_PUNIQUENODE_H
