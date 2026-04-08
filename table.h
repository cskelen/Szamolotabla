#include <cstddef>
#include <cstring>
#include <iostream>

class Cell{
    public:
        Cell() {}
        virtual const char* getData() const = 0;
        virtual void setData(const char* newData) = 0;
        virtual ~Cell() {}
};

class NumberCell : public Cell {
    char buffer[21];
    long long data;
    public:
        NumberCell(long long data = 0) : data(data){std::sprintf(buffer, "%lld", data);}
        NumberCell& operator=(const NumberCell& rhs) {data = rhs.data; return *this;}
       
        const char* getData() const { return buffer; }
        void setData(const char* newData) {
            int num = std::atoi(newData);
            this->data = num;
            std::sprintf(buffer, "%lld", data);
        }
        ~NumberCell() {}
};


enum NodeType {CELL, OP};
enum Operator {
    ADD,
    SUB,
    MULT,
    DIV,
    NONE
};

void parse(const char* input);

class Node{
    public:
        virtual long long eval() const = 0;
        virtual void setChildren(Node* left, Node* right) = 0;
        virtual NodeType getType() const = 0;
        virtual ~Node(){}
};

class OpNode : public Node{
    Operator op;

    Node* left;
    Node* right;
    public:
        OpNode(Operator op, Node* l, Node* r) : op(op), left(l), right(r) {}
        long long eval() const{
            long long leftVal = left->eval();
            long long rightVal = right->eval();
            if (op == Operator::ADD)
                return leftVal + rightVal;
            else if (op == Operator::SUB)
                return leftVal - rightVal;
            else if (op == Operator::MULT)
                return leftVal * rightVal;
            else if (op == Operator::DIV)
                return leftVal / rightVal;
            return 0;
        }
        void setChildren(Node* left, Node* right){
            this->left = left;
            this->right = right;
        }
        NodeType getType() const {return NodeType::OP;}
        Operator getOp() {return op;}
        ~OpNode(){
            delete left;
            delete right;
        }
};

class CellNode : public Node{
    Cell* cell;
    public:
        CellNode(Cell* cell) : cell(cell) {}
        long long eval() const{
            return std::atoi(cell->getData());
        }
        void setChildren(Node* left, Node* right) {}
        NodeType getType() const {return NodeType::CELL;}
        ~CellNode(){}
};

//TODO: eval általánosítás, string konverzió helyett másik megoldás
class ExpCell: public Cell{
    Node* root;
    public:
        ExpCell(Node* root): root(root) {}
        ExpCell() {}
        long long getEval() const {return root->eval();}
        const char* getData() const override {
            return "";
        }
        void setData(const char* newData) override {}
        ~ExpCell(){
            delete root;
        }
};

class TextCell : public Cell{
    const char* data;
    public:
        TextCell(const char* data = "") : data(data){}
        TextCell& operator=(const TextCell& rhs){ data = rhs.data; return *this;}
        const char* getData() const {return data;}
        void setData(const char* newData){
            data = newData;
        }
        ~TextCell() {}
};

class Table{
    size_t numRows;
    size_t numCols;
    Cell** cells;
    public:
        Table(size_t N = 1, size_t M = 1);
        ~Table();

        Cell& at(size_t row, size_t col);
        const Cell& at(size_t row, size_t col) const;
        void kiir() const;
        Node* parse(const char* input);
};
