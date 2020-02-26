#pragma once
#include <iostream>
using std::cout; using std::endl;
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <sstream>
using std::ostringstream;

struct Node {
  string key{};
  int value{};
  Node *next = nullptr;
  Node *prev = nullptr;
  Node() = default;
  Node(string, int, Node * = nullptr, Node * = nullptr);
  bool operator==(const Node &) const;
  friend ostream &operator<<(ostream &out, const Node &n) {
    out << n.key << ":" << n.value;
    return out;
  }
};

Node::Node(string key_, int value_, Node *prev_, Node *next_) {
  key = key_;
  value = value_;
  prev = prev_;
  next = next_;
}

bool Node::operator==(const Node &n) const {
  if ((key == n.key) && (value == n.value)) {
    return true;
  } else {
    return false;
  }
}

class DLL {
private:
  Node *head_ = nullptr;
  Node *tail_ = nullptr;
  size_t sz_ = 0;

public:
  DLL() = default;
  DLL(vector<Node>);
  DLL(const DLL &);
  DLL &operator=(DLL list);
  ~DLL();
  Node *get_head();
  Node *get_tail();
  size_t get_size();
  Node add_back(string, int);
  Node add_front(string, int);
  Node add_after(string, string, int);
  Node search(string);
  Node remove_front();
  Node remove_back();
  Node remove_key(string);
  int update_value(string, int);
  friend ostream &operator<<(ostream &out, const DLL &list) {
    auto itr = list.head_;
    ostringstream oss;
    for (; itr != nullptr; itr = itr->next) {
      oss << itr->key << ":" << itr->value << ", ";
    }
    string ans = oss.str();
    ans = ans.substr(0, ans.size() - 2);
    out << ans;
    return out;
  }
};

Node *DLL::get_head() { return head_; }
Node *DLL::get_tail() { return tail_; }
size_t DLL::get_size() { return sz_; }

DLL::~DLL() {
  while (head_)
    delete std::exchange(head_, head_->next);
}

DLL & DLL::operator =(DLL list) {
  using std::swap;
  swap(head_,list.head_);
  swap(tail_,list.tail_);
  swap(sz_,list.sz_);
  return *this;
}

DLL::DLL(const DLL &list) {
  for (Node *p = list.head_; p; p = p->next)
    add_back(p->key, p->value);
}

DLL::DLL(vector<Node> vec) {
  for (const auto &node : vec)
    add_back(node.key, node.value);
}

Node DLL::add_back(string str, int num) {
    //Allocates memory for a new node and sets the values to the given arguments
    Node *new_node = new Node;
    new_node->key = str;
    new_node->value = num;
    //Checks if the list is empty
    if (head_ != nullptr) {
        new_node->prev = tail_;
        new_node->next = nullptr;
        tail_->next = new_node;
        tail_ = new_node;
        sz_++;
    }
    //Sets both head and tail to the new node if list is empty
    else {
        head_ = new_node;
        tail_ = new_node;
        sz_++;
    }
    return *new_node;
}

Node DLL::add_front(string str, int num) {
    //Allocates memory for a new node and sets the values to the given arguments
    Node *new_node = new Node;
    new_node->key = str;
    new_node->value = num;
    //Checks if list is empty
    if (head_ != nullptr) {
        new_node->prev = nullptr;
        new_node->next = head_;
        head_->prev = new_node;
        head_ = new_node;
        sz_++;
    }
    //Sets both head and tail to the new node if list is empty
    else {
        head_ = new_node;
        tail_ = new_node;
        sz_++;
    }
    return *new_node;
}


Node DLL::add_after(string add_key, string str, int num) {
    Node *new_node = new Node;
    new_node->key = str;
    new_node->value = num;
    //Creates a pointer to a temporary node that will traverse the doubly linked list
    Node *temp_node = head_;
    //Allocates memory for a empty node 
    Node *empty_node = new Node;
    empty_node->key = "";
    empty_node->value = 0;
    //Iterates through the doubly linked list
    while(temp_node != nullptr) {
        //Checks if the key matches the key provided by the argument
        if (temp_node->key == add_key) {
            //Checks if it is currently at the tail node
            if (temp_node->next == nullptr) {
                new_node->prev = tail_;
                new_node->next = nullptr;
                tail_->next = new_node;
                tail_ = new_node;
                sz_++;
                return *new_node;
            }
            else {
                new_node->next = temp_node->next;
                new_node->prev = temp_node;
                temp_node->next = new_node;
                new_node->next->prev = new_node;
                sz_++;
                return *new_node;
            }
        }
        temp_node = temp_node->next;
        
    }
    return *empty_node;
}


