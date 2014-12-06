/*
 * nnn-nnpt-test.cc
 *
 *  Created on: Dec 5, 2014
 *      Author: miyamoto
 */


/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-names-cointainer-test.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-names-cointainer-test.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-names-cointainer-test.cc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Extensions
#include "nnnSIM/nnnSIM-module.h"
#include "nnnSIM/model/nnst/nnn-nnpt.h"
#include "nnnSIM/model/nnst/nnn-nnpt-entry.h"

using namespace ns3;
using namespace std;
using namespace nnn;

int main (int argc, char *argv[])
{
	NNPT test1;

	NNNAddress nn_test1 ("be.54.32");
	NNNAddress nn_test2 ("af.67.31");
	NNNAddress nn_test3 ("ae.34.26");
//	NNNAddress nn_test4 ("zz.54.32");


	Time t_test1 = Seconds (20);
	Time t_test2 = Seconds (60);
	Time t_test3 = Seconds (10);

	Time updateTime = Seconds (80);

	NNPTEntry nnpte_test1 = NNPTEntry (nn_test1, nn_test2, t_test1);
	NNPTEntry nnpte_test2 = NNPTEntry (nn_test2, nn_test3, t_test2);
	NNPTEntry nnpte_test3 = NNPTEntry (nn_test3, nn_test1, t_test3);

	/*	NNNAddress nn_test1 ("be.54.32");
		NNNAddress nn_test2 ("af.67.31");
		NNNAddress nn_test3 ("ae.34.26");
		NNNAddress nn_test4 ("zz.54.32");
		NNNAddress nn_test5 ("zy.67.31");
		NNNAddress nn_test6 ("op.34.26");

		Time t_test1 = Seconds (20);
		Time t_test2 = Seconds (60);
		Time t_test3 = Seconds (10);
		Time t_test4 = Seconds (90);
		Time t_test5 = Seconds (50);
		Time t_test6 = Seconds (100);

		Time updateTime = Seconds (80);

		NNPTEntry nnpte_test1 = NNPTEntry (nn_test1, nn_test2, t_test1);
		NNPTEntry nnpte_test2 = NNPTEntry (nn_test5, nn_test6, t_test2);
		NNPTEntry nnpte_test3 = NNPTEntry (nn_test3, nn_test1, t_test3);
	*/

	test1.addEntry(nnpte_test1);
	test1.addEntry(nnpte_test2);
	test1.addEntry(nnpte_test3);

	std::cout << "We have a NamesContainer of size: " << test1.size() << std::endl;

	std::cout << "Printing ordering by address" << std::endl;
	test1.printByAddress();

	std::cout << "\"" << nn_test1 <<"\"'s New address is \"" << test1.findPairedName(nn_test1) << "\"" << std::endl;

	std::cout << "Printing ordering by lease expire time" << std::endl;
	test1.printByLease();

	std::cout << "Expire time for " << nn_test2 << " is " << test1.findNameExpireTime(nn_test2) << std::endl;
	std::cout << "Updating expire time for " << nn_test2 << " to " << updateTime << std::endl;

	test1.updateLeaseTime(nn_test2, updateTime);

	std::cout << "Deleting " << nn_test3 << " from container..." << std::endl;

	test1.deleteEntry(nn_test3);

	NNNAddress tmp = test1.findNewestName();

	std::cout << "Last address to die will be " << tmp << " at " << test1.findNameExpireTime(tmp) << std::endl;

	std::cout << "We have a NamesContainer of size: " << test1.size() << std::endl;
	std::cout << "Printing ordering by address" << std::endl;
	test1.printByAddress();

	Simulator::Stop (Seconds (70));
	Simulator::Run ();
	Simulator::Destroy ();

	std::cout << "Printing ordering by address" << std::endl;
	test1.printByAddress();

	std::cout << "\"" << nn_test1 <<"\"'s New address is \"" << test1.findPairedName(nn_test1) << "\"" << std::endl;

}


