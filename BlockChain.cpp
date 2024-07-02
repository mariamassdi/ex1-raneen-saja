#include "BlockChain.h"
#include <sstream>
using namespace std ;
singleBlock createSingleBlock(const string& time, const Transaction& transaction)
 {
     singleBlock block = new SingleBlock();
     Transaction* trans = createTransaction(transaction.value , transaction.sender,
                                           transaction.receiver);
     block->transaction = *trans; 
     block->nextBlock = nullptr ;
     block->time=time;
     delete trans ;
     return block ;
 }
BlockChain createBlockChain()
{
    BlockChain blockChain;
    Transaction* trans = createTransaction(0,"","");
    blockChain.head = createSingleBlock("",*trans);
    return blockChain;
}
void destroyBlockChain(BlockChain& blockChain)
{
    if(!blockChain.head)
    {
        return;
    }
    singleBlock ptr = blockChain.head;
    while(ptr)
    {
        singleBlock toDelete = ptr ;
        ptr = ptr->nextBlock ;
        delete toDelete;
    }
    blockChain.head= nullptr;
    return;
}
int BlockChainGetSize(const BlockChain& blockChain)
{
    singleBlock ptr = blockChain.head;
    int size = 0 ;
    while(ptr)
    {
        size++;
        ptr=ptr->nextBlock;
    }
    return size;
}
int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name)
{
    int balance = 0 ;
    singleBlock current = blockChain.head->nextBlock;
    //const BlockChain* current = &blockChain;

    while(current)
    {
        if(current->transaction.receiver==name)
        {
            balance+=current->transaction.value;
        }
        else if(current->transaction.sender==name)
        {
            balance-=current->transaction.value;
        }
        current=current->nextBlock;
    }
    return  balance;
}
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
)
{

    Transaction* toAppend = createTransaction(value,sender,receiver);
    singleBlock newBlock = createSingleBlock(timestamp,*toAppend);
    newBlock->nextBlock = blockChain.head->nextBlock;
    blockChain.head->nextBlock = newBlock ;
    //delete toAppend ;
}

void BlockChainAppendTransaction(BlockChain& blockChain,const Transaction& transaction,
                                 const string& timestamp)
{
    singleBlock block = createSingleBlock(timestamp,transaction);
    block->nextBlock = blockChain.head->nextBlock;
    blockChain.head->nextBlock=block;
}
/**
 * BlockChainLoad - Reads data from a file and creates a new block chain
 *
 * @param file Data file to read from
 *
 * @return BlockChain created from the file
 *
*/
BlockChain BlockChainLoad(ifstream& file)
{
    string line , time , receiver , sender ;
    unsigned  int  value ;
//    getline(file,line);
//    std::istringstream iss(line);
//    iss>>sender>>receiver>>value>>time ;
//    Transaction* trans = createTransaction(value , sender , receiver);
//    //singleBlock* head = createSingleBlock(time,*trans);
    BlockChain blockChain = createBlockChain();
//    //blockChain.head->nextBlock = head;
//    BlockChainAppendTransaction(&blockChain , *trans, time);
    //head=head->nextBlock;
    while(getline(file,line))
    {
        std::istringstream iss(line);
        iss >> sender >> receiver >> value >> time ;
        BlockChainAppendTransaction(blockChain,value,sender,receiver,time);
        //head=head->nextBlock;
    }
    return blockChain;
}

/**
 * BlockChainDump - Prints the data of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 *
 * BlockChain info:
 * <n>.
 * Sender Name: <name>
 * Receiver Name: <name>
 * Transaction Value: <value>
 * Transaction Timestamp: <time>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
void BlockChainDump(const BlockChain& blockChain, ofstream& file)
{
    singleBlock current = blockChain.head->nextBlock;
    file << "BlockChain Info: "<< endl;
    int counter = 1 ;
    BlockChain temp = createBlockChain();
    while(current)
    {
        BlockChainAppendTransaction(temp,current->transaction,current->time);
        current = current->nextBlock;
    }
    singleBlock ptr = temp.head->nextBlock;
    while(ptr)
    {
        file << counter << "." << endl ;
        counter++ ;
        TransactionDumpInfo(ptr->transaction,file);
        file << "Transaction timestamp: " << ptr->time << endl;
        ptr=ptr->nextBlock;
    }
    destroyBlockChain(temp);
}
void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file)
{
    singleBlock current = blockChain.head->nextBlock ;
    string hashed = "" ;
    BlockChain temp = createBlockChain();
    while(current)
    {
        BlockChainAppendTransaction(temp,current->transaction,current->time);
        current = current->nextBlock;
    }
    singleBlock ptr = temp.head->nextBlock ;
    while(ptr->nextBlock)
    {
        hashed = TransactionHashedMessage(ptr->transaction);
        file << hashed << endl ;
        ptr= ptr->nextBlock;
    }
    hashed = TransactionHashedMessage(ptr->transaction);
    file << hashed;
    destroyBlockChain(temp);

}
bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file)
{
    string line ;
    singleBlock current = blockChain.head->nextBlock ;
    bool flag;
    while(getline(file,line))
    {
        flag = TransactionVerifyHashedMessage(current->transaction,line);
        if (!flag)
        {
            return flag ;
        }
        current=current->nextBlock ;
    }
    return true ;
}


/**
 * BlockChainCompress - Compresses the given block chain based on the transaction's data.
 * All consecutive blocks with the same sender and receiver will be compressed to one Block.
 *
 * @param blockChain BlockChain to compress
*/
void BlockChainCompress(BlockChain& blockChain)
{
    singleBlock current = blockChain.head->nextBlock;
    singleBlock next = current->nextBlock;

    while(next)
    {
        if(current->transaction.sender==next->transaction.sender &&
                current->transaction.receiver==next->transaction.receiver)
        {
            current->transaction.value += next->transaction.value;
            current->time=next->time;

            current->nextBlock = next->nextBlock;
            delete next;
            next = current->nextBlock;
        }
        else {
            current = next;
            next = current->nextBlock;
        }
    }
}
void BlockChainTransform(BlockChain& blockChain, updateFunction function)
{
    singleBlock current = blockChain.head->nextBlock ;
    while (current)
    {
        current->transaction.value =function(current->transaction.value);
        current=current->nextBlock;
    }
}
