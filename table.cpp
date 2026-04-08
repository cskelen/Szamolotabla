#include "table.h"
#include <iostream>
#include <cstddef>

Table::Table(size_t N, size_t M) : numRows(N), numCols(M), cells(NULL){
    cells = new Cell*[numRows* numCols];
    for (size_t i = 0; i < numRows; i++){
        for (size_t j = 0; j < numCols; j++)
        {
            cells[i*numCols + j] = new NumberCell(10);
        }
    }
}

Cell& Table::at(size_t row, size_t col){
    if (row >= numRows || col >= numCols) throw "Index out of bounds!";
    return *cells[row*numCols + col];
}

const Cell& Table::at(size_t row, size_t col) const{
    if (row >= numRows || col >= numCols) throw "Index out of bounds!";
    return *cells[row*numCols + col];
}


Table::~Table(){
    for (size_t i = 0; i < numRows*numCols; i++)
        delete cells[i];
    delete[] cells;
}

void Table::kiir() const{
    for (size_t i = 0; i < numRows; i++){
        for (size_t j = 0; j < numCols; j++)
        {
            std::cout << (*this).at(i, j).getData() << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
class Stack{
    size_t len;
    size_t capacity;
    T* items;
    public:
        Stack() : len(0), capacity(10), items(new T[10]){}
        void push(T newItem) {
            if (len + 1 > capacity){
                capacity += 10;
                T* tmp = new T[capacity];
                for (size_t i = 0; i < len; i++)
                    tmp[i] = items[i];
                delete[] items;
                items = tmp;
            }
            items[len++] = newItem;
        }
        void pop() {if(len > 0) len--;}
        size_t size() const{return len;}
        T top() { if(len > 0) return items[len-1]; else throw "Nothing in stack!";}
        ~Stack(){delete[] items;}
};

Operator toOp(const char c){
    switch(c){
        case '+':
            return Operator::ADD;
        case '-':
            return Operator::SUB;
        case '*':
            return Operator::MULT;
        case '/':
            return Operator::DIV;
    }
    throw "Invalid Operator!";
}

Node* Table::parse(const char* input){
    size_t col = 0;
    size_t row = 0;
    int i = 0;
    int len = strlen(input);
    
    Stack<Node*> nodes;
    Stack<Node*> ops;

    while (i < len){
        if (std::isalpha(input[i])){
            col = 0;
            while (i < len && std::isalpha(input[i])){
                char c = std::toupper(input[i]);
                col = col*26+(c-'A');
                i++;
            }

            row = 0;
            while (i < len && std::isdigit(input[i])){
                row = row*10+(input[i] - '0');
                i++;
            }
            nodes.push(new CellNode(&at(row-1, col)));
        }
        else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/'){
            Operator currOp = toOp(input[i]);
            if (ops.size() != 0){
                while(ops.size() != 0){
                    Node* top = ops.top();
                    if (top->getType() != NodeType::OP) throw "Type Error!";
                    Operator topOp = static_cast<OpNode*>(top)->getOp();

                    if(topOp >= currOp){
                        Node* r = nodes.top();
                        nodes.pop();
                        Node* l = nodes.top();
                        nodes.pop();

                        top->setChildren(l, r);
                        nodes.push(top);
                        ops.pop();
                    }
                    else break;
                }
            }
            ops.push(new OpNode(currOp, nullptr, nullptr));
            i++;
        }
        else if (input[i] == ' ') i++;
        else throw "Invalid input!";
    }
    while (ops.size() != 0){
        Node* r = nodes.top();
        nodes.pop();
        Node* l = nodes.top();
        nodes.pop();

        ops.top()->setChildren(l, r);

        nodes.push(ops.top());
        ops.pop();
    }
    return nodes.top();
}
