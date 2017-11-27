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

#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <tuple>
#include "../../interface/interface.h"
#include "../../interface/basics.h"
#include "pselectnode.h"
#include "PJoinNLNode.h"
#include "PCrossProductNode.h"
#include "PProjectNode.h"
#include "PUniqueNode.h"
#include "PUnionNode.h"
#include "PHashJoinNode.h"
#include "PSortJoinNode.h"
#include "PSortNode.h"

void ExecuteQuery(PGetNextNode* query) {
    std::tuple<ErrCode, std::vector<Value>> res;
    res = query->GetRecord();
    ErrCode ec = std::get<0>(res);
    std::vector<Value> vals = (std::vector<Value>&&) std::get<1>(res);
    while (ec == EC_OK) {
        for (int i = 0; i < query->GetAttrNum(); i++) {
            if (vals[i].vtype == VT_INT)
                std::cout << vals[i].vint << " ";
            else if (vals[i].vtype == VT_STRING)
                std::cout << vals[i].vstr << " ";
        }
        printf("\n");
        res = query->GetRecord();
        ec = std::get<0>(res);
        vals = std::get<1>(res);
    }
}

// Here be rewriter and optimizer
PGetNextNode* QueryFactory(LAbstractNode* node) {
    if (dynamic_cast<LSelectNode*>(node) != NULL) {
        LSelectNode* tmp = (LSelectNode*) node;
        return new PSelectNode(tmp);
    }  else if (dynamic_cast<LSortNode*>(node) != NULL) {
        return new PSortNode(QueryFactory(node->GetLeft()), node);
    } else if (dynamic_cast<LJoinNode*>(node) != NULL) {
        return new PHashJoinNode(QueryFactory(node->GetLeft()), QueryFactory(node->GetRight()), node);
//        return new PJoinNLNode(QueryFactory(node->GetLeft()), QueryFactory(node->GetRight()), node);
//        return new PSortJoinNode(QueryFactory(node->GetLeft()), QueryFactory(node->GetRight()), node);
    } else if (dynamic_cast<LCrossProductNode*>(node) != NULL) {
        return new PCrossProductNode(QueryFactory(node->GetLeft()), QueryFactory(node->GetRight()), node);
    } else if (dynamic_cast<LProjectNode*>(node) != NULL) {
        return new PProjectNode(QueryFactory(node->GetLeft()), node);
    } else if (dynamic_cast<LUniqueNode*>(node) != NULL) {
        return new PUniqueNode(QueryFactory(node->GetLeft()), node);
    } else if (dynamic_cast<LUnionNode*>(node) != NULL) {
        return new PUnionNode(QueryFactory(node->GetLeft()), QueryFactory(node->GetRight()), node);
    } else {
        return NULL;
    }
}


int main() {
    {
        std::cout << "Starting demo" << std::endl;
        std::cout << "Query1: select" << std::endl;
        BaseTable bt1 = BaseTable("table1");
        std::cout << bt1;
        LAbstractNode* n1 = new LSelectNode(bt1, {Predicate(PT_GREATERTHAN, VT_INT, 0, 3, "")});
        PGetNextNode* q1 = QueryFactory(n1);
        q1->Print(0);
        ExecuteQuery(q1);
        std::cout << "Selected " << q1->outputSize << " out of " << ((PSelectNode*) q1)->outputNoPredicateSize
                  << std::endl;
        delete n1;
        delete q1;
    }

    {
        std::cout << std::endl << "Query2: simple equi-join" << std::endl;
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");
        std::cout << bt1;
        std::cout << bt2;
        LAbstractNode* n1 = new LSelectNode(bt1, {});
        LAbstractNode* n2 = new LSelectNode(bt2, {});

        LSortNode* n3 = new LSortNode(n1, "table1.id", "table2.id2");
        LSortNode* n4 = new LSortNode(n2, "table1.id", "table2.id2");
        LJoinNode* n5 = new LJoinNode(n3, n4, "table1.id", "table2.id2", 666);
        PGetNextNode* q1 = QueryFactory(n5);
        q1->Print(0);
        ExecuteQuery(q1);
        std::cout << "Join input size: " << q1->inputSize << ", output size: " << q1->outputSize << std::endl;
        delete n5;
        delete q1;
        std::cout << "\n";
    }

    {
        std::cout << std::endl << "Query3: cross product" << std::endl;
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");
        std::cout << bt1;
        std::cout << bt2;
        LAbstractNode* n1 = new LSelectNode(bt1, {});
        LAbstractNode* n2 = new LSelectNode(bt2, {});
        LCrossProductNode* n3 = new LCrossProductNode(n1, n2);
        PGetNextNode* q1 = QueryFactory(n3);
        q1->Print(0);
        ExecuteQuery(q1);
        std::cout << "Cross product input size: " << q1->inputSize << ", output size: " << q1->outputSize << std::endl;
        delete n3;
        delete q1;
    }

    {
        std::cout << std::endl << "Query4: projection" << std::endl;
        BaseTable bt1 = BaseTable("table1");
        std::cout << bt1;
        LAbstractNode* n1 = new LSelectNode(bt1, {});
        LAbstractNode* n2 = new LProjectNode(n1, {"table1.id", "table1.description"});
        PGetNextNode* q1 = QueryFactory(n2);
        q1->Print(0);
        ExecuteQuery(q1);
        delete n2;
        delete q1;
    }

    {
        std::cout << std::endl << "Query5: unique and predicate" << std::endl;
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");
        std::cout << bt1;
        std::cout << bt2;
        LAbstractNode* n1 = new LSelectNode(bt1, {Predicate(PT_GREATERTHAN, VT_INT, 0, 3, "")});
        LAbstractNode* n2 = new LSelectNode(bt2, {Predicate(PT_GREATERTHAN, VT_INT, 0, 1, "")});
        LCrossProductNode* n3 = new LCrossProductNode(n1, n2);
        LProjectNode* n4 = new LProjectNode(n3, {"table2.type2"});
        LUniqueNode* n5 = new LUniqueNode(n4);
        PGetNextNode* q1 = QueryFactory(n5);
        q1->Print(0);
        ExecuteQuery(q1);
        delete n5;
        delete q1;
    }

    {
        std::cout << std::endl << "Query6: union" << std::endl;
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");
        std::cout << bt1;
        std::cout << bt2;
        LAbstractNode* n1 = new LSelectNode(bt1, {});
        LAbstractNode* n2 = new LSelectNode(bt2, {});
        LProjectNode* n3 = new LProjectNode(n1, {"table1.id"});
        LProjectNode* n4 = new LProjectNode(n2, {"table2.id2"});
        LUnionNode* n5 = new LUnionNode(n3, n4);
        PGetNextNode* q1 = QueryFactory(n5);
        q1->Print(0);
        ExecuteQuery(q1);
        delete n5;
        delete q1;
    }

}
