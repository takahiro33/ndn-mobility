/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnst-test.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnst-test.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst-test.cc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Extensions
#include "nnnSIM/nnnSIM-module.h"
#include "nnnSIM/model/nnst/nnn-nnst.h"
#include "nnnSIM/model/nnst/nnn-nnst-entry.h"

using namespace ns3;
using namespace std;
using namespace nnn;

int main (int argc, char *argv[])
{
	NamesContainer test1;

	NNNAddress nn_test1 ("be.54.32");
	NNNAddress nn_test2 ("af.67.31");
	NNNAddress nn_test3 ("ae.34.26");

	Simulator::Stop (Seconds (70));
	Simulator::Run ();
	Simulator::Destroy ();

}
