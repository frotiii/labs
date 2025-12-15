#include <iostream>
#include <string>
#include <cstring>



class CustomStack {

    public:
    
        CustomStack() {
            mHead = nullptr;
        }
    
        void push(int val){
            ListNode *new_node = new ListNode;
            new_node->mData = val;
            if(mHead == nullptr){
                new_node->mNext = nullptr;
                mHead = new_node;
            } else {
                new_node->mNext = mHead;
                mHead = new_node;
            }
    
        }
    
        void pop(){
            if(mHead == nullptr) throw std::runtime_error("error");
    
            ListNode *ex_head = mHead;
            mHead = mHead->mNext;
            delete ex_head;
        }
    
        int top(){
            if(mHead == nullptr) throw std::runtime_error("error");
            return mHead->mData;
        }
    
        size_t size(){
            if(mHead == nullptr) return 0;
            ListNode *tmp = mHead;
            int size = 0;
            while(tmp != nullptr){
                size++;
                tmp = tmp->mNext;
            }
            return size;
        }
    
        bool empty(){
            if(mHead == nullptr) return true;
            return false;
        }

    protected:  
        ListNode* mHead;
};

int read();

int main(){
    try{
        int result = read();
        std::cout<<result;
    }
    catch(const std::exception &err){
        std::cout << err.what();
        return 1;
    }
    return 0;
}
 
int read(){
    CustomStack stack;
     const char *token = "+-*/";
    std::string input;

    while(std::cin >> input){
        if(input.length() == 1 && strchr(token, input[0])){

            if(stack.size() < 2){
                throw std::runtime_error("error");
            }

            const int a = stack.top(); stack.pop();
            const int b = stack.top(); stack.pop();
            const char oper = input[0];

            switch(oper){
                case '+':
                    stack.push(a + b);
                    break;
                case '-':
                    stack.push(b-a);
                    break;
                case '/':
                    if(b == 0) throw std::runtime_error("error");
                    stack.push(a/b);
                    break;
                case '*':
                    stack.push(a * b);
                    break;
                default:
                    throw std::runtime_error("error");

            }
        } else {
            stack.push(std::stoi(input));
        }  
        if(std::cin.peek() == '\n') break; 
    }

    if(stack.size() != 1){
        throw std::runtime_error("error");
    }

    return stack.top();
}