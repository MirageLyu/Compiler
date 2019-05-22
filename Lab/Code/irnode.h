//
// Created by miragelyu on 19-5-13.
//
#pragma once

#include "headers.h"

class irnode{
private:
    irnode* nxt;
    irnode* last;
    enum { Empty, Exp, } Irtype;
public:
    irnode(){
        nxt = NULL;
        last = NULL;
        Irtype = Empty;
    }

    irnode(const irnode& i) = default;
    irnode operator=(const irnode& i) = default;

    friend IRLinkedList;

    string tostring();
};

class IRLinkedList{
private:
    irnode* head;
    irnode* last_node;
    int sz;
public:
    IRLinkedList(){
        //Empty head node
        head = new irnode();
        last_node = head;
        sz = 0;
    }

    void push_back(const irnode*);
    void push_front(const irnode*);
    void pop_back();
    void pop_front();
    irnode* get_back() const;
    irnode* get_front() const;
};
