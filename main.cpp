#include "BlockChain.h"
#include <iostream>
#include <fstream>
#include <string>
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    if(argc!=4)
    {
        getErrorMessage();
    }
    else
    {
        cout << argv[3] ;
        ifstream source(argv[2]);
        if (!source || !source.is_open())
        {
            return 0;
        }
        ofstream target(argv[3]);
        if (!target || !target.is_open())
        {
            source.close();
            return 0;
        }
        std :: string op = argv[1];
        BlockChain block = BlockChainLoad(source);

        if (op == "format")
        {
            BlockChainDump(block,target);
        }
        if (op == "hash")
        {
            BlockChainDumpHashed(block, target);
        }
        if (op == "compress")
        {
            BlockChainCompress(block);
            BlockChainDump(block,target);
        }
        if (op == "verify")
        {
            if (BlockChainVerifyFile(block,source))
            {
                target << "Verification passed" << endl;
            }
            else{
                target << "Verification failed" << endl;
            }
        }
        source.close();
        target.close();

    }
return 0;

}