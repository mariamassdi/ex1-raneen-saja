#include "Transaction.h"
//#include "Utilities.h"
using namespace std ;

Transaction* createTransaction(unsigned int value, const string& sender ,const string &receiver)
{
    Transaction* newTrans = new Transaction() ;
    newTrans->value = value ;
    newTrans->sender = sender;
    newTrans->receiver= receiver;
return newTrans ;
}
void TransactionDumpInfo(const Transaction& transaction, ofstream& file)
{
    file << "Sender Name: " << transaction.sender << std :: endl;
    file << "Receiver Name: " << transaction.receiver << std :: endl;
    file << "Transaction Value: " << transaction.value << std :: endl;
}


string TransactionHashedMessage(const Transaction& transaction)
{
    return hash(transaction.value, transaction.sender, transaction.receiver);
}

bool TransactionVerifyHashedMessage(const Transaction& transaction,string hashedMessage)
{
    string h_massege = TransactionHashedMessage(transaction);
    if(h_massege == hashedMessage)
    {
        return true;
    }
    return false;
}