Node DLL::remove_back() {
    //Allocates memory for an empty node
    Node *empty_node = new Node;
    empty_node->key = "";
    empty_node->value = 0;
    //Checks if the list is empty
    if (head_ != nullptr) {
        //Checks if the list has only size 1
        if (sz_ == 1) {
            Node *copy_node = new Node;
            copy_node->key = head_->key;
            copy_node->value = head_->value;
            Node temp = *copy_node;  
            head_ = nullptr;
            tail_ = nullptr;
            sz_--;
            delete copy_node;
            return temp;
        }
        Node *copy_node = new Node;
        copy_node->key = tail_->key;
        copy_node->value = tail_->value;
        Node temp = *copy_node;
        tail_ = tail_->prev;
        tail_->next = nullptr;
        sz_--;
        //Checks if the list has size 1, after removing the back node
        if (sz_ == 1) {
            tail_->prev = nullptr;
            tail_->next = nullptr;
            head_ = tail_;
        }
        delete copy_node;
        return temp;
    }
    else {
        return *empty_node;
    }
}

Node DLL::remove_front() {
    //Allocates memory for an empty node
    Node *empty_node = new Node;
    empty_node->key = "";
    empty_node->value = 0;
    //Checks if the list is empty
    if (head_ != nullptr) {
        //Accounts for the case when there is onle one node in the list
        if (sz_ == 1) {
            //Allocates memory for a copy of a node to be used for temporary storage, a copy of this node will be returned
            Node *copy_node = new Node;
            copy_node->key = head_->key;
            copy_node->value = head_->value;
            Node temp = *copy_node;  
            head_ = nullptr;
            tail_ = nullptr;
            sz_--;
            delete copy_node;
            return temp;
        }
        Node *copy_node = new Node;
        copy_node->key = head_->key;
        copy_node->value = head_->value;
        Node temp = *copy_node;
        head_ = head_->next;
        head_->prev = nullptr;
        sz_--;
        //Sets the head and tail equal to each other, when there is only one node left in the list
        if (sz_ == 1) {
            tail_->prev = nullptr;
            tail_->next = nullptr;
            head_ = tail_;
        }
        delete copy_node;
        return temp;
    }
    else {
        return *empty_node;
    }  
}

Node DLL::remove_key(string to_remove) {
    Node *temp_node = head_;
    //Allocates memory for an empty node
    Node *empty_node = new Node;
    empty_node->key = "";
    empty_node->value = 0;
    //Iterates through the list
    while(temp_node != nullptr) {
        if (temp_node->key == to_remove) {
            //Accounts for the case where there is only one node in the list
            if (sz_ == 1) {
                head_ = nullptr;
                tail_ = nullptr;
                sz_--;
                Node ret = *temp_node;
                delete temp_node;
                return ret;
            }
            //Accounts for the case where there are two nodes in the list and must determine whether the tail or the head node is being removed
            if (sz_ == 2) {
                if (temp_node->next == nullptr) {
                    head_->next = nullptr;
                    head_->prev = nullptr;
                    tail_= head_;
                    Node ret = *temp_node;
                    sz_--;
                    delete temp_node;
                    return ret;
                }
                else {
                   tail_ -> next = nullptr;
                   tail_ -> prev = nullptr;
                   head_ = tail_;
                   Node ret = *temp_node;
                   sz_--;
                   delete temp_node;
                   return ret;
                }
            }
            //Accounts for the case where the removed node is the tail node.
            if (temp_node->next == nullptr) {
                 tail_ = tail_->prev;
                 tail_->next = nullptr;
                 Node ret = *temp_node;
                 sz_--;
                 delete temp_node;
                 return ret;
            }
            //Accounts for the case where the removed node is the head node.
            if (temp_node->prev == nullptr) {
                head_ = head_->next;
                head_->prev = nullptr;
                Node ret = *temp_node;
                sz_--;    
                delete temp_node;
                return ret;
            }
            temp_node->prev->next = temp_node->next;
            temp_node->next->prev = temp_node->prev;
            Node ret = *temp_node;
            sz_--;
            delete temp_node;
            return ret;
        }
        temp_node = temp_node->next;
    }
    return *empty_node;
}

Node DLL::search(string find_key) {
    Node *temp_node = head_;
    Node *empty_node = new Node;
    empty_node->key = "";
    empty_node->value = 0;
    //Iterates through the list
    while(temp_node != nullptr) {
        //Returns the node with the matching key if found
        if (temp_node->key == find_key) {
            return *temp_node;
        }
        temp_node = temp_node->next;
    }
    return *empty_node;
}

int DLL::update_value(string find_key, int new_value) {
    Node *temp_node = head_;
    //Iterates through the list
    while(temp_node != nullptr) {
        //Updates value if a node with a matching key is found
        if (temp_node->key == find_key) {
            temp_node->value = new_value;
            return new_value;
        }
        temp_node = temp_node->next;
    }
    return -1;
}


