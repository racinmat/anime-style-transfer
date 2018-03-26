/**
 * Implements an example program that shows how to easily work with referenced file paths delivered
 * within the extension page of an ADTF dat-file.
 *
 * This example shows:
 * \li how to read and write referenced file paths from and to an ADTF dat-file respectively
 * \li how to iterate over referenced file paths
 * \li how to remove, add or replace referenced file paths
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: axasil2 $
 * $Date: 2013-03-14 10:47:58 +0100 (Do, 14 Mrz 2013) $
 * $Revision: 19131 $
 *
 * @remarks Introduced in streaming lib Version 2.8.
 *
 */

#include <iostream>         //std::cout, std::endl
#include <cstring>          //std::string
#include <adtf_streaming.h>

using namespace adtfstreaming;

//prototype to print referenced file paths
tVoid PrintReferencedFilePaths(const IReferencedFilesExtension* const i_pReferencedFiles);

//main function
int main(int argc, char* argv[])
{
    if (2 != argc)
    {
        std::cout << "usage: referencedfiles.exe <datfile>\n" << std::endl;
        return -1;
    }

    //create the referenced files extension object reading from file
    const std::string strFilePath(argv[1]);
    IReferencedFilesExtension* const pReferencedFiles = IReferencedFilesExtension::Create();
    if (ERR_NOERROR != pReferencedFiles->Read(strFilePath.c_str()))
    {
        std::cout << "Error reading from file" << std::endl;
        //cleanup
        IReferencedFilesExtension::Release(pReferencedFiles);
        return -1;
    }

    //cycle through all referenced files and print them
    std::cout << "The original referenced files: " << std::endl;
    PrintReferencedFilePaths(pReferencedFiles);

    //remove this very first referenced file path
    std::string strFirstReferencedFile; //backup first referenced file
    if (pReferencedFiles->Get(0) != NULL)
    {
        strFirstReferencedFile = pReferencedFiles->Get(0);
    }
    const tChar* strCurrentReferencedFile = pReferencedFiles->Get(0);
    pReferencedFiles->Remove(strCurrentReferencedFile);

    std::cout << std::endl << "After removing the first referenced file: " << std::endl;
    PrintReferencedFilePaths(pReferencedFiles);

    //add the backed up (former first) referenced file path as a new one
    pReferencedFiles->Add(strFirstReferencedFile.c_str());

    std::cout << std::endl << "After adding the former first referenced file: " << std::endl;
    PrintReferencedFilePaths(pReferencedFiles);

    //replace the last referenced file with a self chosen one
    //If no referenced file paths exists, this call evaluates to '0 - 1' which means out of range
    //In this case, Get() returns NULL, forcing the subsequent call to Replace() appending one
    //referenced file path instead of replacing it.
    strCurrentReferencedFile = pReferencedFiles->Get(pReferencedFiles->GetCount() - 1);
    pReferencedFiles->Replace(strCurrentReferencedFile, "./path/to/adtf_file.dat");

    std::cout << std::endl << "After replacing the last referenced file: " << std::endl;
    PrintReferencedFilePaths(pReferencedFiles);

    //the changed referenced file paths must explicitely be written to the hard drive, otherwise
    //changes will be lost after call to Relase()
    //Note: This will overwrite the existing file, no backup is performed!
    pReferencedFiles->Write();

    //cleanup
    IReferencedFilesExtension::Release(pReferencedFiles);
    return 0;
}


//definition of the prototype
tVoid PrintReferencedFilePaths(const IReferencedFilesExtension* const i_pReferencedFiles)
{
    tSize nCount = 0;
    while(i_pReferencedFiles->GetCount() > nCount)
    {
        std::cout << '\t' << i_pReferencedFiles->Get(nCount) << std::endl;
        ++nCount; //next loop condition
    }
}
