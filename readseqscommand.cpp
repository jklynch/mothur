/*
 *  readseqscommand.cpp
 *  Mothur
 *
 *  Created by Thomas Ryabin on 4/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "readseqscommand.h"

//**********************************************************************************************************************
ReadSeqsCommand::ReadSeqsCommand(){
	try {
		globaldata = GlobalData::getInstance();
		
	}
	catch(exception& e) {
		cout << "Standard Error: " << e.what() << " has occurred in the ReadOtuCommand class Function ReadOtuCommand. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}
	catch(...) {
		cout << "An unknown error has occurred in the ReadOtuCommand class function ReadOtuCommand. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}
}

//**********************************************************************************************************************

ReadSeqsCommand::~ReadSeqsCommand(){
	//delete readFasta->getDB();
//	delete readNexus->getDB();
//	delete readClustal->getDB();
//	delete readPhylip->getDB();
}

//**********************************************************************************************************************

int ReadSeqsCommand::execute(){
	try {
		filebuf fb;
		
		//fb.open ("fasta.txt",ios::out);
//		readFasta->read();
//		SequenceDB* db = readFasta->getDB();
		
		//fb.open("nexus.txt",ios::out);
//		readNexus->read();
//		SequenceDB* db = readNexus->getDB();	

		//fb.open("clustal.txt",ios::out);
//		readClustal->read();
//		SequenceDB* db = readClustal->getDB();
		
		//fb.open("phylip.txt",ios::out);
//		readPhylip->read();
//		SequenceDB* db = readPhylip->getDB();

		
		
		//for(int i = 0; i < db->size(); i++) {
//			cout << db->get(i).getLength() << "\n" << db->get(i).getName() << ": " << db->get(i).getUnaligned() << "\n\n";
//		}

		//ostream os(&fb);
//		db->print(os);
//		fb.close();

		return 0;
	}
	catch(exception& e) {
		cout << "Standard Error: " << e.what() << " has occurred in the ReadOtuCommand class Function execute. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}
	catch(...) {
		cout << "An unknown error has occurred in the ReadOtuCommand class function execute. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}
}
//**********************************************************************************************************************