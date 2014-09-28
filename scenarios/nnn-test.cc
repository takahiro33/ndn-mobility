/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-test.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-test.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-test.cc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Standard C++ modules
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <vector>

// Random modules
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

// ns3 modules
#include <ns3-dev/ns3/applications-module.h>
#include <ns3-dev/ns3/bridge-helper.h>
#include <ns3-dev/ns3/buffer.h>
#include <ns3-dev/ns3/csma-module.h>
#include <ns3-dev/ns3/core-module.h>
#include <ns3-dev/ns3/mobility-module.h>
#include <ns3-dev/ns3/network-module.h>
#include <ns3-dev/ns3/point-to-point-module.h>
#include <ns3-dev/ns3/wifi-module.h>

// Extensions
#include "nnnSIM/nnnSIM-module.h"

using namespace ns3;
using namespace std;
using namespace boost;
using namespace nnn;

namespace br = boost::random;

char scenario[250] = "nnnTest";

NS_LOG_COMPONENT_DEFINE (scenario);

// Number generator
br::mt19937_64 gen;

// Obtains a random number from a uniform distribution between min and max.
// Must seed number generator to ensure randomness at runtime.
int obtain_Num(int min, int max)
{
	br::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

// Obtain a random double from a uniform distribution between min and max.
// Must seed number generator to ensure randomness at runtime.
double obtain_Num(double min, double max)
{
	br::uniform_real_distribution<> dist(min, max);
	return dist(gen);
}

int main (int argc, char *argv[])
{

	NNNAddress errorProne;
	NNNAddress working;

	try
	{
		NS_LOG_INFO("Too long address");
		errorProne = NNNAddress ("0.1.2.3.4.5.6.7.8.9.A.B.C.D.E.F.F");

	}
	catch (nnn::error::NNNAddress &e)
	{
		cout << "Error!" << endl;
	}

	try {
		NS_LOG_INFO("Not hex address");
		errorProne = NNNAddress ("A.4.5.U");

	}
	catch (nnn::error::NNNAddress &e)
	{
		cout << "Error!" << endl;
	}

	try {
		NS_LOG_INFO("Normal address");
		working = NNNAddress ("AE.34.25");
	}
	catch (nnn::error::NNNAddress &e)
	{
		cout << "Error!" << endl;
	}

	try {
		NS_LOG_INFO("Double dot address");
		errorProne = NNNAddress ("AE.34..25");
	}
	catch (nnn::error::NNNAddress &e)
	{
		cout << "Error!" << endl;
	}

	try {
		NS_LOG_INFO("Dot empty end address");
		errorProne = NNNAddress ("AE.34.25.");
	}
	catch (nnn::error::NNNAddress &e)
	{
		cout << "Error!" << endl;
	}

	NNNAddress dest ("A.34");
	NNNAddress maxd ("0.1.2.3.4.5.6.7.8.9.A.B.C.D.E.F");
	NNNAddress tests ("111111111111111.A");

	cout << "Working address is: " << working << endl;

	NNNAddress workingSector = working.getSectorName();

	cout << "Sector is: " << workingSector << endl;

	NNNAddress closestSector = working.getClosestSector(workingSector);
	NNNAddress closestSector2 = workingSector.getClosestSector(working);

	cout << "Closest working sector between " << working << " and " << workingSector << " is " << closestSector << endl;
	cout << "Closest working sector between " << workingSector << " and " << working << " is " << closestSector2 << endl;

	cout << "--------------------" << endl;
	NNNAddress closestSector3 = working.getClosestSector(dest);

	cout << "Closest working sector between " << working << " and " << dest << " is " << closestSector3 << endl;

	cout << "--------------------" << endl;
	NNNAddress closestSector4 = dest.getClosestSector(working);

	cout << "Closest working sector between " << dest << " and " << working << " is " << closestSector4 << endl;

	Buffer buf;

	size_t name_size = wire::NnnSim::SerializedSizeName(working);
	size_t name_sized = wire::NnnSim::SerializedSizeName(dest);
	size_t name_sizeM = wire::NnnSim::SerializedSizeName(maxd);
	size_t name_size15 = wire::NnnSim::SerializedSizeName(tests);

	cout << "Size obtained for " << working << " wire is " << name_size << endl;
	cout << "Size obtained for " << dest << " by wire is " << name_sized << endl;
	cout << "Size obtained for " << maxd << " by wire is " << name_sizeM << endl;
	cout << "Size obtained for " << tests << " by wire is " << name_size15 << endl;

	buf.AddAtStart(name_size);

	Buffer::Iterator i = buf.Begin();

	wire::NnnSim::SerializeName(i, working);

	Simulator::Stop (Seconds (1.0));
	Simulator::Run ();
	Simulator::Destroy ();

}
